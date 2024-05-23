# Ros-7Bot-Geomagic-Touch
实验系统环境为Ubuntu 20.04，使用ros中Movite作为主控，并利用Rviz来进行三维可视化仿真，其可以借助导入的URDF，文件来实现对模型的图形化显示。实现了通过Geomagic Touch 操控 7Bot 机械臂的毛笔书写

包含两大部分：
**Code：**
1、mybot_description：7Bot机械臂的描述；
2、mybot_moveit_config：movite在Ubantu20.04下的配置文件；
3、mybot_plannig：通过两种不同的设置方式（角度、位置）来进行运动规划；
4、mybot_serial_port：将ros规划的角度通过Serial传输到Arduino板；
5、mybot_serial_port：movite规划后需要包含的server，来订阅规划角度，以及更新tf；
6、mybot_toch：Geomagic Touch自带的ros通讯等；
**Paper：**
讲述了系统整体设计的过程。
