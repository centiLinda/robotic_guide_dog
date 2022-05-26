// Subscribe to /pocketsphinx_recognizer/output and /speed
// Publish to /cmd_vel and /command_status
// Commands: pause, straight-on, left, right, find counter, find gate, find suitcase, navigate, cancel

#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>

std::string command;
int command_state = 1; // 1 for command sent, 0 for ready to send new command
                       // so that cmd_vel message is only sent once
                       // forward speed need to update continuously
float speed=0.1;

void wordStringReceived(const std_msgs::String& word) {
  command = word.data;
  command_state = 0;
}

void speedReceived(const std_msgs::Float64& speed_Received){
  speed = speed_Received.data;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "send_command_01");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/pocketsphinx_recognizer/output", 1000, &wordStringReceived);
  ros::Subscriber sub_speed = nh.subscribe("/speed", 1000, &speedReceived);
  ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Publisher pub_status = nh.advertise<std_msgs::String>("/command_status", 1000);

  ros::Rate rate(2);
  while(ros::ok()){
      geometry_msgs::Twist msg;
      std_msgs::String status;
      status.data = "basic";

    if(!command_state){
      if (command == "cancel "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          command_state = 1;
          status.data = "cancel";
      }
      else if (command == "straighton "){
          msg.linear.x = speed;
          msg.angular.z = 0;
          //command_state = 1;  //to keep speed updated
      }
      /*else if (command == "left "){
          msg.linear.x = 0;
          msg.angular.z = 0.2;
          command_state = 1;
      }
      else if (command == "right "){
          msg.linear.x = 0;
          msg.angular.z = -0.2;
          command_state = 1;
      }*/
      else if (command == "left "){
          msg.linear.x = 0;
          msg.angular.z = 0.2;
          ros::Time startTime = ros::Time::now();
          ros::Duration seconds = ros::Duration(2.4);
          ros::Time endTime = startTime + seconds;
          while(ros::Time::now() < endTime){
              pub_vel.publish(msg);
              ros::Duration(0.1).sleep();
          }
          command = "straighton ";
      }
      else if (command == "right "){
          msg.linear.x = 0;
          msg.angular.z = -0.2;
          ros::Time startTime = ros::Time::now();
          ros::Duration seconds = ros::Duration(2.4);
          ros::Time endTime = startTime + seconds;
          while(ros::Time::now() < endTime){
              pub_vel.publish(msg);
              ros::Duration(0.1).sleep();
          }
          command = "straighton ";
      }

      else if (command == "pause "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          command_state = 1;
      }

      else if (command == "find gate "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          status.data = "goal_gate";
          command_state = 1;
      }
      else if (command == "find counter "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          status.data = "goal_counter";
          command_state = 1;
      }
      else if (command == "find suitcase "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          status.data = "goal_suitcase";
          command_state = 1;
      }
      else if (command == "navigate "){
          msg.linear.x = 0;
          msg.angular.z = 0;
          status.data = "navigate";
          command_state = 1;
      }

      pub_vel.publish(msg);
      pub_status.publish(status);
      ROS_INFO_STREAM("Status: " << status.data << "   New command: " << command);        
    }
    ROS_INFO_STREAM("Speed: " << speed);
    rate.sleep();
    ros::spinOnce();
  }

}
