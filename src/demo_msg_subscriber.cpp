#include "ros/ros.h"
#include "mastering_ros_demo_pkg/demo_msg.h"
#include <iostream>
#include <sstream>

void message_callback(const mastering_ros_demo_pkg::demo_msg::ConstPtr& msg)
{
	ROS_INFO("Received greeting:%s", msg->greeting.c_str());
	ROS_INFO("Received %d", msg->number);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "demo_msg_subscriber");
	ros::NodeHandle node_obj;
	ros::Subscriber message_subscriber = node_obj.subscribe("/demo_msg_topic", 10, message_callback);
	ros::spin();
	return 0;
}
