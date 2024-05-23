//serial_port.cpp
//这是作为测试接收driver数据的cpp
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/String.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int16MultiArray.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float64.h>

#include <iostream>
#include <string>

#include <cstring>

#include <sstream>

using namespace std;

     // 定义一个数组，存储6个关节角
    double joint_angles[6] = {70, 115, 65, 90, 90, 90};
    
    void callback1(const std_msgs::Float64& pos_msg)
     {
      joint_angles[0] =  pos_msg.data  ;//舵机1
      ROS_INFO("[%f] ", joint_angles[0]);
     }

    void callback2(const std_msgs::Float64& pos_msg)
     {
      joint_angles[1] =  pos_msg.data  ;//舵机2
      ROS_INFO("[%f] ", joint_angles[1]);
     }

    void callback3(const std_msgs::Float64& pos_msg)
     {
      joint_angles[2] =  pos_msg.data  ;//舵机3
      ROS_INFO("[%f] ", joint_angles[2]);
     }

    void callback4(const std_msgs::Float64& pos_msg)
     {
      joint_angles[3] =  pos_msg.data  ;//舵机4
      ROS_INFO("[%f] ", joint_angles[3]);
     }

    void callback5(const std_msgs::Float64& pos_msg)
     {
      joint_angles[4] =  pos_msg.data  ;//舵机5
      ROS_INFO("[%f] ", joint_angles[4]);
     }

    void callback6(const std_msgs::Float64& pos_msg)
     {
      joint_angles[5] =  pos_msg.data  ;//舵机6
      ROS_INFO("[%f] ", joint_angles[5]);
      ROS_INFO("-----------done successfully-----------");
     }


int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;
    
    ros::Subscriber sub1 = n.subscribe("joint1_value", 1000, callback1);
    ros::Subscriber sub2 = n.subscribe("joint2_value", 1000, callback2);
    ros::Subscriber sub3 = n.subscribe("joint3_value", 1000, callback3);
    ros::Subscriber sub4 = n.subscribe("joint4_value", 1000, callback4);
    ros::Subscriber sub5 = n.subscribe("joint5_value", 1000, callback5);
    ros::Subscriber sub6 = n.subscribe("joint6_value", 1000, callback6);   
    //sp.close();
    
    ros::spin();
    return 0;
}
