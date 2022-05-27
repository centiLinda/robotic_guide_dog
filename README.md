# Robotic Guide Dog
This repo is for my final year project titled "Development of A Robotic Guide Dog". It's a voice-controlled robot dog with various guidance functions. The user experience is to imitate real human-guide dog interaction. [More info](https://centilinda.github.io/portfolio/fyp.html)

## Hardware
- The robot platform is [Unitree A1 Robot Dog](https://www.unitree.com/cn/products/a1).
- The main computing platform is Jetson Nano. You can find the complete hardware connection diagram in the main folder.

## Software
- The software platform is ROS. All the packages I used are well-maintained and open-source.
- `/Arduino` contains instructions to connect the circuit and the Arduino code.
- `/src` contains instructions to install and set up the ROS packages.
- Make sure you meet the prerequisites of each package, use `catkin tools` or `catkin_make` to build the packages, and source the necessary `.bash` files before you run the nodes.