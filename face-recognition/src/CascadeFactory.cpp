#pragma once
#include "CascadeFactory.hpp"

#define WINDOWS

#ifdef WINDOWS
	#include <windows.h>
#endif

void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
#ifdef WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do
	{
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} 
	while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL)
	{
		const std::string file_name = ent->d_name;
		const std::string full_file_name = directory + "/" + file_name;

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
}


CascadeFactory::CascadeFactory()
{

}


CascadeFactory::~CascadeFactory()
{

}


bool CascadeFactory::initialize(std::string path)
{
	std::vector<std::string> fileNames;
	GetFilesInDirectory(fileNames, path);

	if (fileNames.size() == 0)
	{
		return false;
	}

	cascades.resize(fileNames.size());
	for (int i = 0; i < fileNames.size(); i++)
	{
		if (!cascades[i].load(fileNames[i]))
		{
			std::cout << "Warning: CascadeClassifier <" << fileNames[i] << "> was not loaded!" << std::endl;
		}
	}
	return true;
}


std::vector<int> CascadeFactory::detectPerson(const cv::Mat& image, std::vector<cv::Rect>& objects)
{
	std::vector<int> people;

	const double scaleFactor = 1.1;
	const int minNeighbors = 50;

	for (int i = 0; i < cascades.size(); i++)
	{
		cascades[i].detectMultiScale(image, objects, scaleFactor, minNeighbors);

		if (objects.size() != 0)
		{
			people.push_back(i);
		}
	}

	return people;
}


std::vector<std::string> CascadeFactory::getPeople(const std::vector<int>& people)
{
	std::vector<std::string> peopleNames(people.size());

	for (int i = 0; i < peopleNames.size(); i++)
	{
		peopleNames[i] = getPerson(people[i]);
	}

	return peopleNames;
}


std::string CascadeFactory::getPerson(int person)
{
	switch (person)
	{
	case 1: 
		return "Makarova Olga";
		break;

	case 2:
		return "Shchedrin Roman";
		break;

	case 3:
		return "Kozitsin Alexander";
		break;
	}
	return "unknown";
}
