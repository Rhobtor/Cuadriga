#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#include <sys/time.h>

#include "arduino_interface/serial_arduino.h"
#include <poll.h>

// -----------------------------------------------------------------------------
// SerialArduino
// -----------------------------------------------------------------------------
// Driver de comunicación con el Arduino por puerto serie.
//
// Responsabilidades:
// - Abrir/configurar el dispositivo (/dev/ttyUSB*, etc.) a 57600 baudios.
// - Enviar comandos binarios al Arduino y esperar ACK/errores.
// - Implementar reintento simple en caso de CHECKSUM_ERROR.
//
// Nota: el protocolo (comandos START/STOP/PING, etc.) y las constantes ACK/*
// están definidas en el header `arduino_interface/serial_arduino.h`.
// -----------------------------------------------------------------------------

SerialArduino::SerialArduino():counter(0)
{
  closePort();
}

/** Open the IMU port
 * @brief serial_arduino::openPort
 * @param port_name
 */
void SerialArduino::openPort(const char *port_name)
{
  closePort(); // In case it was previously open, try to close it first.

  // Open the port.
  // - O_NOCTTY: asegura que el tty no se convierta en controlling terminal.
  // - O_NONBLOCK: evitamos bloquear al abrir; luego usamos poll/read con timeout.
  fd = open(port_name, O_RDWR | O_SYNC | O_NONBLOCK | O_NOCTTY, S_IRUSR | S_IWUSR );
  if (fd < 0)
  {
    const char *extra_msg = "";
    switch (errno)
    {
    case EACCES:
      extra_msg = "You probably don't have premission to open the port for reading and writing.";
      break;
    case ENOENT:
      extra_msg = "The requested port does not exist. Is the Arduino connected? Was the port name misspelled?";
      break;
    }

    throw ArduinoException("Unable to open serial port " + std::string(port_name) +". " + std::string(strerror(errno)) + ". "+ extra_msg);
  }

  // Lock the port
  struct flock fl;
  fl.l_type   = F_WRLCK;
  fl.l_whence = SEEK_SET;
  fl.l_start = 0;
  fl.l_len   = 0;
  fl.l_pid   = getpid();

  if (fcntl(fd, F_SETLK, &fl) != 0)
    throw ArduinoException("Device " + std::string(port_name) +" is already locked. Try 'lsof | grep" + std::string(port_name) +"' to find other processes that currently have the port open");

  // Change port settings.
  // cfmakeraw: modo "raw" (sin traducción de caracteres) para un protocolo binario.
  struct termios term;
  if (tcgetattr(fd, &term) < 0)
    throw ArduinoException("Unable to get serial port attributes. The port you specified"+ std::string(port_name)+ "may not be a serial port.");

  cfmakeraw( &term );
  cfsetispeed(&term, B57600);
  cfsetospeed(&term, B57600);

  if (tcsetattr(fd, TCSAFLUSH, &term) < 0 )
    throw ArduinoException("Unable to set serial port attributes. The port you specified ("+std::string(port_name)+") may not be a serial port.");

  // Make sure queues are empty before we begin.
  // Si hay bytes residuales en el buffer del driver, podrían desalinear el framing.
  if (tcflush(fd, TCIOFLUSH) != 0)
    throw ArduinoException("Tcflush failed. Please report this error if you see it.");
}

/** Close the IMU port
 * @brief serial_arduino::closePort
 */
void SerialArduino::closePort()
{
  if (fd != -1)
  {
    if (close(fd) != 0)
      throw ArduinoException("Unable to close serial port: "+ std::string(strerror(errno)));
    fd = -1;
  }
  is_started=false;
}

bool SerialArduino::transac(std::uint8_t *buffer,uint8_t buffer_size,bool checksum,uint8_t *b_out) {
  uint8_t buffer_out[1];

  // El checksum se pone en los dos últimos bytes del buffer (según protocolo).
  if(checksum)
    create_checksum(buffer,buffer_size-2,&buffer[buffer_size-2]);

  // Enviamos y esperamos respuesta (ACK o error) con timeout.
  send(buffer,buffer_size);
  read_with_timeout(buffer_out,1,1000);

  switch(buffer_out[0]) {
  case ACK:
    return true;
    break;
  case CHECKSUM_ERROR:
    // Reintento 1 vez: recalculamos checksum y reenviamos.
    create_checksum(buffer,buffer_size-2,&buffer[buffer_size-2]);
    send(buffer,buffer_size);
    read_with_timeout(buffer_out,1,1000);
    if(buffer_out[0]!=ACK)
      throw ArduinoException("Problem to comunicate with arduino");
    break;
  case BAD_COMMNAND_ERROR:
    throw ArduinoException("Bad command");
    break;
  case NOT_STARTED_ERROR:
    throw ArduinoException("Navigation not started");
    break;
  case BAD_DATA_ERROR:
    throw ArduinoException("Bad data");
    break;
  default:
    // Se devuelve el primer byte recibido para diagnóstico (si b_out != nullptr).
    b_out[0] = buffer_out[0];
    return true;
    break;
  }
  return false;
}

