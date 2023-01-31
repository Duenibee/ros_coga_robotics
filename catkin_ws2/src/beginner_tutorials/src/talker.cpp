#include "ros/ros.h" //ros 헤더파일 선언
#include "std_msgs/String.h"

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker"); //ros 초기화, 노드이름 정의, 매개변수 할당
  ros::NodeHandle n; // 토픽, 파라메터 등을 컨트롤하기 위한 노드 핸들 n 선언(자동적으로 노드를 만들고 소멸까지 시켜준다.)

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
   // publisher 선언, 데이터 유형 string, 토픽이름은 chatter,버퍼 사이즈는 1000 

  ros::Rate loop_rate(10);
  //10hz로 전송

  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);
    // 토픽 publish
    ros::spinOnce();
    //ros에 입력된 명령 처리(callbacks 등)
    loop_rate.sleep();
    //설정된 10Hz가 될 때까지 휴식
    ++count;
  }


  return 0;
}