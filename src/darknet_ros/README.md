# How to set up darknet_ros package
1. Install `darknet_ros` package and put the following files into repective folders. Use your own files if you want to detect different objects.
    * yolov3-tiny-XXX.weights\
    *src/darknet_ros/darknet_ros/yolo_network_config/weights*

    * yolov3-tiny-XXX.cfg\
    *src/darknet_ros/darknet_ros/yolo_network_config/cfg*

    * yolov3-tiny-XXX.yaml\
    *src/darknet_ros/darknet_ros/config*

    * yolo_v3_tiny_XXX.launch\
    *src/darknet_ros/darknet_ros/launch*

2. Launch both ZED and darknet nodes.
```
$ roslaunch zed_wrapper zed.launch
$ roslaunch darknet_ros XXX.launch
```
I used zed_no_tf.launch in my own launch file, because the clock in ZED TF somehow didn't match with the rest of the packages. Potentially because I updated the system clock one time.