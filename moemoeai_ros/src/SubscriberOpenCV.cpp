//
// Created by shagai on 3/26/17.
//

#include "../include/SubscriberOpenCV.h"

int oldTime;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	try
	{
	  int num = msg->header.seq;
	  int newTime =  msg->header.stamp.sec;
	  cv::Mat image = cv_bridge::toCvShare(msg,"bgr8" )->image;
	  cv::imshow("view", image);

	  if((newTime - oldTime) == 1){
		std::stringstream ss;
		ss << "image" << num << ".jpg";
		cv::imwrite( ss.str(), image );
	  }

	  oldTime = newTime;
	}
	catch (cv_bridge::Exception& e)
	{
	  ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
}

int main(int argc, char **argv)
{
    /**
     * The ros::init() function needs to see argc and argv so that it can perform
     * any ROS arguments and name remapping that were provided at the command line.
     * For programmatic remappings you can use a different version of init() which takes
     * remappings directly, but for most command-line programs, passing argc and argv is
     * the easiest way to do it.  The third argument to init() is the name of the node.
     *
     * You must call one of the versions of ros::init() before using any other
     * part of the ROS system.
     */
    ros::init(argc, argv, "image_listener");

    /**
     * NodeHandle is the main access point to communications with the ROS system.
     * The first NodeHandle constructed will fully initialize this node, and the last
     * NodeHandle destructed will close down the node.
     */
    ros::NodeHandle nh;

    cv::namedWindow("view");
    cv::startWindowThread();

    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("image_raw", 1, imageCallback);

    ros::spin();

    cv::destroyWindow("view");

    return 0;
}


