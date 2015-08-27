#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "Utility.hpp"
#include "CascadeFactory.hpp"
#include "FisherFaceRecognizer.hpp"
#include "Euclidian.hpp"


static void test(std::string pathToCascades, std::string pathToTrain, std::string pathToTest)
{
    std::vector<cv::Mat> trainImages;
    std::vector<int> trainLabels;

    read(pathToTrain, trainImages, trainLabels);

    FisherFaceRecognizer recognizer = FisherFaceRecognizer(trainImages, trainLabels);
    
    CascadeFactory cascades = CascadeFactory();
    std::vector<cv::Rect> obj;
    cascades.initialize(pathToCascades);

    std::vector<int> temp;

    Euclidian euclid = Euclidian(pathToTrain);


	std::vector<Mat> testImages;
	std::vector<int> testLabels;

    read(pathToTest, testImages, testLabels);

    std::vector<int> amount(9);
    for (int i = 0; i < testLabels.size(); i++)
        amount[testLabels[i] - 1]++;


	std::string result_message;
	int predictedLabel = -1;


    std::vector<std::vector<int>> fisher(9);
    for (int i = 0; i < 9; i++)
        fisher[i].resize(9);

    std::vector<std::vector<int>> cascade(9);
    for (int i = 0; i < 9; i++)
        cascade[i].resize(9);

    std::vector<std::vector<int>> euclidian(9);
    for (int i = 0; i < 9; i++)
        euclidian[i].resize(9);



	for (int i = 0; i < testImages.size(); i++)
	{
		predictedLabel = recognizer.predict(testImages[i]);

        fisher[predictedLabel - 1][testLabels[i] - 1]++;
        
        temp = cascades.detectPerson(testImages[i], obj);
        if (temp.size() > 0)
        {
            cascade[temp[0] - 1][testLabels[i] - 1]++;
        }

        predictedLabel = euclid.loadAndRecognize(testImages[i]);
        euclidian[predictedLabel - 1][testLabels[i] - 1]++;

        std::cout << "image " << i << " of " << testImages.size() << std::endl;
	}


    std::fstream fischerFile("fisher.csv", std::ios_base::out);
    std::fstream cascadeFile("cascade.csv", std::ios_base::out);
    std::fstream euclideanFile("euclid.csv", std::ios_base::out);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fischerFile << fisher[j][i] / (double) (amount[i]) << '\t';
            cascadeFile << cascade[j][i] / (double) (amount[i]) << '\t';
            euclideanFile << euclidian[j][i] / (double) (amount[i]) << '\t';
        }
        fischerFile << std::endl;
        cascadeFile << std::endl;
        euclideanFile << std::endl;
    }
	//cout << "Total correctness of fisher: " << correctFisher / (double) testImages.size() << endl; 
    //cout << "Total correctness of cascades: " << correctCascades / (double) testImages.size() << endl; 
    //cout << "Total correctness of euclid: " << correctEuclid / (double) testImages.size() << endl; 
}