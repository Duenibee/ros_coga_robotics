#include "ros/ros.h"
#include "std_msgs/String.h"

// callback 함수 노드가 msg를 받으면 동작하는 함수
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  ros::spin(); //무한 반복 -> 대기

  return 0;
}