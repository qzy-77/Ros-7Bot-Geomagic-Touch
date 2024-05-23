/**
 * 该例程将订阅/phantom/button话题，自定义消息类型mybot_touch::OmniButtonEvent
 * 该例程将订阅/phantom/pose话题，自定义消息类型mybot_touch::OmniState
 */
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Wrench.h>
#include <geometry_msgs/WrenchStamped.h>
#include <sensor_msgs/JointState.h>

#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/String.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float64.h>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <sstream>

#include "mybot_touch/OmniButtonEvent.h"
#include "mybot_touch/OmniFeedback.h"
#include "mybot_touch/OmniState.h"

using namespace std;
     
     struct Point3D
       {
	  double x;
	  double y;
	  double z;
	};
     // 定义一个全局变量，用来储存上一秒的数据
     Point3D last_position;
     // 定义一个全局变量，用来储存上一次接收到的消息的时间戳
     ros::Time last_time;	
     
     int grey_button;
     int white_button;
     bool check = false;
     
    // 定义一个函数，用来计算两个Point3D对象之间的欧氏距离
    double distance(Point3D p1, Point3D p2)
    {
       return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    } 
    
    void callback1(const mybot_touch::OmniButtonEvent::ConstPtr& button_msg)
     {
       grey_button =  button_msg->grey_button;
       white_button =  button_msg->white_button;  
       
       if(grey_button == 1 || white_button == 1)  
       { 
         ROS_INFO("I hear grey_button is%d, white_button is%d ", grey_button, white_button);
         check = true ;
       }
     }
     
    void callback2(const geometry_msgs::PoseStamped state_msg)
     {
        // 获取当前消息的时间戳
        ros::Time current_time = state_msg.header.stamp;
        // 计算与上一次接收到的消息的时间差
        ros::Duration time_diff = current_time - last_time;
        
       if(time_diff.toSec() >= 0.5)
       {
        // 创建一个Point3D类型的变量，用来储存pose中的position信息
    	Point3D current_position;
        // 将state_msg中的position信息赋值给position变量
        current_position.x = state_msg.pose.position.x;
        current_position.y = state_msg.pose.position.y;
        current_position.z = state_msg.pose.position.z;

        // 计算与上一秒数据的欧氏距离
        double dist = distance(last_position, current_position);
        //ROS_INFO("The distance between last and current data is %f", dist);
        // 将当前数据赋值给上一秒数据
        last_position = current_position;
        // 等待一秒
        //rate.sleep();
        if(dist > 0.02)
        {
          // 打印current_position信息           
          ROS_INFO("I hear x_position = %f, y_position = %f , z_position = %f", current_position.x, current_position.y, current_position.z);
          check = true ; 
        }
        
        // 将当前时间赋值给上一次时间
        last_time = current_time;
        
       }
     }

int main(int argc, char** argv)
{
    ros::init(argc, argv, "mybot_touch");
    //创建句柄
    ros::NodeHandle n;
    
    ros::Subscriber sub1 = n.subscribe<mybot_touch::OmniButtonEvent>("/phantom/phantom/button", 100, callback1);
    ros::Subscriber sub2 = n.subscribe<geometry_msgs::PoseStamped>("/phantom/phantom/pose", 1000, callback2);
    
    cout << "---------------Listenning to touch now!-----------------:" << endl;
       while(ros::ok())
   {
        ros::spinOnce();
        if(check)
        {
        
        cout << "---------------Another loop!-----------------:" << endl;
        check = false;
        
        }      
   } 
 
    //sp.close();
    return 0;
}
