// Subscribe to /pocketsphinx_recognizer/output
// Publish to /turtle1/cmd_vel

#include <string>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

std::string command;

void wordStringReceived(const std_msgs::String& word) {
  ROS_INFO_STREAM(word.data);
  command = word.data;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "speech_control_turtle");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/pocketsphinx_recognizer/output", 1000, &wordStringReceived);
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
      geometry_msgs::Twist msg;

      if (command == "stop "){
          msg.linear.x = 0;
          msg.angular.z = 0;
      }
      else if (command == "straighton "){
          msg.linear.x = 1;
          msg.angular.z = 0;
      }
      else if (command == "left "){
          msg.linear.x = 0;
          msg.angular.z = +1;
          ros::Time startTime = ros::Time::now();
          ros::Duration seconds = ros::Duration(1.5);
          ros::Time endTime = startTime + seconds;
          while(ros::Time::now() < endTime){
              pub.publish(msg);
              ros::Duration(0.1).sleep();
          }
          command = "straighton ";
      }
      else if (command == "right "){
          msg.linear.x = 0;
          msg.angular.z = -1;
          ros::Time startTime = ros::Time::now();
          ros::Duration seconds = ros::Duration(1.5);
          ros::Time endTime = startTime + seconds;
          while(ros::Time::now() < endTime){
              pub.publish(msg);
              ros::Duration(0.1).sleep();
          }
          command = "straighton ";
      }

      pub.publish(msg);

      rate.sleep();
      ros::spinOnce();
  }

}
