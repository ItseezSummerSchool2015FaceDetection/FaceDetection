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

	const double scaleFactor = 1.1;
	const int minNeighbors = 500;

    std::vector<int> rejectLevels;
    std::vector<double> levelWeights;

    cascades[0].detectMultiScale(image, objects, /*rejectLevels, levelWeights,*/ scaleFactor, minNeighbors);

    if (objects.size() != 0)
    {
        people.push_back(1);
    }
    cascades[1].detectMultiScale(image, objects, rejectLevels, levelWeights);

    if (objects.size() != 0)
    {
        people.push_back(2);
    }

    if (levelWeights.size() != 0)
    {
        std::cout << "Lol!" << std::endl;
    }

	for (int i = 3; i < cascades.size(); i++)
	{
		cascades[i].detectMultiScale(image, objects, /*rejectLevels, levelWeights,*/ scaleFactor, minNeighbors);

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
