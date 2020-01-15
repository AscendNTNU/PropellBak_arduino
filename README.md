# PropellBak_arduino
ROS node kjørende på STM32(f103c8t6) for å styre den 5. propellen på dronen for team 2020.

Sender og mottar en std_msgs::UInt32MultiArray melding med 10 verdier.

## For å teste koden kjør følgende kommandoer:
Terminal 1
```sh
roscore
```
Terminal 2
```sh
rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
```
