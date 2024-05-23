//serial_port.cpp
//这是接收角度，并且下发给下位机的demo3
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

    //创建一个serial类
    serial::Serial sp;

/**************************************************************************
函数功能：将数据经由串口发送出去
入口参数：[serial::Serial &ser]：              串口类名称；
                      [std::string &serial_msg]:      发送的数据数组；
**************************************************************************/
int serial_write(serial::Serial &ser, std::string &serial_msg)
{
    ser.write(serial_msg);
    return 0;
}

/**************************************************************************
函数功能：将数据经由串口发送出去
入口参数：[serial::Serial &ser]：              串口类名称；
                      [std::string &serial_msg]:      接收的数据数组；
**************************************************************************/
int serial_read(serial::Serial &ser, std::string &result)
{
    result = ser.read( ser.available() );
    return 0;
}

/////////////////////// 定义一个数组，存储6个关节角
    double joint_angles[6] = {70, 115, 65, 90, 90, 90};
    bool check = false ;
/////////////////////////    
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
      check = true ;
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
     
    //创建一个serial类
    //serial::Serial sp;
    
    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyACM0");
    //设置串口通信的波特率
    sp.setBaudrate(9600);    
    //sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
 
    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyACM0  is opened!!!");
    }
    else
    {
        return -1;
    }
    
    ros::Rate loop_rate(500);
    
    //ros::Rate loop_rate(50);
    
    std::string data, result;
   
    cout << "---------------Arm is working now!-----------------:" << endl;
   
   while(ros::ok())
   {
        ros::spinOnce();
        if(check)
        {
         //清空data变量
         data.clear();
         // 把关节角的数组转换为一个字符串
         for (int i = 0; i < 6; i++) 
           {
             data += std::to_string (joint_angles [i]);
             data += ",";
            }   

        //串口写数据
        serial_write(sp, data);
        cout << " the data write to serial is :  " << data.c_str();
         
        //等待1秒
        sleep(2);    
            
        //串口读数据
        serial_read(sp, result);
        cout << " the data read from serial is : " << result.c_str();
        cout << endl;
        cout << "---------------Another loop!-----------------:" << endl;

        check = false;
        }      
   }         
    //关闭串口
    //sp.close();

    return 0;
}
