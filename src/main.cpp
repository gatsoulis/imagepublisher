/**
 * @file main.cpp
 * @brief
 * @date Created on: 4 Nov 2010
 *
 * @author Yianni
 * [Note: deprecated practice as git will maintain author info and blame logs.]
 */

#include "boost/filesystem.hpp"
#include <iostream>
#include <vector>
using namespace boost::filesystem;
using namespace std;



int main(int argc, char *argv[])
{
	string p(argc <= 1 ? "." : argv[1]);
	vector<string> filenames;

	if (is_directory(p)) {
		for (directory_iterator itr(p); itr!=directory_iterator(); ++itr)
			if (is_regular_file(itr->status())) {
				string str(itr->path().filename());
				filenames.push_back(str);
			}
	} else
		cout << "Enter directory name or leave blank for current directory" << endl;

	for (vector<string>::iterator itr = filenames.begin(); itr != filenames.end(); ++itr) {
		cout << *itr << endl;
	}

	return 0;
}
