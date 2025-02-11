# RTKSERVER #


一个变形监测系统后端软件，基于RTKlib开发，实现GNSS数据流接收和解算、并将解算结果以指定形式存放至数据库。
整个监测系统由后端、数据库和前端组成。


本程序利用Qt编写、调试和生成可执行程序，为满足课题实际需要，可在Windows和Linux下运行


- 在Windows下利用 `Qt 6.2.1 MinGW 64-bit` 版本编译器构建


- 在linux下利用 `Qt 6.1.2 GCC 64-bit` 版本编译器构建


- 工程的构建参照了[emlid/RTKLIB](https://github.com/emlid/RTKLIB/tree/reach)的工程构建方法，并将rtklib核心代码部分替换为[RTKLIB 2.4.3 b34](https://github.com/tomojitakasu/RTKLIB/tree/rtklib_2.4.3) 版本



based on RTKlib 2.4.3 b34 , use Qt 6.2.1 MinGW 64-bit in Windows and Qt 6.1.2 GCC 64bit in Linux.


----------

# 已实现功能 #


##  1. RTKSERVER ##
本项目的主要工作，逐步实现各个功能并完善，基于程序rtknavi和rtkrcv改写，主要实现接收GNSS实时流并实时解算，将结果和一些其他指定信息写入数据库，为监测系统前端展示提供数据支持。


### 功能实现 ###
- 单个线程的RTK解算，并输出结果到数据库


### 存在问题 ###


- 针对长时间数据断开会导致无法正常写入数据，在输出到数据库前增加了连接验证（问题未解决）


## 2.server_mult  ##

### 功能实现 ###

- 相较于RTKSERVER，重构了类内结构，简化了实现流程。现在可以通过读取极为简单的函数来实现一个rtksvrthread的创建。
- 完善了monitor功能，现在可以正常的通过tcp流接收实时的解算结果（如使用rtkplot来进行实时监控线程的解算状态）


### 存在问题 ###

- 早期版本，仅仅实现了多线程RTK的技术验证，距离替换RTKSERVER版本还有很多工作要做


## 3. str2str_mult ##
一个CUI数据记录软件，基于str2str改写，相较于RTKLIB提供的CUI程序str2str，可以实现多个数据流的同步记录。

### 功能实现 ###
- 多个数据流同时记录并存储数据，理论上str2str能做的都能做，并且一个程序能执行多个线程


### 存在问题 ###

- 程序实现过于简单，不能针对某一流进行操作
- 存放文件的时候不能根据需求自动创建文件夹，只能在已有的文件夹下建立新文件

----------


# 下一步开发计划 #


## 1.rtk_analysis  ##
- 数据分析软件


## 2.rtk_tools  ##

- GNSS数据处理小工具

## 3.server_GUI  ##

- SERVER程序的图形化界面接口

# 自建函数库 #

## koro.h  ##

用于封装一些函数，实现一些开发过程中需要用到的功能

### 已添加功能 ###


- enu2pose


	利用当前坐标系下任意三个点构成一个平面，输入三个点的坐标，输出该平面相对于当前坐标系下的姿态信息