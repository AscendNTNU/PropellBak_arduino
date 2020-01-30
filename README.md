# PropellBak_arduino
ROS node kjørende på STM32(f103c8t6) for å styre den 5. propellen på dronen for team 2020.

Sender og mottar en std_msgs::TwoInts melding med 2 verdier som er hhv thrust(1000(min) - 2000(max)) og arm(0 = av, 1 = på).

## For å teste koden kjør følgende kommandoer:
Terminal 1
```sh
roscore
```

Terminal 2
```sh
rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
```
USB# er nummeret til porten som stm32 er koblet til pc-en via, så sjekk hvilken port det er og endre # til det nummeret; F.eks. 1


Terminal 3
```sh
rosrun fifth_prop prop.py
```
