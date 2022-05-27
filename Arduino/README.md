## To set up the load cell
- Follow [this tutorial](https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide/all) to connect the load cell, HX711 amplifier, and Arduino board.
- Install `HX711 Arduino library` and use the example codes to test the readings.
- Install `rosserial` library and add/edit ROS-related part of the code.

## To launch the node
```
$ rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
```

- Change the port and baud value according to your device.
- The `.ino` code will be running on the Arduino once you power it on. What the above command does is to add the Arduino into the ROS network.