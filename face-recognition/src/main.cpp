#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define WINDOWS
#include<windows.h>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void GetFilesInDirectory(std::vector<string> &out, const string &directory)
{
#ifdef WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	}
	closedir(dir);
#endif
} // GetFilesInDirectory


static void read(string path, vector<Mat>& images, vector<int>& labels)
{
	vector<string> files;
	GetFilesInDirectory(files, path);

	if (files.size() == 0)
	{
		cerr << "There is no files for training!" << endl;
		return;
	}

	vector<string> splitName;

	for (size_t i = 0; i < files.size(); i++)
	{
		images.push_back(imread(files[i], 0));
		splitName = split(files[i],'/');
		labels.push_back(stoi(splitName.back().substr(0,2)));
	}
}


void test(string path, Ptr<FaceRecognizer> model)
{
	vector<Mat> testImages;
	vector<int> testLabels;

	read(path, testImages, testLabels);

	string result_message;
	int predictedLabel = -1;
	int correct = 0;

	for (int i = 0; i < testImages.size(); i++)
	{
		predictedLabel = model->predict(testImages[i]);

		result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabels[i]);
		cout << result_message << endl;

		if (predictedLabel == testLabels[i])
			correct++;
	}

	cout << "Total correctness: " << correct / (double) testImages.size() << endl; 
}


int main(int argc, const char *argv[])
{
    if (argc != 3)
	{
        cout << "usage: " << argv[0] << " <csv.ext>" << endl;
        exit(1);
    }
    string fn = string(argv[1]);

    vector<Mat> images;
    vector<int> labels;

    try
	{
        read(fn, images, labels);
    } 
	catch (cv::Exception& e)
	{
        cerr << "Error opening file \"" << fn << "\". Reason: " << e.msg << endl;
        exit(1);
    }

    if(images.size() <= 1)
	{
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

	/*
    Mat testSample = imread(argv[2], 0);
    int testLabel = atoi(argv[3]);
	*/

	Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);

    //int predictedLabel = model->predict(testSample);

    //string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    //cout << result_message << endl;

	test(argv[2], model);

    return 0;
}