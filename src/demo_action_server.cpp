#include "ros/ros.h"
#include "mastering_ros_demo_pkg/Demo_actionAction.h"
#include <actionlib/server/simple_action_server.h>
#include <iostream>
#include <sstream>

class Demo_actionAction
{
protected:
	ros::NodeHandle nh_;

	actionlib::SimpleActionServer<mastering_ros_demo_pkg::Demo_actionAction> as;

	mastering_ros_demo_pkg::Demo_actionFeedback feedback;
	mastering_ros_demo_pkg::Demo_actionResult result;

	std::string action_name;
	int goal;
	int progress;

public:
	Demo_actionAction(std::string name) : 
		as(nh_, name, boost::bind(&Demo_actionAction::executeCB, this, _1), false), action_name(name)
		{
			as.registerPreemptCallback(boost::bind(&Demo_actionAction::preemptCB, this));
			as.start();
		}
	
	~Demo_actionAction(void)
	{
	}

	void preemptCB()
	{
		ROS_WARN("%s got preempted!", action_name.c_str());
		result.final_count = progress;
		as.setPreempted(result, "I got preempted!");
	}

	void executeCB(const mastering_ros_demo_pkg::Demo_actionGoalConstPtr &goal)
	{
		if(!as.isActive() || as.isPreemptRequested()) return;
		ros::Rate loop_rate(5);
		ROS_INFO("%s is processing the goal %d", action_name.c_str(), goal->count);
		for(progress = 1; progress <= goal->count; progress++)
		{
			if(!ros::ok())
			{
				result.final_count = progress;
				as.setAborted(result, "I failed!");
				ROS_INFO("%s shutting down", action_name.c_str());
				break;
			}
			
			if(!as.isActive() || as.isPreemptRequested()) return;

			if(goal->count <= progress)
			{
				result.final_count = progress;
				ROS_INFO("%s succeeded at getting to goal %d", action_name.c_str(), goal->count);
				as.setSucceeded(result);
			}
			else
			{
				feedback.current_number = progress;
				ROS_INFO("current: %d    goal: %d", feedback.current_number, goal->count);
				as.publishFeedback(feedback);
			}
			loop_rate.sleep();

		}

	}

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "demo_action");
	ROS_INFO("Starting Demo Action Server!");
	Demo_actionAction demo_action_obj(ros::this_node::getName());
	ros::spin();
	return 0;
}


