#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class CascadeFactory
{
public:
	CascadeFactory();
	~CascadeFactory();

	bool initialize(std::string);
	std::vector<int> detectPerson(const cv::Mat&, std::vector<cv::Rect>&);
	std::vector<std::string> getPeople(const std::vector<int>&);
private:
	std::vector<cv::CascadeClassifier> cascades;

	std::string getPerson(int);
};
