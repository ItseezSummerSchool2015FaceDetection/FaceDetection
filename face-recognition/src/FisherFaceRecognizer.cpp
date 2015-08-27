#include "FisherFaceRecognizer.hpp"

FisherFaceRecognizer::FisherFaceRecognizer(const std::vector<cv::Mat>& images, const std::vector<int>& labels)
{
    model = cv::createFisherFaceRecognizer();
    model->train(images, labels);
}

FisherFaceRecognizer::~FisherFaceRecognizer()
{

}

int FisherFaceRecognizer::predict(const cv::Mat& test)
{
    return model->predict(test);
}
