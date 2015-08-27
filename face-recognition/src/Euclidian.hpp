#pragma once

#include <vector>
#include <string>

#include "Utility.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Euclidian
{
public:
    Euclidian(std::string);
    ~Euclidian();

    int loadAndRecognize(cv::Mat);

private:
    std::vector<cv::Mat> everyone;
    std::vector<std::string> files;
    cv::Rect cutrect;

    cv::Mat equalize(cv::Mat&);
    int getClass(cv::Mat, std::vector<cv::Mat>, std::vector<std::string>, cv::Rect);
};