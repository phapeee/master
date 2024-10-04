#include <ros/ros.h>  // This .h file must always be included in all ROS code
// There must be a .h file for every message type used
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3.h>
#include <sensor_msgs/Joy.h>
#include <keyboard/Key.h> // This is where the keyboard/Key message type is defined
#include <iostream>
#include "vector3/Vector3.h"

#define BUTTON_COUNT (11)
#define AXES_COUNT (8)
#define STEP_LIN_VEL (0.5)
#define STEP_ANG_VEL (0.1)

// Global variables
sensor_msgs::Joy gamepad_data; // This will hold the message from the topic we subscribe to
vector3::Vector3 velocities;	// initialize vector3 object
double max_lin_vel;
double max_ang_vel;
bool pressed_buttons[BUTTON_COUNT];
/* This is our callback function that is called when something is published to the keyboard/keydown
topic. Callback functions take in 1 parameter that is an object of the message type that topic 
receives. Callback functions always return void, so we will store this data in our global variable
so we can use it in other functions.*/

void gamepad_callback(const sensor_msgs::Joy& joy_msg)
{
	gamepad_data = joy_msg; // Store the data from this topic into our global variable
	
	/*	
    	std_msgs::String button_data;
	for (auto i: joy_msg.buttons){
		if (i == 0){
    			button_data.data += "0"; 
		}
		if (i==1){
			button_data.data += "1";
		}
		button_data.data += ", ";
	}
	ROS_INFO("Buttons: %s", button_data.data.c_str());
    	button_data.data = "";
	char Float_string[9];
	for (auto i: joy_msg.axes){
		std::sprintf(Float_string, "%.4f, ", i);
		button_data.data += Float_string;
	}
	ROS_INFO("Axes: %s", button_data.data.c_str());
	*/

	for (int i=0; i < BUTTON_COUNT; i++){
		if (gamepad_data.buttons[i] && !pressed_buttons[i]){
			switch (i){
				case 0:
					max_lin_vel -= STEP_LIN_VEL;
					ROS_INFO("Max linear velocity: %.1f", max_lin_vel);
					break;
				case 1:
					max_ang_vel += STEP_ANG_VEL;
					ROS_INFO("Max angular velocity: %.1f", max_ang_vel);
					break;
				case 2:
					max_ang_vel -= STEP_ANG_VEL;
					ROS_INFO("Max angular velocity: %.1f", max_ang_vel);
					break;
				case 3:
					max_lin_vel += STEP_LIN_VEL;
					ROS_INFO("Max linear velocity: %.1f", max_lin_vel);
					break;
				default: break;
			}
		}
	}

	velocities.setY(gamepad_data.axes[1]);
	velocities.setZ(gamepad_data.axes[0]);
	//velocities.normalize();
	velocities.scaleComp(1, max_lin_vel, max_ang_vel);

	//ROS_INFO("Local velocities: %.3fi + %.3fj + %.3fk", velocities.getX(), velocities.getY(), velocities.getZ());

	for (int i=0; i < BUTTON_COUNT; i++){
		pressed_buttons[i] = gamepad_data.buttons[i] == 1;
	}	
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "master"); /* This is how you create a node. The only thing that
    changes for this function is the string argument which indicates the name of the node.*/
    ros::NodeHandle n; /* This creates something called an object that is used in object-
    oriented programming. This object is responsible for handling the publications and 
    subscriptions that this node talks to.*/

    ros::Publisher my_publisher_object = n.advertise<geometry_msgs::Vector3>("local_velocities", 1);
    
    /* This creates a publisher object. This is done by calling the function n.advertise which
    is a function tied to the object that we created earlier. The <> brackets indicate what
    message type is going to be used for this topic. The first argument is the name of the 
    topic. The "1" argument says to use a buffer size of 1; could make larger, if expect network 
    backups */

    ros::Subscriber gamepad_in = n.subscribe("/joy", 1, gamepad_callback);
    /* This creates a subscriber object. This is done by calling the function n.subscribe which
    is also a function tied to the object of class ros::NodeHandle. The first argument is the 
    name of the topic that you want to subscribe to. The second argument is the buffer size which
    we will just set to 1. The third argument is the callback function that gets called when data
    is published to this topic. This works like an interupt.*/

    // Initialize global variable
	max_lin_vel = 1.0;
	max_ang_vel = 1.0;
	for (auto &i: pressed_buttons){
		i = false;
	}

    while (ros::ok()) // The ros::ok() function returns true as long as ROS is running
    {

        my_publisher_object.publish(velocities.getVector()); 
	/* This is the function that we call to publish        
	the data in the input_float object. Note that we use the my_publisher_object to call the 
        function.*/
    	ros::spinOnce();
	}
}


