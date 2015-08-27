#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#define WINDOWS
#include<windows.h>

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


void detectOnImage(CascadeClassifier &classif, const Mat &image, Mat &res){
    res = image.clone();
    vector<Rect> found;
    CascadeClassifier anyface;
    if(!anyface.load("haarcascade_frontalface_default.xml")){
        cerr << "Error loading cascade"<<endl;
    }
    vector<Rect> faces;
    anyface.detectMultiScale(image, faces, 1.1,7);
    vector<Mat> everyone;
    vector<string> files;
    GetFilesInDirectory(files,"negatives");
    everyone.resize(files.size());
    for(int i = 0; i < files.size(); i++){
        Mat tmp, gs;
        tmp = imread(files[i]);
        cvtColor(tmp,gs,CV_BGR2GRAY);
        equalizeHist( gs, everyone[i] );
    }
    cout<<"read done"<<endl;

    for(int i = 0; i < faces.size(); i++){
        
        
        Rect roi_rect = faces[i];
        roi_rect.y -=roi_rect.height * 0.3;
        roi_rect.height *= 1.4;
        roi_rect.x -= roi_rect.width*0.1;
        roi_rect.width *= 1.2;
        roi_rect = roi_rect & Rect(0,0,image.cols,image.rows);

        rectangle(res, roi_rect,Scalar(0,0,200),2);
        Mat recognizeImage;
        Size OptimalSize(128,128);
        resize(image(roi_rect), recognizeImage, OptimalSize, 0, 0, INTER_AREA);
        Size patternSize(64,64);
        Mat m64, gs64, eq64;
        resize(image(roi_rect), m64, patternSize, 0, 0, INTER_AREA);
        cvtColor(m64,gs64,CV_BGR2GRAY);
        equalizeHist( gs64, eq64 );
        if(true){
            double minNorm = norm(m64);
            int min_i = 0;
            for(int i_pat = 0; i_pat < everyone.size(); i_pat++){
                if(minNorm > norm(eq64 - everyone[i_pat])){
                    minNorm = norm(eq64 - everyone[i_pat]);
                    min_i = i_pat;
                }
            }
            cout<<files[min_i].substr(9,13)<<endl;
            char name1[100], name2[100];
            sprintf(name1,"%d found", i);
            sprintf(name2,"%d orig", i);
            imshow(name1,everyone[min_i]);
            imshow(name2,m64);
        }
        imshow("r",recognizeImage);
        classif.detectMultiScale(image(roi_rect), found,1.1,950);    
        if(found.size()){
            rectangle(res, roi_rect,Scalar(0,200,0),2);
        }
    }


}

int main(int argc, char** argv)
{
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, params);
    // If help flag is present, print help message and exit.
    if (parser.get<bool>("help"))
    {
        parser.printParams();
        return 0;
    }

    string detector_file = parser.get<string>("detector");
    string detector_dir = parser.get<string>("detector_dir");
    CV_Assert(!detector_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    // TODO: Load detector.
    CascadeClassifier classif;

    if(!classif.load(detector_file)){
        cerr<<"Can not load detector"<<endl;
    }

    if (!image_file.empty())
    {   
        Mat image = imread(image_file);
        Mat res;
        detectOnImage(classif, image, res);
        imshow("detect", res);
        int key = waitKey();
    }
    else if (!video_file.empty())
    {
        int key = 0;
        VideoCapture cap(video_file);
        if(!cap.isOpened()){
            cerr<<"Can not open video"<<endl;
            return -1;
        }
        Mat image, res;
        do{
            cap >> image;
            if(image.empty()){
                continue;
            }
            detectOnImage(classif, image, res);
            imshow("detect", res);
            key = waitKey(1);
        }while(key != 27);

    }
    else if (use_camera)
    {
        int key = 0;
        VideoCapture cap(0);
        if(!cap.isOpened()){
            cerr<<"Can not connect to camera"<<endl;
            return -1;
        }
        Mat image, res;
        do{
            cap >> image;
            if(image.empty()){
                continue;
            }
            detectOnImage(classif, image, res);
            imshow("detect", res);
            key = waitKey(1);
        }while(key != 27);
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}


