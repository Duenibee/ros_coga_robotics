#include "ros/ros.h" // Must include for all ROS C++
#include "geometry_msgs/Twist.h" // To control linear and angular velocity of turtle
#include "turtlesim/Pose.h" //Turtlesim의 위치 data
#include <sstream>
#include<iostream>
#include "std_msgs/Bool.h"

// Turtle Class
class Turtle{
    private:
    // Variables for Current Pose
    double x;
    double y;
    double theta;
    bool isdrive=false;

    // ROS
    ros::NodeHandle n; // Create its specific node handler
    ros::Publisher velocityPublisher; // publisher 
    ros::Subscriber poseSubscriber; // turtlesim 위치 subscriber
    ros::Subscriber drivesub; // 주행할지 안할지 받는 subscriber
    public:
    // Constructor
    Turtle()
    {
        this->velocityPublisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10); // cmd_vel 을 publish
        this->poseSubscriber = n.subscribe("/turtle1/pose", 10, &Turtle::updatePose, this); //pose subscribe
        this->drivesub= n.subscribe("/driver_start",10, &Turtle::drive,this); // drive 할지 안할지 subscribe
    }

    // Get X of turtle
    double getX()
    {
        return x;
    }
    // Get Y of turtle
    double getY()
    {
        return y;
    }
    double getTheta()
    {
        return theta;
    }
    // for movewhere: x면 True, y방향이면 false
    void drive(std_msgs::Bool isdrive_in)
    {
        this->isdrive=isdrive_in.data;
    }
    // dist: 거리, speed: 속도
    void move(double dist, double speed)
    {
        // Update position
        ros::spinOnce();

        // Initialize msg
        geometry_msgs::Twist vel_msg;

        // Time based implementation
        double initialtime, traveltime, finaltime;

        // Set forward or backwards movement
        if(this->isdrive) //모든 동작은 isdrive가 true여야 움직인다.
        {
            vel_msg.linear.x = abs(speed);
            //Publish Move
            velocityPublisher.publish(vel_msg); // 변한값 publish

            ROS_INFO("Turtle is moving!\n");
            
            // Stop when reaching the distance specified
            initialtime = ros::Time::now().toSec(); 
            traveltime = dist/speed; // 시간=거리/속도
            finaltime = initialtime + traveltime;
            while (ros::Time::now().toSec() <= finaltime)
            {
                velocityPublisher.publish(vel_msg);
                ros::spinOnce();
            }
            // Reached specified distance

            vel_msg.linear.x = 0;
            vel_msg.linear.y = 0;
            // Publish again
            velocityPublisher.publish(vel_msg);
            
            ROS_INFO("Turtle reached its destination!\n");
            }
        else
        {
            ROS_INFO("if you want to drive, you have to input isdrive true");
        }
    }


    void turtlerRotate()
    {
        ros::spinOnce(); //이전에 있었던 callback 함수들 호출
        double initialtime, traveltime, finaltime;
        double goal_theta=0;
        geometry_msgs::Twist vel_msg;
        ROS_INFO("Turtle is turning!\n");
        if(this->isdrive) //모든 동작은 isdrive가 true여야 움직인다.
        {
            goal_theta=1.57;
            vel_msg.angular.z=0.5;
            initialtime = ros::Time::now().toSec(); 
            traveltime = goal_theta/0.5; // 시간=각/각속도
            finaltime = initialtime + traveltime;
            
            while(ros::Time::now().toSec() <= finaltime)
            {
                velocityPublisher.publish(vel_msg);
                ros::spinOnce();
            }
            vel_msg.angular.z = 0;
            // Publish again
            velocityPublisher.publish(vel_msg);
            
            ROS_INFO("Turtle reached its degree!\n");
        }
        else
        {
            ROS_INFO("if you want to drive, you have to input isdrive true");
        }
    }

    void updatePose(const turtlesim::Pose::ConstPtr& msg)
    {
        this->x = msg->x;
        this->y = msg->y;
        this->theta= msg->theta;
    }

};



int main(int argc, char **argv)
{
    // Initialize ROS node
    ros::init(argc, argv, "robot_runner");
    Turtle turtle = Turtle();
    ros::Rate loop_rate(300);
    while(ros::ok()) // while ros is running, repeat this 
    {
        turtle.turtlerRotate();
        turtle.move(4,2);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}