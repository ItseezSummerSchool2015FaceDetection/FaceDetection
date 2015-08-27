#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <sys/stat.h>
#include <dirent.h>

using namespace std;
using namespace cv;

//#define WINDOWS
//#include<windows.h>

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

    dir = opendir(directory.c_str());
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




int main(int argc, char** argv)
{
    vector<string> files;
    GetFilesInDirectory(files,"/home/arqwer/code/itseez/faceDetection/FaceDetection/somePics");
    sort(files.begin(), files.end());
    cout<<files.size()<<endl;
    Mat img, disp(64*9,64*9,CV_8UC3);
    for(int i = 0; i < files.size(); i++){
        img = imread(files[i]);
        int row = i/9, col = i%9;
        img.copyTo(disp(Rect(col*64,row*64,64,64)));
        cout<<files[i]<<endl;
    }
    imshow("",disp);
    waitKey(0);
    return 0;
}



