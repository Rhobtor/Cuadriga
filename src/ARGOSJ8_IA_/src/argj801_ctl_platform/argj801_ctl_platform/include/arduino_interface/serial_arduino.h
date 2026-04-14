#ifndef SERIAL_ARDUINO_H
#define SERIAL_ARDUINO_H


#include <cstdint>
#include <limits.h>
#include <vector>
#include <iostream>
#include <memory>
#include <string>

#define X 0
#define Y 1
#define Z 2

#define Kv 128
#define Kw 128

#define PING 0xAA
#define START 0xA0
#define STOP 0xB5
#define MOVE 0xC6
#define SET_SPEED 0xC7
#define GET_SPEED 0xC8
#define ACK 0xAB
#define START_DATA_1 0xA1
#define START_DATA_2 0xA3

#define MAX_SPEED 0x02

//ERROR CODES
#define CHECKSUM_ERROR 0xD1
#define BAD_COMMNAND_ERROR 0xD2
#define NOT_STARTED_ERROR 0xD3
#define BAD_DATA_ERROR 0xD4

#define ATTEMPTS 5
class SerialArduino
{
private:

  int fd; //! The file descriptor
  bool is_started;
  int16_t counter;


  void create_checksum(uint8_t *buffer_in,int buffer_len,uint8_t* checksum);
  int send(uint8_t *buffer, int buffer_len);
  int read_with_timeout(uint8_t *buff, std::size_t count, int timeout);
  bool transac(std::uint8_t *buffer, uint8_t buffer_size, bool checksum, uint8_t *b_out);

public:
  SerialArduino();

  void openPort(const char *port_name);
  void closePort();
  void start();
  void stop();
  void move(int16_t v_lineal,int16_t v_angular,int16_t inc_counter);
  void ping();
  uint8_t set_speed(uint8_t speed);
  uint8_t get_speed();
  void sendSetPoints(std::vector<double> linear,std::vector<double> angular);
  bool getIs_started() const;
};

class ArduinoException  : public std::exception {
    private:	
    std::string error;
    public:
    ArduinoException(const std::string msg) : error(msg) {}

    const char* what() const noexcept override {
        return error.c_str();
}
};

#endif // SERIAL_ARDUINO_H
