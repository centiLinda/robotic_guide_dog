#include <ros.h>
#include "HX711.h"
#include <std_msgs/Float64.h>

#define calibration_factor -7050.0
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

HX711 scale;

ros::NodeHandle nh;
std_msgs::Float64 msg;
ros::Publisher pub("/force_reading", &msg);

void setup() {
    //Serial.begin(9600);
    //Serial.println("Reading from load cell...");

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
    scale.tare();	//Assume no weight at start up, reset the scale to 0

    nh.initNode();
    nh.advertise(pub);
}

void loop() {
    //erial.print(scale.get_units(), 1); //scale.get_units() returns a float
    //Serial.println();

    msg.data = scale.get_units();
    pub.publish(&msg);
    nh.spinOnce();

    delay(100);
}
