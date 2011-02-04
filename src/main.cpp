/**
 * @file main.cpp
 * @brief
 * @date Created on: 4 Nov 2010
 *
 * @author Yianni
 * [Note: deprecated practice as git will maintain author info and blame logs.]
 */

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem.hpp"
#include "sensor_msgs/Image.h"
#include "image_transport/image_transport.h"
#include "cv_bridge/CvBridge.h"
#include "std_msgs/String.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ros/ros.h>
#include <signal.h>
#include <sstream>

using namespace boost::filesystem;
using namespace std;

void ctrlc(int s)
{
	cout << "ctrl-c pressed" << endl;
	ros::shutdown();
	signal(SIGINT, SIG_DFL);
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "Static_Image_Publisher");
	ros::NodeHandle n;
	image_transport::ImageTransport it_(n);
	sensor_msgs::CvBridge bridge_;
	image_transport::Publisher image_pub_;
	image_pub_ = it_.advertise("/camera_sim/image_raw", 1);
	ros::Publisher strPub = n.advertise<std_msgs::String>("/camera_sim/image_filename", 1, true);
	std_msgs::String msg;
	int cvDelay = (argc == 2 ? atoi(argv[1]) : 500);
	cout << "Change time between images is: " << cvDelay << " ms" << endl;
	string p(argc == 3 ? argv[2] : ".");
	vector<string> filenames;
	IplImage *img = NULL;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5);

	if (is_directory(p)) {
		for (directory_iterator itr(p); itr!=directory_iterator(); ++itr)
			if (is_regular_file(itr->status())) {
				string str(itr->path().file_string());
				filenames.push_back(str);
			}
	} else
		cout << "Enter directory name or leave blank for current directory" << endl;

	sort(filenames.begin(), filenames.end());
	cvNamedWindow("main", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("main", 600, 600);
	signal(SIGINT, ctrlc);
	for(;;) {
		for (vector<string>::iterator itr = filenames.begin(); itr != filenames.end(); ++itr) {
			img = cvLoadImage(itr->c_str());
			if(!img)
				cout << "Could not load image file: " << itr->c_str() << endl;
			else {
				try	{
					image_pub_.publish(bridge_.cvToImgMsg(img, "bgr8"));
					msg.data = itr->c_str();
					strPub.publish(msg);
				} catch (sensor_msgs::CvBridgeException error) {
					ROS_ERROR("error");
				}
				cvPutText(img, itr->c_str(), cvPoint(200, 290), &font, cvScalar(0, 255, 0, 0));
				cvShowImage("main", img);
				cvWaitKey(cvDelay);
				cvReleaseImage(&img);
			}
		}
	}

	ros::spin();
	return 0;
}
