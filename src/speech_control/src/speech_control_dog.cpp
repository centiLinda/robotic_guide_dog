// Subscribe to /pocketsphinx_recognizer/output
// Publish to /cmd_vel and /command_status

#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

std::string command;
int command_state = 1; // 1 for command sent, 0 for ready to send new command
                       // so that cmd_vel message is only sent once

void wordStringReceived(const std_msgs::String& word) {
  command = word.data;
  command_state = 0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "speech_control_dog");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/pocketsphinx_recognizer/output", 1000, &wordStringReceived);
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Publisher pub_status = nh.advertise<std_msgs::String>("/command_status", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
      geometry_msgs::Twist msg;
      std_msgs::String status;
      status.data = "default";

    if(!command_state){
      if (command == "pause "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          command_state = 1;
          pub_vel.publish(msg);
      }
      else if (command == "straighton "){
          msg.linear.x = 0.1;
          msg.angular.z = 0;
          command_state = 1;
          pub_vel.publish(msg);
      }
      else if (command == "left "){
          msg.linear.x = 0;
          msg.angular.z = 0.2;
          command_state = 1;
          pub_vel.publish(msg);
      }
      else if (command == "right "){
          msg.linear.x = 0;
          msg.angular.z = -0.2;
          command_state = 1;
          pub_vel.publish(msg);
      }

      else if (command == "find gate "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          status.data = "goal";
          command_state = 1;
          pub_vel.publish(msg);
      }
      pub_status.publish(status);
      ROS_INFO_STREAM("Status: " << status.data << "   New command: " << command);  
    }

    rate.sleep();
    ros::spinOnce();
  }

}
