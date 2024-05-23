//serial_port.cpp
#include<ros/ros.h>
#include<serial/serial.h>
#include<std_msgs/String.h>

#include<iostream>
#include<string>
#include<sstream>

using namespace std;


/**************************************************************************
函数功能：将数据经由串口发送出去
入口参数：[serial::Serial &ser]：              串口类名称；
                      [std::string &serial_msg]:      发送的数据数组；
返  回  值：无
说        明：无
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
返  回  值：无
说        明：无
**************************************************************************/
int serial_read(serial::Serial &ser, std::string &result)
{
    result = ser.read( ser.available() );
    return 0;
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;
    
    //创建一个serial类
    serial::Serial sp;
    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyACM1");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
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
        ROS_INFO_STREAM("/dev/ttyACM1 is opened!!!");
    }
    else
    {
        return -1;
    }
    
    ros::Rate loop_rate(500);
    
    //ros::Rate loop_rate(50);
	
    //data 为发送数据
    //result 为接收数据
    //
    
    std::string data, result;
    //std::vector<std::string> joint_angles = {"70", "115", "65", "90", "90", "75"}; 
    // 定义并初始化一个std::vector对象，名为joint_angles，存储6个关节角的字符串
    //joint_angles.push_back("1.7"); // 向joint_angles中添加一个新的元素，值为"1.7"

    
    int func(0);
    
    cout << "Please input function number:" << endl;

    
    while(ros::ok())
    {
      	//从键盘中读取键入数据
      	cout << "Your function number is: ";
        cin >> func;
        
  /*****************************************************************************
 * 以下逻辑可以按照你自己的写，主要工作是根据键盘键入的数据，为 data 赋值
 *****************************************************************************/
     	 switch (func)
        {
            case 0:     data = "R\r\n";           break;
            //case 0:     data = "A 800 456\r\n";           break;
            case 1:     data = "B 1200 456\r\n";        break;
            case 2:     data = "C 1600 456\r\n";        break;
            case 3:     data = "D 1800 456\r\n";        break;
            default:    ROS_ERROR_STREAM("No this function number!!!");     break;
        }

        //串口写数据
        serial_write(sp, data);
        cout << " the data write to serial is :  " << data.c_str();
        //串口读数据
        serial_read(sp, result);
        cout << " the data read from serial is : " << result.c_str();
        cout << endl;
    }
    //关闭串口
    sp.close();
 
    return 0;
}
