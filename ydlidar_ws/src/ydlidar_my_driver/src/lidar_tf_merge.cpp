#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include "sensor_msgs/PointCloud.h"
#include <tf/transform_listener.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>

sensor_msgs::PointCloud point2;

int main(int argc, char** argv){
  ros::init(argc, argv, "lidar_tf_sub_merge");
  ros::NodeHandle n;
  ros::Subscriber sub1;
  ros::Subscriber sub2;

  // std::cout<<point1<<std::endl;
  ros::spin();
  return 0;
}