/*#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "CascadeFactory.hpp"

const char* params =
    "{ h | help     | false | print usage                                   }"
    "{   | detector |       | path to folder with detectors                 }"
    "{   | image    |       | image to detect objects on                    }"
    "{   | video    |       | video file to detect on                       }"
    "{   | camera   | false | whether to detect on video stream from camera }";


static cv::CascadeClassifier anyface;

void detectOnImage(CascadeFactory& factory, const cv::Mat& image, cv::Mat& res)
{
	std::vector<cv::Rect> faces;
	std::vector<cv::Rect> found;

	std::vector<int> people;
	std::vector<std::string> peopleNames;

	res = image.clone();

	double scaleFactor = 1.1;
	int minNeighbors = 3;
    std::vector<double> levelWeights;
     std::vector<int> rejectLevels;
	anyface.detectMultiScale(image, faces,rejectLevels,levelWeights);
	for (int i = 0; i < faces.size(); i++)
	{
		cv::Rect roi_rect = faces[i];
		roi_rect.y -= roi_rect.height * 0.3;
		roi_rect.height *= 1.4;
		roi_rect.x -= roi_rect.width * 0.1;
		roi_rect.width *= 1.2;
		roi_rect = roi_rect & cv::Rect(0, 0, image.cols, image.rows);

		rectangle(res, roi_rect, cv::Scalar(0,0,200),2);
		cv::Mat recognizeImage;
		cv::Size OptimalSize(128,128);
		resize(image(roi_rect), recognizeImage, OptimalSize, 0, 0, cv::INTER_AREA);
		imshow("r", recognizeImage);

		people = factory.detectPerson(image(roi_rect), found);
		peopleNames = factory.getPeople(people); 
		if (found.size())
		{
			rectangle(res, roi_rect, cv::Scalar(0, 200, 0), 2);

            if (people.size() == peopleNames.size())
            {
                std::cout << "Was found: " << std::endl;
                for (int j = 0; j < peopleNames.size(); j++)
                {
                    std::cout << people[j] << " " << peopleNames[j] << std::endl;
                }
                std::cout << "-------------------------" << std::endl;
            }
		}
	}
}


int main(int argc, char** argv)
{
	// Parse command line arguments.
	cv::CommandLineParser parser(argc, argv, params);
	// If help flag is present, print help message and exit.
	if (parser.get<bool>("help"))
	{
		parser.printParams();
		return 0;
	}

	std::string detector_dir = parser.get<std::string>("detector");
	std::string image_file = parser.get<std::string>("image");
	std::string video_file = parser.get<std::string>("video");
	bool use_camera = parser.get<bool>("camera");

    if (!anyface.load("C:/Temp/GitKoz/Project/FaceDetection/face-recognition-build/bin/haarcascade_frontalface_default.xml"))
    {
        std::cerr << "Error loading haarcascade" << std::endl;
        return -1;
    }

	CascadeFactory factory = CascadeFactory();

	factory.initialize(detector_dir);

	if (!image_file.empty())
	{   
		cv::Mat image = cv::imread(image_file);
		cv::Mat res;
		detectOnImage(factory, image, res);
		cv::imshow("detect", res);
		int key = cv::waitKey();
	}
	else if (!video_file.empty())
	{
		int key = 0;
		cv::VideoCapture cap(video_file);
		if(!cap.isOpened())
		{
			std::cerr << "Can not open video" << std::endl;
			return -1;
		}
		cv::Mat image, res;
		do
		{
			cap >> image;
			if(image.empty())
			{
				continue;
			}
			detectOnImage(factory, image, res);
			cv::imshow("detect", res);
			key = cv::waitKey(1);
		}
		while (key != 27);

	}
	else if (use_camera)
	{
		int key = 0;
		cv::VideoCapture cap(0);
		if(!cap.isOpened())
		{
			std::cerr << "Can not connect to camera" << std::endl;
			return -1;
		}
		cv::Mat image, res;
		do
		{
			cap >> image;
			if(image.empty())
			{
				continue;
			}
			detectOnImage(factory, image, res);
			cv::imshow("detect", res);
			key = cv::waitKey(1);
		}
		while (key != 27);
	}
	else
	{
		std::cout << "Declare a source of images to detect on." << std::endl;
	}

    return 0;
}*/


#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Utility.hpp"
#include "CascadeFactory.hpp"
#include "test.hpp"

using namespace cv;
using namespace std;

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