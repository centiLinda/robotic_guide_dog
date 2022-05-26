// Subscribe to /darknet_ros/bounding_boxes, /command_status, and /speed
// Publish to /cmd_vel and /move_base_simple/goal

/* TODO
1. Change default-speed to force-speed
2. Change width tolerance for both objects
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
#include <actionlib_msgs/GoalID.h>

int direction=-1; //-1 for NULL, 0 for on the left, 2 for on the right. 1 for in front
int move_state=0; //0 for in basic mode, already reached object, or already sent goal
                  //1 for available, 2 for reaching the closest point
std::string status;
float speed=0.1;

void objectPositionReceived(const darknet_ros_msgs::BoundingBoxes::ConstPtr& object) {  //output direction, move_state
    if(object->bounding_boxes[0].Class == "counter"){
        float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
        float width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;

        if((width>210) && (move_state == 1)) move_state = 2; //at closest point ~30cm
        //move_state will be 0 in the next loop
        //move_state reset in function statusReceived

        //center is at 365 if ZED is placed at center
        if(center < 330) direction = 0; //need to turn left
        else if(center > 400) direction = 2; //need to turn right
        else direction = 1;
    }
    else if(object->bounding_boxes[0].Class == "tree"){
        float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
        float width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;

        if((width>210) && (move_state == 1)) move_state = 2; //at closest point ~30cm
        //move_state will be 0 in the next loop
        //move_state reset in function statusReceived

        //center is at 365 if ZED is placed at center
        if(center < 330) direction = 0; //need to turn left
        else if(center > 400) direction = 2; //need to turn right
        else direction = 1;
    }
    else if(object->bounding_boxes[0].Class == "suitcase"){
        float center = (object->bounding_boxes[0].xmin + object->bounding_boxes[0].xmax)/2.0;
        float width = object->bounding_boxes[0].xmax - object->bounding_boxes[0].xmin;

        if((width>500) && (move_state == 1)) move_state = 2; //at closest point
        //move_state will be 0 in the next loop
        //move_state reset in function statusReceived

        //center is at 365 if ZED is placed at center
        // 602, 760
        if(center < 560) direction = 0; //need to turn left                        -----increase threshold if still got delay
        else if(center > 870) direction = 2; //need to turn right
        else direction = 1;
    }
    else direction = -1; //found nothing
}

void statusReceived(const std_msgs::String& status_Received){
    status = status_Received.data;
    if(status != "basic") move_state = 1; //reset when recevive new goal/navigate command
}

/*void speedReceived(const std_msgs::Float64& speed_Received){
  speed = speed_Received.data;
}*/

int main(int argc, char **argv) {
    ros::init(argc, argv, "send_goal_01");
    ros::NodeHandle nh;

    ros::Subscriber sub_yolo = nh.subscribe("/darknet_ros/bounding_boxes", 1000, &objectPositionReceived);
    ros::Subscriber sub_status = nh.subscribe("/command_status", 1000, &statusReceived);
    //ros::Subscriber sub_speed = nh.subscribe("/speed", 1000, &speedReceived);
    ros::Publisher pub_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    ros::Publisher pub_goal = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000);
    ros::Publisher pub_cancel = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 1000);
    tf::TransformListener tf_listener;

    ros::Rate rate(2);
    while(ros::ok()){
        geometry_msgs::Twist vel;
        geometry_msgs::PoseStamped goal;
        actionlib_msgs::GoalID cancel;

        if(move_state){
            if(status == "navigate"){
                tf::StampedTransform tf_transform;
                try{
                    tf_listener.lookupTransform("/odom", "/base_link", ros::Time(0), tf_transform);
                }
                catch(tf::TransformException &ex){
                    ROS_ERROR("%s", ex.what());
                    ros::Duration(1.0).sleep();
                    continue;
                }

                float distance = 1; // 1 meters in front
                float z = tf_transform.getRotation().z();
                float w = tf_transform.getRotation().w();
                float angle = atan2(2*w*z, (1-2*z*z));
                goal.pose.position.x = tf_transform.getOrigin().x() + distance * cos(angle);
                goal.pose.position.y = tf_transform.getOrigin().y() + distance * sin(angle);
                goal.pose.orientation.z = tf_transform.getRotation().z();
                goal.pose.orientation.w = tf_transform.getRotation().w();
                goal.header.frame_id = "map";
                pub_goal.publish(goal);
                move_state = 0;          
            }

            else if((status == "goal_counter") || (status == "goal_gate") || (status == "goal_suitcase")){
                switch(direction){
                    case -1: //NULL, turn CW to search
                        vel.linear.x = 0;
                        vel.angular.z = -0.1;
                        break;
                    case 0: //left
                        vel.linear.x = 0;
                        vel.angular.z = 0.1;
                        break;
                    case 2: //right
                        vel.linear.x = 0;
                        vel.angular.z = -0.1;
                        break;
                    case 1: //in front
                        vel.linear.x = speed;
                        vel.angular.z = 0;
                        break;   
                }

                if(move_state == 2){ //stop at closest point
                    vel.linear.x = 0;
                    vel.angular.z = 0;
                    move_state = 0;
                }
                pub_vel.publish(vel);
            }
        }

        if(status == "cancel"){
            pub_cancel.publish(cancel);
            ROS_INFO_STREAM("cancel goal");
        }

        ROS_INFO_STREAM("Status: " << status << "   Move status: " << move_state);
        //ROS_INFO_STREAM("Speed: " << speed);
        rate.sleep();
        ros::spinOnce();
    }

}
