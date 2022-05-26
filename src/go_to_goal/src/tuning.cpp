#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <string>
#include <math.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <actionlib_msgs/GoalID.h>


float width, center;

void objectPositionReceived(const darknet_ros_msgs::BoundingBoxes::ConstPtr& object) {
  if((object->bounding_boxes[0].Class == "suitcase") || (object->bounding_boxes[0].Class == "counter")){
    center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
    width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;
    //distance = 2050.5 * pow(width, -0.687);
  }

  if(object->bounding_boxes[0].Class == "plant"){
    if(object->bounding_boxes[1].Class == "plant"){ //see 2 plants, bring to center (gate)
      float center1 = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
      float center2 = (object->bounding_boxes[1].xmin + object->bounding_boxes[1].xmax)/2.0;
      center = (center1 + center2)/2.0;
      width = abs(center1 - center2);
    }
    else{ //see 1 plant, bring to plant
      center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
      width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;
      //distance = 2050.5 * pow(width, -0.687);
    }
  }
}
/*
float speed=0.1;

void forceReceived(const std_msgs::Float64& force_Received){
  float force = force_Received.data;

  if((force > 120) && (speed > 0.05)) speed = speed - 0.01;       //too fast
  else if((force < 50) && (speed <= 0.4)) speed = speed + 0.01; //too slow
}
*/
/*std::string command;
void wordStringReceived(const std_msgs::String& word) {
  command = word.data;
}*/
//float distance;


int main(int argc, char **argv) {
  ros::init(argc, argv, "tuning");
  ros::NodeHandle nh;
  ros::Subscriber sub_yolo = nh.subscribe("/darknet_ros/bounding_boxes", 1000, &objectPositionReceived);
  //tf::TransformListener tf_listener;
  //ros::Subscriber sub_force = nh.subscribe("/force_reading", 1000, &forceReceived);
  //ros::Subscriber sub = nh.subscribe("/pocketsphinx_recognizer/output", 1000, &wordStringReceived);
  //ros::Publisher pub = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
    /*
    tf::StampedTransform tf_transform;
    try{
      tf_listener.lookupTransform("/base_link", "/odom", ros::Time(0), tf_transform);
    }
    catch(tf::TransformException &ex){
      ROS_ERROR("%s", ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    geometry_msgs::PoseStamped goal;
    goal.pose.position.x = tf_transform.getOrigin().x();
    goal.pose.position.y = tf_transform.getOrigin().y();
    goal.pose.orientation.z = tf_transform.getRotation().z();
    goal.pose.orientation.w = tf_transform.getRotation().w();
    goal.header.frame_id = "map";*/

    /*actionlib_msgs::GoalID cancel;
    if(command == "pause "){
      pub.publish(cancel);
      ROS_INFO_STREAM("cancel goal");
    }*/
    //nh.param("distance", distance, float(1));
    //ROS_INFO_STREAM(distance);

    //ROS_INFO_STREAM("Center: " << center << "  Width: " << width << "  Distance: " << distance);
    ROS_INFO_STREAM("Center: " << center << "   Width: " << width);
    //ROS_INFO_STREAM(goal);
    //ROS_INFO_STREAM(speed);
    rate.sleep();
    ros::spinOnce();
  }

}
