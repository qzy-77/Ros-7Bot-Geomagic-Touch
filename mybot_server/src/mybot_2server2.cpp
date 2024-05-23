//实现接受规划、发布real_joint_states、给下位机发送joint_states
# include <ros/ros.h>
# include <actionlib/server/simple_action_server.h>
# include <control_msgs/FollowJointTrajectoryAction.h>
# include <control_msgs/FollowJointTrajectoryActionGoal.h>
# include <control_msgs/FollowJointTrajectoryActionResult.h>
# include <sensor_msgs/JointState.h>
# include <std_msgs/Float64.h>
# include <std_msgs/Float32MultiArray.h>
# include <iostream>
# include <string.h>
# include <stdio.h>
# include <vector>

# include <moveit_msgs/RobotTrajectory.h>

#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <math.h> // 导入数学库，用于计算π

//action服务端的相关定义，请加入到驱动节点的头文件中
#include "actionlib/server/action_server.h"    
//action服务端的目标控制句柄定义，与接收的目标相关联后，可以用来实现action的信息反馈等操作       
#include "actionlib/server/server_goal_handle.h" 
 
using namespace std;
 
class FollowJointTrajectoryAction
{
public:
 
   FollowJointTrajectoryAction(std::string name) :
   as_(nh_, name, false),
   action_name_(name)
 {
    as_.registerGoalCallback(boost::bind(&FollowJointTrajectoryAction::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&FollowJointTrajectoryAction::preemptCB, this));
    as_.start();
    Pub_joint = nh_.advertise<sensor_msgs::JointState>("/move_group/real_joint_states",10);
        pub_joint1 = nh_.advertise<std_msgs::Float64>("joint1_value", 100);
        pub_joint2 = nh_.advertise<std_msgs::Float64>("joint2_value", 100);
        pub_joint3 = nh_.advertise<std_msgs::Float64>("joint3_value", 100);
        pub_joint4 = nh_.advertise<std_msgs::Float64>("joint4_value", 100);
        pub_joint5 = nh_.advertise<std_msgs::Float64>("joint5_value", 100);
        pub_joint6 = nh_.advertise<std_msgs::Float64>("joint6_value", 100);
        
    // ros::Rate loop_rate(10);    
        
    js.name.resize(6);
    js.position.resize(6);
    //名字要与关节定义的名字一致
    js.name[0] = "joint1";
    js.name[1] = "joint2";
    js.name[2] = "joint3";
    js.name[3] = "joint4";
    js.name[4] = "joint5";
    js.name[5] = "joint6";
    ROS_INFO("---------------------action start!--------------------------");
 }
   
 ~FollowJointTrajectoryAction(void)
  {
  }
 
 void goalCB()
 {
    ROS_INFO("----------------------goal is receive!------------------------"); 
    
    // 用于存储 moveit 发送出来的轨迹数据
    std::vector<trajectory_msgs::JointTrajectoryPoint> points_;
    
    double points_end[6];
    double Pos_length;
    
    if(as_.isNewGoalAvailable())
     {
       js.position.clear();
       points_ = as_.acceptNewGoal()->trajectory.points;
       Pos_length = points_.size(); 
       
    for (int i = 0; i < 6; i++)
      {
        //假设v是一个vector对象,v.at(n)和v[n]是一样的
        //但是前者会检查是否越界，后者不会
        points_end[i] = points_.at(Pos_length - 1).positions[i];
        js.position.push_back(points_end[i]);
      }
       js.header.stamp = ros::Time::now();
       //向move_group节点发布规划得到的关节值    
       Pub_joint.publish(js);
       //向下位机arduino节点发布规划得到的关节值       
       joint1_pos.data = js.position[0] * 180 / M_PI;
       joint2_pos.data = js.position[1] * 180 / M_PI;
       joint3_pos.data = js.position[2] * 180 / M_PI;
       joint4_pos.data = js.position[3] * 180 / M_PI;
       joint5_pos.data = js.position[4] * 180 / M_PI;
       joint6_pos.data = js.position[5] * 180 / M_PI ;
       
       pub_joint1.publish(joint1_pos);
       pub_joint2.publish(joint2_pos);
       pub_joint3.publish(joint3_pos);
       pub_joint4.publish(joint4_pos);
       pub_joint5.publish(joint5_pos);
       pub_joint6.publish(joint6_pos);
       
      }
     else
      {
        ROS_INFO("--------------------------goal is not available!----------------------------"); 
      }
   control_msgs::FollowJointTrajectoryResult result;
   result.error_code = 0;
   as_.setSucceeded(result);
 } 
 
 void preemptCB()
 {
   ROS_INFO("%s: Preempted", action_name_.c_str());
   // set the action state to preempted
   as_.setPreempted();
 }
 
  protected: 
  sensor_msgs::JointState js;
  std_msgs::Float64 joint1_pos, joint2_pos, joint3_pos, joint4_pos, joint5_pos, joint6_pos;
  //moveit_msgs::RobotTrajectory moveit_tra;
  std::string action_name_; 
  ros::NodeHandle nh_;
  //给move_group识别的publisher，代替joint_state_publisher，发布joint_states
  ros::Publisher Pub_joint;
    ros::Publisher pub_joint1;//给下位机arduino识别的publiser
    ros::Publisher pub_joint2;//同上
    ros::Publisher pub_joint3;//同上
    ros::Publisher pub_joint4;//同上
    ros::Publisher pub_joint5;//同上
    ros::Publisher pub_joint6;//同上
    
  //与moveit中action client通讯的action server
  actionlib::SimpleActionServer<control_msgs::FollowJointTrajectoryAction> as_;
   
  //to the client
  control_msgs::FollowJointTrajectoryResult result_;
 
  //receive
  control_msgs::FollowJointTrajectoryGoal goal_;
};
 
int main(int argc, char *argv[])
 {
   ros::init(argc, argv, "moveit_action_server");
   // 创建 action 话题名称
   FollowJointTrajectoryAction followJointTrajectoryAction("arm_group_controller/follow_joint_trajectory");  
   ros::spin();
   return 0;
}
