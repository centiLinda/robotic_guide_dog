// Subscribe to /force_reading
// Publish to /speed

#include <ros/ros.h>
#include <std_msgs/Float64.h>

float speed=0.1; //speed will be between 0 and 0.4

void forceReceived(const std_msgs::Float64& force_Received){
  float force = force_Received.data;

  if((force > 80) && (speed > 0.01)) speed = speed - 0.01;       //too fast
  else if((force < 10) && (speed < 0.4)) speed = speed + 0.01; //too slow
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "speed_tuning");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/force_reading", 1000, &forceReceived);
    ros::Publisher pub = nh.advertise<std_msgs::Float64>("/speed", 1000);

    ros::Rate rate(1);
    while(ros::ok()){
        std_msgs::Float64 Speed;
        Speed.data = (int)((speed*1000+5)/10)/100.0;

        pub.publish(Speed);

        ROS_INFO_STREAM(speed);
        rate.sleep();
        ros::spinOnce();
    }

}
