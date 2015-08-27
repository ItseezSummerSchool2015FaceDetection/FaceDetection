#include "Euclidian.hpp"

Euclidian::Euclidian(std::string pathToTrain)
{
    int cutX = 0, cutY = 0;
    cutrect = Rect(cutX, cutY, 64 - cutX * 2, 64 - cutY * 2);

    GetFilesInDirectory(files, pathToTrain);
    everyone.resize(files.size());

    for(int i = 0; i < files.size(); i++)
    {
        Mat tmp, gs;
        tmp = imread(files[i]);
        everyone[i] = equalize(tmp(cutrect));
    }

}


Euclidian::~Euclidian()
{

}


cv::Mat Euclidian::equalize(cv::Mat& m)
{
    Mat res, gs, eq;

    if (m.type() != CV_8UC1 && m.type() != CV_16UC1)
    {
        cv::cvtColor(m, gs, CV_BGR2GRAY);
    }
    else
    {
        m.copyTo(gs);
    }
    equalizeHist(gs, eq);
    eq.copyTo(res);
    return res;
}


int Euclidian::loadAndRecognize(cv::Mat img64)
{
    return getClass(img64, everyone, files, cutrect);
}


int Euclidian::getClass(Mat img64, vector<Mat> everyone, vector<string> files, Rect cutrect)
{
    int cutX = 12, cutY = 7;
    Mat gs64, eq64;
    eq64 = equalize(img64(cutrect));
    double minNorm = norm(eq64) + 10e9;
    int min_i = 0;
    for (int i_pat = 0; i_pat < everyone.size(); i_pat++)
    {
        double diff = norm(eq64 / everyone[i_pat]);
        diff += norm(everyone[i_pat]/ eq64);
        diff += norm(everyone[i_pat]- eq64)/256.0;
        if (minNorm > diff)
        {
            minNorm = diff;
            min_i = i_pat;
        }
    }
    return str2class(files[min_i]);
}