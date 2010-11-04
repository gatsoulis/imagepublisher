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
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace boost::filesystem;
using namespace std;



int main(int argc, char *argv[])
{
	string p(argc <= 1 ? "." : argv[1]);
	vector<string> filenames;	IplImage *img = NULL;

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
	cvMoveWindow("main", 100, 100);
	for (vector<string>::iterator itr = filenames.begin(); itr != filenames.end(); ++itr) {
		cout << itr->c_str() << endl;
		img = cvLoadImage(itr->c_str());
		if(!img)
			cout << "Could not load image file: " << itr->c_str() << endl;
		else {
			cvShowImage("main", img);
			cvWaitKey(0);
		}
	}
	cvReleaseImage(&img );

	return 0;
}
