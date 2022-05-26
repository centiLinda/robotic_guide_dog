// Subscribe to /darknet_ros/bounding_boxes, /command_status, and /force_reading
// Publish to /cmd_vel and /move_base_simple/goal

/* TODO
1. Add f(object, width) -> distance
*/

#include <ros/ros.h>
#include <string>
#include <math.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <tf/transform_listener.h>

int direction=-1; //-1 for NULL, 0 for on the left, 2 for on the right. 1 for in front
int forward_state=0; //1 if already on the way to object, 2 if already in front of object
std::string status;
float width, distance; 
float speed=0.1;

void objectPositionReceived(const darknet_ros_msgs::BoundingBoxes::ConstPtr& object) {  //output direction, distance, forward_state
  if(object->bounding_boxes[0].Class == "suitcase"){
      float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
      width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;

      if(width>210) forward_state = 2; //at closest point ~30cm
      else if((forward_state == 2) && (status == "default")) forward_state = 0; //reset forward_state when moved away

      distance = 2050.5 * pow(width, -0.687);

      //center is at 365 if ZED is placed at center
      if(center < 330) direction = 0; //need to turn left
      else if(center > 400) direction = 2; //need to turn right
      else direction = 1;

  }
  else direction = -1;
}

void statusReceived(const std_msgs::String& status_Received){
  status = status_Received.data;
}
/*
void forceReceived(const std_msgs::Float64& force_Received){
  float force = force_Received.data;

  if((force > 200) && (speed >= 0)) speed = speed - 0.02;       //too fast
  else if((force < 50) && (speed <= 0.3)) speed = speed + 0.02; //too slow
}*/

int main(int argc, char **argv) {
  ros::init(argc, argv, "send_goal_dog");
  ros::NodeHandle nh;

  ros::Subscriber sub_yolo = nh.subscribe("/darknet_ros/bounding_boxes", 1000, &objectPositionReceived);
  ros::Subscriber sub_status = nh.subscribe("/command_status", 1000, &statusReceived);
  //ros::Subscriber sub_force = nh.subscribe("/force_reading", 1000, &forceReceived);
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Publisher pub_goal = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
  tf::TransformListener tf_listener;

  ros::Rate rate(2);
  while(ros::ok()){
    geometry_msgs::Twist vel;
    geometry_msgs::PoseStamped goal;

    if((!forward_state) && (status == "goal")){

      tf::StampedTransform tf_transform;
      try{
        tf_listener.lookupTransform("/odom", "/base_link", ros::Time(0), tf_transform);
      }
      catch(tf::TransformException &ex){
        ROS_ERROR("%s", ex.what());
        ros::Duration(1.0).sleep();
        continue;
      }

      switch(direction){
          case -1: //NULL, turn CW to search
            vel.linear.x = 0;
            vel.angular.z = -0.15;
            pub_vel.publish(vel);
            break;
          case 0: //left
            vel.linear.x = 0;
            vel.angular.z = 0.15;
            pub_vel.publish(vel);
            break;
          case 2: //right
            vel.linear.x = 0;
            vel.angular.z = -0.15;
            pub_vel.publish(vel);
            break;

          case 1: //in front
            forward_state=1;
            vel.linear.x = 0;
            vel.angular.z = 0;
            //goal.pose.position.x = distance;
            //goal.pose.position.x = tf_transform.getOrigin().x() + 1; //forward 1m
            //goal.pose.position.y = tf_transform.getOrigin().y();
            
            float x = tf_transform.getOrigin().x();
            float y = tf_transform.getOrigin().y();
            float a = x / pow((pow(x, 2) + pow(y, 2)), 0.5);
            goal.pose.position.x = x + copysignf(a, x);
            goal.pose.position.y = y + copysignf(a*y/x, y);
            

            goal.pose.orientation.z = tf_transform.getRotation().z();
            goal.pose.orientation.w = tf_transform.getRotation().w();
            goal.header.frame_id = "map";
            pub_vel.publish(vel);
            pub_goal.publish(goal);          
            break;   
      }
    }
    ROS_INFO_STREAM("Status: " << status << "   Forward status: " << forward_state); //distance, speed
    rate.sleep();
    ros::spinOnce();
  }

}
