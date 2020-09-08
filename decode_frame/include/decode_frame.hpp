/*
编写者：lisn3188
网址：www.chiplab7.com
作者E-mail：lisn3188@163.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2012-05-25
测试： 本程序已在第七实验室的mini IMU上完成测试
功能：
使用串行中断程序接收mini IMU 的数据。
移植者需在自己编写对应的串口初始化程序，并开启接收中断。
在中断子程序中 需要做相应的改动

为了使用这个程序，你需要：
1.初始化你的串口，将波特率与模块的波特率设置成一致的
2.开启串口的接收中断，并参考以下程序编写中断子程序
  参考 void USART2_IRQHandler(void)
3.在主程序的循环体中，定时调用子程序：
  void UART2_CommandRoute(void)
  以检查是否收到新的数据帧
  所有的姿态数据和ADC值都会在 UART2_CommandRoute
  中进行更新。
4.使用 以下值会自动更新：

float 	yaw,  //偏航角
		pitch,//俯仰
		roll, //滚转
		alt,  //高度
		tempr,//温度
		press;//气压
int16_t ax, ay, az;//加速度计
int16_t gx, gy, gz;//陀螺仪
int16_t hx, hy, hz;//磁力计
------------------------------------
*/
#include <stdint.h> //int8_t int16_t int32_t 这些数据类型未定义，需要加入#include <stdint.h>
#include <ros/ros.h>
#include <serial/serial.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/String.h>
#include <iostream>
#include <cstring>




struct SAngle
{
	short Angle[3];
	short T;
};

class DecodeFrame
{
	public:
 		float 	yaw,  //偏航角
				pitch,//俯仰
				roll, //滚转
				alt,  //高度
				tempr,//温度
				press;//气压

		// 三轴角度
		struct SAngle stcAngle;
		char *strHex;
		int Range_t[4];
		int Range_deca[4];
		int32_t buf[200]; 
		int A0,A1,A2,A3;
		int Tag = 0;
		//ADC值
		int16_t ax, ay, az;//加速度计
		int16_t gx, gy, gz;//陀螺仪
		int16_t hx, hy, hz;//磁力计

		//GPS位置信息
		float 	GPS_Altitude , //天线离海平面的高度，-9999.9到9999.9米
				Latitude_GPS , //纬度	 单位为度
				Longitude_GPS , //经度  单位为度
				Speed_GPS , //地面速率  单位  米每秒
				Course_GPS ; //地面航向（000.0~359.9度，以真北为参考基准)
		unsigned char GPS_STA_Num;

		geometry_msgs::Vector3 uwb_data;

		ros::NodeHandle nh;
		serial::Serial ser;
		ros::Publisher pub_uwb = nh.advertise<geometry_msgs::Vector3>("decode_frame/uwb_data", 1000);
		ros::Timer publish_timer = nh.createTimer(ros::Duration(0.1), &DecodeFrame::CB_publishCycle, this); // 非静态成员函数需要加上域作用符取地址符号与this指针

		DecodeFrame(void)
		{
		};
		~DecodeFrame(){};
		void Decode_frame(unsigned char data);
		uint32_t hex2deci(const char *strHex);
		void CB_publishCycle(const ros::TimerEvent& e);

};
