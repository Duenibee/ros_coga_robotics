#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include "turtlesim/Pose.h"

// callback 함수 노드가 msg를 받으면 동작하는 함수
void TurtleListener(const turtlesim::Pose &turtle_pose)
{
  std::cout<<turtle_pose.x<<std::endl;
  std::cout<<turtle_pose.y<<std::endl;

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "turtle_listener_node");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/turtle1/pose", 1000, TurtleListener);

  ros::spin(); //무한 반복 -> 대기

  return 0;
}