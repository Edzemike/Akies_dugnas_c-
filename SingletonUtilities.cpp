#include "SingletonUtilities.h"

SingletonUtilities *SingletonUtilities::utilities_instance = 0;

SingletonUtilities::SingletonUtilities()
{
}

SingletonUtilities::~SingletonUtilities()
{
}

SingletonUtilities* SingletonUtilities::Instance()
{
	if (!utilities_instance)
		utilities_instance = new SingletonUtilities;
	return utilities_instance;
}

std::vector<std::string> SingletonUtilities::GetFilesNamesInFolder(std::string folder)
{
	std::vector<std::string> namesOfImages;
	std::string search_path = folder + "/*.jpg*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Read all (real) files in current folder
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				namesOfImages.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return namesOfImages;
}

void SingletonUtilities::SaveImage(std::string path, cv::Mat *image)
{
	if (path[path.length() - 1] != '/')
	{
		std::cout << "error: could note save image because of wrong path\n";
		_getch();
		exit(EXIT_FAILURE);
	}

	const char letters[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
	std::string name("img_");
	for (int i = 0; i < 9; i++)
	{
		name += letters[rand() % 10];
	}
	name += ".jpg";

	cv::imwrite(path + name, *image);
}

cv::Mat SingletonUtilities::ReadImage(std::string path)
{
	cv::Mat imgOriginal;
	imgOriginal = cv::imread(path);

	if (imgOriginal.empty()) {
		std::cout << "error: image not read from file\n";
		_getch();
		exit(EXIT_FAILURE);
	}

	return imgOriginal;
}

void SingletonUtilities::DisplayImage(std::string name, cv::Mat *image)
{
	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
	cv::namedWindow(name, CV_WINDOW_NORMAL);
	cv::imshow(name, *image);
}