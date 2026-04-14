## PACKAGE TO COMMUNICATE WITH ROVERJ8 WITH LCM

It is needed to install this library. Maybe ROS install it. 

`sudo apt-get install libyaml-cpp-dev`

## HOW TO AUTOMATE MULTICAST NETWORK ROUTE CONFIGURATION

Create a new script

`sudo nano /usr/local/bin/add-route.sh`

Write this bash code and save it

```#!/bin/bash
# Espera hasta que eno1 esté arriba
while ! ip link show eno1 | grep -qw "state UP"; do
    sleep 1
done

# Añade la ruta
/usr/sbin/ip route add 224.0.0.0/4 dev eno1
```

Assigns execution permissions to the script

`sudo chmod +x /usr/local/bin/add-route.sh`

Create another script

`sudo nano /etc/systemd/system/add-route.service`

Write the configuration and save it

```[Unit]
Description=Añadir ruta de red al inicio
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/add-route.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
```

Activate de the service 

`sudo systemctl enable add-route.service`

`route`

`224.0.0.0       0.0.0.0         240.0.0.0       U     0      0        0 eno1`