void SerialArduino::start() {

  uint8_t buffer[5];

  buffer[0] = START;
  buffer[1] = START_DATA_1;
  buffer[2] = START_DATA_2;

  if(transac(buffer,5,true,nullptr))
    is_started=true;
  else
    is_started=false;
}

void SerialArduino::stop() {
  uint8_t buffer[1];

  buffer[0] = STOP;

  if(transac(buffer,1,false,nullptr))
    is_started=false;

}

void SerialArduino::ping() {
  uint8_t buffer[1];

  buffer[0] = PING;

  transac(buffer,1,false,nullptr);

}
void SerialArduino::sendSetPoints(std::vector<double> linear,std::vector<double> angular) {

    int16_t v,w;
    
    //PROVISONAL UNTIL KNOW THE KINEMATICS
    if(linear[X]>=0 && linear[X]<=1.0)
      v = (SHRT_MAX - Kv)*linear[X]+Kv;
    else if(linear[X]>=-1.0 && linear[X]<0.0)
      v = -(SHRT_MIN + Kv)*linear[X]-Kv;
    else if(linear[X] > 1.0)
      v = SHRT_MAX;
    else
      v = SHRT_MIN;

    if(angular[Z]>=0 && angular[Z]<=1.0)
      w = (SHRT_MAX - Kw)*angular[Z]+Kw;
    else if(angular[Z]>=-1.0 && angular[Z]<0.0)
      w = -(SHRT_MIN + Kw)*angular[Z]-Kw;
    else if(angular[Z] > 1.0)
      w = SHRT_MAX;
    else
      w = SHRT_MIN;

    counter++;
    if(counter == SHRT_MAX)
      counter = SHRT_MIN;

    move(v,w,counter);
}

void SerialArduino::move(int16_t v_lineal,int16_t v_angular,int16_t inc_counter) {
  uint8_t buffer[9];

  buffer[0] = MOVE;
  buffer[1] = v_lineal >> 8;
  buffer[2] = v_lineal;
  buffer[3] = v_angular >> 8;
  buffer[4] = v_angular;
  buffer[5] = inc_counter >> 8;
  buffer[6] = inc_counter;

  transac(buffer,9,true,nullptr);

}

uint8_t SerialArduino::set_speed(uint8_t speed) {
  uint8_t buffer[4],b_out[1];

  buffer[0] = SET_SPEED;
  buffer[1] = speed;

  transac(buffer,4,true,b_out);

  return b_out[0];

}

uint8_t SerialArduino::get_speed() {
  uint8_t buffer[1],b_out[1];

  buffer[0] = GET_SPEED;

  transac(buffer,1,false,b_out);

  return b_out[0];


}

/** Creates the checksum of two bytes
 * @brief create_checksum
 * @param buffer_in buffer input (without checksum)
 * @param buffer_len length of the input_buffer
 * @param checksum buffer output with checkout of 2 bytes
 */
bool SerialArduino::getIs_started() const
{
  return is_started;
}

void SerialArduino::create_checksum(uint8_t *buffer_in,int buffer_len,uint8_t* checksum) {

  uint16_t cs = 0;
  for(int i=0;i<buffer_len;i++) {
    cs+=buffer_in[i];
  }
  checksum[0] = cs >> 8;
  checksum[1] = cs;
}

/** Send a packet to the IMU.
 * @brief microstrain_3dmgx2_imu::IMU::send
 * @param cmd
 * @param cmd_len
 * @return The number of bytes written.
 */
int SerialArduino::send(uint8_t *buffer, int buffer_len)
{
  int bytes;

  bytes = write(fd, buffer, buffer_len);

  if (bytes < 0)
    throw ArduinoException("Error writing to Arduino" + std::string(strerror(errno)));

  if (bytes != buffer_len)
    throw ArduinoException("whole message not written to Arduino");

  // Make sure the queue is drained
  // Synchronous IO doesnt always work
  if (tcdrain(fd) != 0)
    throw ArduinoException("Tcdrain failed");

  return bytes;
}

int SerialArduino::read_with_timeout(uint8_t *buff, std::size_t count, int timeout)
{
  ssize_t nbytes=-1;
  int retval;

  struct pollfd ufd[1];
  ufd[0].fd = fd;
  ufd[0].events = POLLIN;
  //  timeout=0;
  if (timeout == 0)
    timeout = -1; // For compatibility with former behavior, 0 means no timeout. For poll, negative means no timeout.

  if ( (retval = poll(ufd, 1, timeout)) < 0 )
    throw ArduinoException("poll failed " + std::string(strerror(errno)));

  if (retval == 0)
    throw ArduinoException("timeout reached");

  nbytes = read(fd, buff, count);

  if (nbytes < 0)
    throw ArduinoException("read failed  [" + std::string(strerror(errno))+"]");

  return nbytes;
}
