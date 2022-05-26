# To set up the load cell
Follow [this tutorial](https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide/all) from SparkFun to connect the load cell, HX711 amplifier, and Arduino board. Install the HX711 Arduino library and use the example codes to test the connection.

# To launch the node
```
$ rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=57600
```

Change the port and baud value according to your device.\
The `.ino` code will be running on the Arduino once you power it on. What the above code does is to add the Arduino into the ROS network.