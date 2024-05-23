#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
该例程将订阅/phantom/button话题，自定义消息类型mybot_touch::OmniButtonEvent
该例程将订阅/phantom/pose话题，自定义消息类型mybot_touch::OmniState
"""

import rospy, sys
import moveit_commander
from moveit_msgs.msg import RobotTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint
from geometry_msgs.msg import PoseStamped, Pose
from geometry_msgs.msg import Wrench
from geometry_msgs.msg import WrenchStamped
from sensor_msgs.msg import JointState
from tf.transformations import euler_from_quaternion, quaternion_from_euler

from std_msgs.msg import MultiArrayLayout
from std_msgs.msg import String
from std_msgs.msg import MultiArrayDimension
from std_msgs.msg import Int16MultiArray
from std_msgs.msg import Float64

import math
import numpy as np

from mybot_touch.msg import OmniButtonEvent
from mybot_touch.msg import OmniFeedback
from mybot_touch.msg import OmniState

# 定义一个类，用来储存三维坐标点的信息
class Point3D(object):
    def __init__(self, x=0.0, y=0.0, z=0.0):
        self.x = x
        self.y = y
        self.z = z

# 定义一个全局变量，用来储存上一秒的数据
last_position = Point3D()
# 定义一个全局变量，用来储存上一次接收到的消息的时间戳
last_time = rospy.Time()

grey_button = 0
white_button = 0
check = False

# 定义一个函数，用来计算两个Point3D对象之间的欧氏距离
def distance(p1, p2):
    return math.sqrt((p1.x - p2.x)**2 + (p1.y - p2.y)**2 + (p1.z - p2.z)**2)

def callback1(button_msg):
    global grey_button, white_button, check
    
    grey_button = button_msg.grey_button
    white_button = button_msg.white_button
    
    if grey_button == 1 :
        rospy.loginfo("I hear grey_button is %d，I need to go home now!", grey_button)
        arm.set_named_target('home')
        arm.go()
        check = True

def callback2(state_msg):
    global last_position, last_time, check
    
    # 获取当前消息的时间戳
    current_time = state_msg.header.stamp
    # 计算与上一次接收到的消息的时间差
    time_diff = current_time - last_time
    
    if time_diff.to_sec() >= 0.5:
        # 创建一个Point3D类型的变量，用来储存pose中的position信息
        current_position = Point3D()
        # 将state_msg中的position信息赋值给position变量
        current_position.x = state_msg.pose.position.x
        current_position.y = state_msg.pose.position.y
        current_position.z = state_msg.pose.position.z
        
        # 计算与上一秒数据的欧氏距离
        dist = distance(last_position, current_position)
        #rospy.loginfo("The distance between last and current data is %f", dist)

        
        if dist > 0.01:
            # 打印current_position信息           
            #rospy.loginfo("I hear x_position = %f, y_position = %f , z_position = %f", current_position.x, current_position.y, current_position.z)
            move_x = current_position.x - last_position.x
            move_y = current_position.y - last_position.y
            move_z = current_position.z - last_position.z
            check_x = abs(move_x)
            check_y = abs(move_y)
            check_z = abs(move_z)
            #rospy.loginfo("The arm need to move x = %f, y = %f ", move_x, move_y)
            if check_x > check_z and check_x > check_y:
            	rospy.loginfo("The arm need to move x = %f", move_x)
            	arm.shift_pose_target(1, move_x, end_effector_link)
            	arm.go()
            	check = True
            if check_y > check_z and check_y > check_x:
            	rospy.loginfo("The arm need to move y = %f ", -move_y*1.2)
            	arm.shift_pose_target(0, -move_y*1.2, end_effector_link)
            	arm.go()  
            	check = True
            if check_z > check_x and check_z > check_y:
            	rospy.loginfo("The arm need to move z = %f ", move_z*1.2)
            	arm.shift_pose_target(2, move_z*1.2, end_effector_link)
            	arm.go()  
            	check = True            	
            
        # 将当前时间赋值给上一次时间
        last_time = current_time
        # 将当前数据赋值给上一秒数据
        last_position = current_position        

def main():
    while not rospy.is_shutdown():
        rospy.spin()
        if check:
            print("---------------Another loop!-----------------:")
            check = False

    # 关闭并退出moveit
    moveit_commander.roscpp_shutdown()
    moveit_commander.os._exit(0)        
            
if __name__ == '__main__':
   # 初始化move_group的API
    moveit_commander.roscpp_initialize(sys.argv)

    # 初始化ROS节点
    rospy.init_node('mybot_touch')

    # 初始化需要使用move group控制的机械臂中的arm group
    arm = moveit_commander.MoveGroupCommander('arm_group')

    # 获取终端link的名称
    end_effector_link = arm.get_end_effector_link()
                        
    # 设置目标位置所使用的参考坐标系
    reference_frame = 'world'
    arm.set_pose_reference_frame(reference_frame)
                
    # 当运动规划失败后，允许重新规划
    arm.allow_replanning(True)
        
    # 设置位置(单位：米)和姿态（单位：弧度）的允许误差
    arm.set_goal_position_tolerance(0.008)
    arm.set_goal_orientation_tolerance(0.08)

    # 设置允许的最大速度和加速度
    arm.set_max_acceleration_scaling_factor(0.5)
    arm.set_max_velocity_scaling_factor(0.5)
        
    # 控制机械臂先回到初始化位置
    arm.set_named_target('home')
    arm.go()
    rospy.sleep(2)

    sub1 = rospy.Subscriber("/phantom/phantom/button", OmniButtonEvent, callback1)
    sub2 = rospy.Subscriber("/phantom/phantom/pose", PoseStamped, callback2)
    
    print("---------------Listenning to touch now!-----------------:")
    main()

