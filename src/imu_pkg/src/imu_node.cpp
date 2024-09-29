#include <ros/ros.h>
#include "sensor_msgs/Imu.h"
#include "tf/tf.h"
#include "geometry_msgs/Twist.h"

ros::Publisher vel_pub;


void imu_callback(sensor_msgs::Imu msg)
{
    if (msg.orientation_covariance[0] < 0)
    {
        return;
    }

    tf::Quaternion quaterion(msg.orientation.x,msg.orientation.y,msg.orientation.z,msg.orientation.w);
    double roll,pitch,yaw;
    tf::Matrix3x3(quaterion).getRPY(roll,pitch,yaw);
    roll = roll*180/M_PI;
    pitch = pitch*180/M_PI;
    yaw = yaw*180/M_PI;

    ROS_INFO("Roll: %f, Pitch: %f, Yaw: %f", roll, pitch, yaw);
    geometry_msgs::Twist vel_cmd;

    double target_yaw = 90;

    double error = target_yaw - yaw;

    vel_cmd.angular.z = error * 0.01;
    vel_cmd.linear.x = 0.1;

    vel_pub.publish(vel_cmd);


}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_node");
    ros::NodeHandle nh;

    ros::Subscriber imu_sub = nh.subscribe("/imu/data" , 10 , imu_callback);
    ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::spin();

    return 0;

}