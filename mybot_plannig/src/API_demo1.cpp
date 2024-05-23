#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
 
int main(int argc, char **argv)  //主函数
{
    //ros初始化节点，节点名为moveit_fk_demo
    ros::init(argc, argv, "moveit_fk_demo1");
    //多线程
    ros::AsyncSpinner spinner(1);
    //开启新的线程
    spinner.start();
 
    //初始化需要使用move group控制的机械臂中的arm group
    moveit::planning_interface::MoveGroupInterface arm("arm_group");
    //当运动规划失败后，允许重新规划
    arm.allowReplanning(true);
    //设置机械臂运动的允许误差
    arm.setGoalJointTolerance(0.01);
    
    //设置允许的最大速度和加速度
    //arm.setMaxAccelerationScalingFactor(0.8);
    //arm.setMaxVelocityScalingFactor(0.8);
 
    // 控制机械臂先回到初始化位置
    arm.setNamedTarget("1");
    arm.move(); //规划+运动
    sleep(6);
 
    //定义一个数组，存放6个关节的信息
    double targetPose[7] = {1.57, 1.57, -1.57, 1.11, 1.57, 1.57, 1.57};
    //关节的向量，赋值
    std::vector<double> joint_group_positions(7);
    joint_group_positions[0] = targetPose[0];
    joint_group_positions[1] = targetPose[1];
    joint_group_positions[2] = targetPose[2];
    joint_group_positions[3] = targetPose[3];
    joint_group_positions[4] = targetPose[4];
    joint_group_positions[5] = targetPose[5];
    joint_group_positions[6] = targetPose[6];
   
    //将关节值写入
    arm.setJointValueTarget(joint_group_positions);
    arm.move(); //规划+移动
    sleep(2);
 
    // 控制机械臂先回到初始化位置
    arm.setNamedTarget("1");
    arm.move();
    sleep(1);
    
    //关闭并退出
    ros::shutdown(); 
 
    return 0;
}
