#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Vector3.h>

void uwbCallback(const geometry_msgs::Vector3::ConstPtr& uwb_data)
{  
    ROS_INFO("Listener: uwb tag position x = %f ,y = %f ,z =%f", uwb_data->x,uwb_data->y,uwb_data->z);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("decode_frame/uwb_data", 1, uwbCallback);
  //ros::spin()用于调用所有可触发的回调函数。将进入循环，不会返回，类似于在循环里反复调用ros::spinOnce()。
  ros::spin(); 
  return 0;
}

