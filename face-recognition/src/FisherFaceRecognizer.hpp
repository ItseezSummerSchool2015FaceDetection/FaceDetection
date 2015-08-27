#pragma once

#include <vector>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "Utility.hpp"

class FisherFaceRecognizer
{
public:
    FisherFaceRecognizer(const std::string&);
    ~FisherFaceRecognizer();

    int predict(const cv::Mat&);
private:
    cv::Ptr<cv::FaceRecognizer> model;
};