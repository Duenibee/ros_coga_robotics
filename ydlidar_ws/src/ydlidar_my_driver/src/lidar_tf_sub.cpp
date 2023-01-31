#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include "sensor_msgs/PointCloud.h"
#include <tf/transform_listener.h>
class Merge{
  public:
    ros::Subscriber sub1;
    ros::Subscriber sub2;
    ros::Publisher pub1;
    ros::Publisher pub2;
    tf::Vector3 T_matrix;
    tf::Matrix3x3 R;
    tf::TransformListener listener;

    ros::NodeHandle n;
    int num_;
  
  void get_point_cloud(const sensor_msgs::PointCloud point_in)
  {
    tf::Vector3 point3;
    tf::Vector3 pointout;    
    sensor_msgs::PointCloud pointout2;
    pointout2.points.resize(point_in.points.size());
    pointout2.channels=point_in.channels;
    // pointout2.header=point_in.header;
    if(num_==0) {pointout2.header.frame_id="laser_frame3";}
    if(num_==1) {pointout2.header.frame_id="laser_frame3";}
    
    for(int i=0; i<point_in.points.size(); i++)
    {
      point3 = tf::Vector3(point_in.points.at(i).x, point_in.points.at(i).y, 0);
      pointout=this->R*point3;
      pointout+=this->T_matrix;
      
      pointout2.points.at(i).x=pointout.getX();
      pointout2.points.at(i).y=pointout.getY();
      pointout2.points.at(i).z=pointout.getZ();
    }
    if(num_==0) {pub1.publish(pointout2);}
    if(num_==1) {pub2.publish(pointout2);}
  }

  Merge(int num)
  {
    this->num_=num;
    tf::StampedTransform transform;
    ros::Time time_now = ros::Time::now();
    if(num_==0)
    {
      this->sub1=n.subscribe("/point_cloud1",100, &Merge::get_point_cloud,this);
      this->pub1 = n.advertise<sensor_msgs::PointCloud>("/point_cloud3", 100);
      try{
        listener.waitForTransform("/base_link", "/laser_frame1", time_now, ros::Duration(1.0));

        listener.lookupTransform("/base_link", "/laser_frame1",
                                    time_now, transform);
      }
      catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
      }

      this->R = tf::Matrix3x3(transform.getRotation());
      this->T_matrix=transform.getOrigin();
    }

    if(num_==1)
    {
      this->sub2=n.subscribe("/point_cloud2",100, &Merge::get_point_cloud,this);
      this->pub2 = n.advertise<sensor_msgs::PointCloud>("/point_cloud4", 100);  
      try{
        listener.waitForTransform("/base_link", "/laser_frame2", time_now, ros::Duration(1.0));

        listener.lookupTransform("/base_link", "/laser_frame2",
                                    time_now, transform);
      }
      catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
      }

      this->R = tf::Matrix3x3(transform.getRotation());
      this->T_matrix=transform.getOrigin();
    }
  }
};

int main(int argc, char** argv){
  ros::init(argc, argv, "lidar_tf_sub");
  ros::NodeHandle n;
  ros::Publisher pub=n.advertise<sensor_msgs::PointCloud>("/point_cloud_merge", 100);
  sensor_msgs::PointCloud point_out_merge;
  Merge merge1(0);
  Merge merge2(1);
  ros::spin();
  return 0;
}