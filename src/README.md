# ROS packages to install
1. [ZED ros wrapper](https://github.com/stereolabs/zed-ros-wrapper)

2. [rosserial](http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup)

3. [darknet_ros](https://github.com/leggedrobotics/darknet_ros)

4. [pocketsphinx](https://github.com/UTNuclearRoboticsPublic/pocketsphinx)

The navigation function comes with the A1 Robot Dog and is launched seperately on the onboard Raspberry Pi.\
`speech_control` package is to process the voice commands and send basic instructions. `go_to_goal` package is to process commands that involve object detection and navigation. These 2 could be done within 1 package, but this is just how I built it along the way. You can also see my progress from all the unused codes. I decided to just upload them all for fun.

# To launch the codes
```
$ roslaunch go_to_goal full_functions.launch
```
There are 4 arguments in the launch file:
- ssh: 1 if you ssh into the machine and don't want any window to pop-up (which may cause error). Default value is 0.
- force: 1 if you have connected the load cell. Default value is 0.
- test: 1 if you don't want to run the 2 main codes in `speech_control` and `go_to_goal`. Default value is 0.
- suitcase: 1 to detect the suitcase (a test object). 0 to detect counter and plant (actual objects). Default value is 1.

Add for example `ssh:=1` after the launch command to change the default value.