#include <ros/ros.h>  // This .h file must always be included in all ROS code
// There must be a .h file for every message type used
#include <std_msgs/Float64.h> // This is type std_msgs/Float64 which is a standard 64 bit floating point number
#include <keyboard/Key.h> // This is where the keyboard/Key message type is defined

// Global variables
keyboard::Key message_in; // This will hold the message from the topic we subscribe to
std_msgs::Float64 input_float; /* We create an object of the same type as the message for
the topic we will be publishing to. We will store information into this object that we want
to be published to the topic.*/

/* This is our callback function that is called when something is published to the keyboard/keydown
topic. Callback functions take in 1 parameter that is an object of the message type that topic 
receives. Callback functions always return void, so we will store this data in our global variable
so we can use it in other functions.*/
void keydown_callback(const keyboard::Key& msg)
{
    message_in = msg; // Store the data from this topic into our global variable
    ROS_INFO("Key pressed has code: %d",message_in.code); /* This is how we do print statements in
    ROS. Here we are just printing the code for the key that was pressed.*/
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "node_name"); /* This is how you create a node. The only thing that
    changes for this function is the string argument which indicates the name of the node.*/
    ros::NodeHandle n; /* This creates something called an object that is used in object-
    oriented programming. This object is responsible for handling the publications and 
    subscriptions that this node talks to.*/

    ros::Publisher my_publisher_object = n.advertise<std_msgs::Float64>("topic1", 1);
    /* This creates a publisher object. This is done by calling the function n.advertise which
    is a function tied to the object that we created earlier. The <> brackets indicate what
    message type is going to be used for this topic. The first argument is the name of the 
    topic. The "1" argument says to use a buffer size of 1; could make larger, if expect network 
    backups */

    ros::Subscriber keydown_in = n.subscribe("/keyboard/keydown", 1, keydown_callback);
    /* This creates a subscriber object. This is done by calling the function n.subscribe which
    is also a function tied to the object of class ros::NodeHandle. The first argument is the 
    name of the topic that you want to subscribe to. The second argument is the buffer size which
    we will just set to 1. The third argument is the callback function that gets called when data
    is published to this topic. This works like an interupt.*/

    input_float.data = 0.0; /* We treat this object like a struct and store the information we 
    want to go to the topic. The "data" attribute in this message type is where we store the 
    floating point number to be published.*/

    while (ros::ok()) // The ros::ok() function returns true as long as ROS is running
    {
        input_float.data = input_float.data + 0.001; //increment by 0.001 each iteration

        my_publisher_object.publish(input_float); /* This is the function that we call to publish
        the data in the input_float object. Note that we use the my_publisher_object to call the 
        function.*/
    }
}