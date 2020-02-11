# PropellBak_arduino
ROS node running on the STM32 to control the fifth prop on the drone for team 2020

Its transmitting and Recieving an ascend_msgs::TwoInts message with to ints, which is respectively thrust(1000(min) - 2000(max)) and arm(0 = off, 1 = on)

## To even compile the code, the following steps must be completed:
In the Arduino IDE you have to add the STM32 since its not amongst the default board that is included when you download the IDE. This is explained in the video below:

You need:

STM32(blue pill)

FTDI programmer

cabler


https://www.youtube.com/watch?v=MLEQk73zJoU

When this is done you have to include rosserial in the Arduino IDE as explained below

http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

Now the code should compile without error, and you can upload the code to the STM32

## To test the code run the following commands:
Terminal 1
```sh
roscore
```

Terminal 2
```sh
rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
```

USB# is the number of the USB gate the STM32 is connected to. Check which port this is and change # to the corresponding number i.e. 0


Terminal 3
```sh
rosrun fifth_prop prop.py
```

# PS

The output signal for the ESC is on pin PB6
