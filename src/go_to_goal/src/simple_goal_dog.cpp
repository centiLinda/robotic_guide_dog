// Subscribe to /darknet_ros/bounding_boxes
// Publish to /cmd_vel

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <darknet_ros_msgs/BoundingBoxes.h>

int direction=1; //-1 for object on the right, 0 for on the left, 1 for at center

void objectPositionReceived(const darknet_ros_msgs::BoundingBoxes::ConstPtr& object) {
  if(object->bounding_boxes[0].Class == "suitcase"){
      //ROS_INFO("Found suitcase");
      float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
      if(center < 275) direction = 0; //need to turn left
      else if(center > 430) direction = -1; //need to turn right
      else{
          direction = 1;
          ROS_INFO("Found suitcase");
      }
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "simple_goal_dog");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/darknet_ros/bounding_boxes", 1000, &objectPositionReceived);
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Publisher pub_goal = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
      geometry_msgs::Twist vel;
      geometry_msgs::PoseStamped goal;

      switch(direction){
          /*
          case 0:
            vel.linear.x = 0;
            vel.angular.z = 0.15;
            break;
          case -1:
            vel.linear.x = 0;
            vel.angular.z = -0.15;
            break;
          */
          case 1:
            //vel.linear.x = 1.0;
            goal.pose.position.x = 2.0;
            goal.pose.orientation.w = 1.0;
            goal.header.frame_id = "map";
            break;
      }   

      //pub_vel.publish(vel);
      pub_goal.publish(goal);

      rate.sleep();
      ros::spinOnce();
  }

}
