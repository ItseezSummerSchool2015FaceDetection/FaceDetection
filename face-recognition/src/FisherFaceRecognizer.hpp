#pragma once

#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "Utility.hpp"

class FisherFaceRecognizer
{
public:
    FisherFaceRecognizer(const std::vector<cv::Mat>&, const std::vector<int>&);
    ~FisherFaceRecognizer();

    int predict(const cv::Mat&);
private:
    cv::Ptr<cv::FaceRecognizer> model;
};