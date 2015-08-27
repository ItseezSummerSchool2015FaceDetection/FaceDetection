#include "CascadeFactory.hpp"
#include "Utility.hpp"

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
			std::cerr << "Warning: CascadeClassifier <" << fileNames[i] << "> was not loaded!" << std::endl;
		}
        else
        {
            std::cout << "Info: CascadeClassifier <" << fileNames[i] << "> was loaded!" << std::endl;
        }
	}
	return true;
}


std::vector<int> CascadeFactory::detectPerson(const cv::Mat& image, std::vector<cv::Rect>& objects)
{
    std::vector<int> people;

    cv::Mat temp;
    resize(image, temp, cv::Size(128, 128));

	const double scaleFactor = 1.1;
	const int minNeighbors = 3;

	for (int i = 0; i < cascades.size(); i++)
	{
		cascades[i].detectMultiScale(temp, objects, scaleFactor, minNeighbors);

		if (objects.size() != 0)
		{
			people.push_back(i + 1);
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
		return "Golubev";
		break;

	case 2:
		return "Kozitsin";
		break;

	case 3:
		return "Kuklina";
		break;

	case 4:
		return "Mahnev";
		break;

	case 5:
		return "Maslova";
		break;

	case 6:
		return "Moshkina";
		break;

	case 7:
		return "Semichev";
		break;

	case 8:
		return "Shchedrin";
		break;

	case 9:
		return "Makarova";
		break;

    default:
        return "unknown";
        break;
	}
	return "unknown";
}
