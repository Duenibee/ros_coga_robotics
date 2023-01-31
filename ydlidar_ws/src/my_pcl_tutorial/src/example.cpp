#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"
#include <pcl/impl/point_types.hpp>

ros::Publisher pub1;
ros::Publisher pub2;
void laser2cloudmsg(sensor_msgs::LaserScan laser)
{
    laser_geometry::LaserProjection projector;
    sensor_msgs::PointCloud2 pc2_dst;
    projector.projectLaser(laser, pc2_dst,-1,laser_geometry::channel_option::Intensity | laser_geometry::channel_option::Distance);
    pc2_dst.header.frame_id = "map";
    pub1.publish(pc2_dst);
}
int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub1 = nh.subscribe ("/scan1", 10, laser2cloudmsg);
  ros::Subscriber sub2 = nh.subscribe ("/scan2", 10, laser2cloudmsg);
  // Create a ROS publisher for the output point cloud
  pub1 = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin();
}