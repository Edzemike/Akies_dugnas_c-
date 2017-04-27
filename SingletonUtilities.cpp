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

cv::Mat SingletonUtilities::CropToROI(cv::Mat *imgGrayscale)
{
	cv::Rect ROI; // Region of interest
	cv::Mat imgCropped;
	int xmin = imgGrayscale->cols;
	int ymin = imgGrayscale->rows;
	int xmax = 0;
	int ymax = 0;

	// Go through all pixels
	for (int x = 0; x < imgGrayscale->cols; x++)
	{
		for (int y = 0; y < imgGrayscale->rows; y++)
		{
			float pixel = imgGrayscale->at<uchar>(y, x);

			// If pixel is black (not part of ROI),
			// we set boundaries to that point.
			if (pixel == 0)
			{
				if (x < xmin)
				{
					xmin = x;
				}
				if (y < ymin)
				{
					ymin = y;
				}
				if (x > xmax)
				{
					xmax = x;
				}
				if (y > ymax)
				{
					ymax = y;
				}
			}
		}
	}

	ROI = cv::Rect(xmin, ymin, xmax - xmin, ymax - ymin);
	imgCropped = cv::Mat(*imgGrayscale, ROI);
	return imgCropped;
}

void SingletonUtilities::DisplayImage(std::string name, cv::Mat *image)
{
	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
	cv::namedWindow(name, CV_WINDOW_NORMAL);
	cv::imshow(name, *image);
}

cv::Mat SingletonUtilities::ApplyMovingAverageFilter(cv::Mat *imgGrayscale, int filterSize) // Blures
{
	// note: don't put the same matrix in destination (to)
	// or it (original) will get blurred
	cv::Mat imgBlurred;
	//		from			to					  x			   y
	blur(*imgGrayscale, imgBlurred, cv::Size(filterSize, filterSize));

	return imgBlurred;
}