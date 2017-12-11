#include "ros/ros.h"
#include "mastering_ros_demo_pkg/demo_srv.h"
#include <iostream>
#include <sstream>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "demo_service_client");
	ros::NodeHandle node_obj;
	ros::ServiceClient client = node_obj.serviceClient<mastering_ros_demo_pkg::demo_srv>("demo_service");
	ros::Rate loop_rate(10);

	while(ros::ok())
	{
		mastering_ros_demo_pkg::demo_srv srv;
		std::stringstream ss;
		ss << "Send from here";
		srv.request.in = ss.str();
		if(client.call(srv))
		{
			ROS_INFO("from client:%s, server says:%s", srv.request.in.c_str(), srv.response.out.c_str());
		}
		else
		{
			ROS_INFO("failed to call service");
			return 1;
		}

		// actually, in a service call,spinOnce is not necessary
		// if there are callbacks, spinOnce should be used
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;

}
