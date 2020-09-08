#include "decode_frame.hpp"


void DecodeFrame::CB_publishCycle(const ros::TimerEvent& e)
{
    pub_imu.publish(imu_data);
}


int main (int argc, char** argv)
{
    //初始化节点
    ros::init(argc, argv, "DecodeFrame_node");
    DecodeFrame decoder;
    std::string data = ""; // 接收串口的单个字节
    static unsigned char uwbdata[] = "";
    static unsigned char num = 0;

    try
    {
        //设置串口属性，并打开串口
        decoder.ser.setPort("/dev/ttyACM0");
        decoder.ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        decoder.ser.setTimeout(to);
        decoder.ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    //检测串口是否已经打开，并给出提示信息
    if(decoder.ser.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        return -1;
    }
    // 根据协议接收到完成的消息就发topic
    while(ros::ok()) // 这里不能用ros::Rate设置延时器,否则会因为延时的原因导致数据解析失败
    {
        if(decoder.ser.available()){
            data = decoder.ser.read();           
            //std::cout << data ;
           decoder.Decode_frame(data[0]);	//进入解帧程序,IMU更新速率不超过300Hz
    
        };
        ros::spinOnce();
    }
}



