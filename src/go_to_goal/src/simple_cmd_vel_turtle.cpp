// Subscribe to /darknet_ros/bounding_boxes
// Publish to /turtle1/cmd_vel

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <darknet_ros_msgs/BoundingBoxes.h>

int direction=1; //-1 for object on the right, 0 for on the left, 1 for at center

void objectPositionReceived(const darknet_ros_msgs::BoundingBoxes::ConstPtr& object) {
  if(object->bounding_boxes[0].Class == "suitcase"){
      ROS_INFO("Found suitcase");
      float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
      if(center < 275) direction = 0; //need to turn left
      else if(center > 430) direction = -1; //need to turn right
      else direction = 1;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "simple_cmd_vel_turtle");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/darknet_ros/bounding_boxes", 1000, &objectPositionReceived);
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
      geometry_msgs::Twist msg;

      switch(direction){
          case 0:
            msg.linear.x = 0;
            msg.angular.z = 1;
            break;
          case -1:
            msg.linear.x = 0;
            msg.angular.z = -1;
            break;
          case 1:
            msg.linear.x = 1;
            msg.angular.z = 0;
            break;
      }   

      pub.publish(msg);

      rate.sleep();
      ros::spinOnce();
  }

}
