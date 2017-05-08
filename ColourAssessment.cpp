#include "ColourAssessment.h"

ColourAssessment::ColourAssessment(std::string imagePath)
/**
* Tuščias konstruktorius objekto sukūrimui.
*/
{
}

ColourAssessment::ColourAssessment(std::vector<std::string> &data)
/**
* Konstruktorius, iškviečiantis "Colour Assessment" (spalvos vertinimo) apmokymą.
*/
{
	namesOfImages = data;
	setColourClassifierData(namesOfImages);
}

ColourAssessment::~ColourAssessment()
/**
* Destruktorius objekto sunaikinimui.
*/
{
}

void ColourAssessment::setColourClassifierData(std::vector<std::string> namesOfImages)
/**
* Programos apmokymas apdorojant mokomasias nuotraukas.
* Iškviečiamas metodas, nustatantis spalvos įvertinimo matus, rezultatai išsugomi faile.
*/
{
	std::ofstream output;
	output.open("./qualityParameters/parametersColour.txt");
	imagePool = namesOfImages.size();
	for (int i = 0; i < imagePool; i++)
	{
		gradesOfImages.push_back(getColourMeasuresHSV("./images/Colour/"+namesOfImages[i]));
		output << gradesOfImages[i][0] << " " << gradesOfImages[i][1] << " " << gradesOfImages[i][2] << " " << namesOfImages[i] << std::endl;
	}
	output.close();
}

std::vector<float> ColourAssessment::getColourMeasuresBGR(std::string path)
{
	cv::Mat imgOriginal;
	std::vector<float> ColourMeasures;
	float CM1, CM2, CM3;
	float average = 0;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	int B = 0, D = 0, O = 0; // brigth dark normal
	int totalPixels = imgOriginal.total();

	for (int x = 0; x < imgOriginal.cols; x++) {
		for (int y = 0; y < imgOriginal.rows; y++) {
			cv::Vec3b colour = imgOriginal.at<cv::Vec3b>(cv::Point(x, y));
			int averageColor = (colour.val[0] + colour.val[1] + colour.val[2]) / 3;
			average += averageColor;
			if (averageColor <= Bmax && averageColor > Omax)
				B++;
			else if (averageColor <= Omax && averageColor > Dmax)
				O++;
			else if (averageColor <= Dmax && averageColor >= 0)
				D++;
			else
				std::cout << "Unexpected error." << std::endl;
		}
	}

	CM1 = (float)B / (float)totalPixels;
	CM2 = (float)D / (float)totalPixels;
	CM3 = (float)O / (float)totalPixels;
	average /= (float)totalPixels;

	ColourMeasures.push_back(CM1);
	ColourMeasures.push_back(CM2);
	ColourMeasures.push_back(CM3);
	ColourMeasures.push_back(average);

	return ColourMeasures;
}

void ColourAssessment::makeBackProjections(std::string path) {
	cv::Mat imgColorMapBright = SingletonUtilities::Instance()->ReadImage("./images/colour/bright.jpg");
	cv::Mat imgColorMapDark = SingletonUtilities::Instance()->ReadImage("./images/colour/dark.jpg");
	cv::Mat imgColorMapNormal = SingletonUtilities::Instance()->ReadImage("./images/colour/normal.jpg");
	cv::Mat imgCroppedColorMapBright = SingletonUtilities::Instance()->CropToROI(&imgColorMapBright);
	cv::Mat imgCroppedColorMapDark = SingletonUtilities::Instance()->CropToROI(&imgColorMapDark);
	cv::Mat imgCroppedColorMapNormal = SingletonUtilities::Instance()->CropToROI(&imgColorMapNormal);
/*	cv::Mat imgHSVCroppedColorMapBright;
	cv::Mat imgHSVCroppedColorMapDark;
	cv::Mat imgHSVCroppedColorMapNormal;
	cv::cvtColor(imgCroppedColorMapBright, imgHSVCroppedColorMapBright, cv::COLOR_BGR2HSV);
	cv::cvtColor(imgCroppedColorMapDark, imgHSVCroppedColorMapDark, cv::COLOR_BGR2HSV);
	cv::cvtColor(imgCroppedColorMapNormal, imgHSVCroppedColorMapDark, cv::COLOR_BGR2HSV);*/

	cv::Mat imgOriginal;
	cv::Mat imgCropped;
//	cv::Mat imgHSV;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);
//	cv::cvtColor(imgCropped, imgHSV, cv::COLOR_BGR2HSV);

	cv::Mat imgBackProjectionBright = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgCroppedColorMapBright);
	SingletonUtilities::Instance()->SaveBackProjection("images/Colour/bb/", &imgBackProjectionBright);
	cv::Mat imgBackProjectionDark = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgCroppedColorMapDark);
	SingletonUtilities::Instance()->SaveBackProjection("images/Colour/bd/", &imgBackProjectionDark);
	cv::Mat imgBackProjectionNormal = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgCroppedColorMapNormal);
	SingletonUtilities::Instance()->SaveBackProjection("images/Colour/bn/", &imgBackProjectionNormal);
}

std::vector<float> ColourAssessment::getColourMeasuresHSV(std::string path)
/**
* Metodas apskaičiuojantis spalvos vertinimo matus.
* Naudojama HSV spalvų skalė. (Alternatyvus metodas getColourMeasuresBGR)
*/
{
	cv::Mat imgOriginal;
	cv::Mat imgCropped;
	cv::Mat imgHSV;
	std::vector<float> colourMeasures;
	float CM1, CM2, CM3;
	int B = 0, D = 0, O = 0; // brigth dark normal
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);
	
	int totalPixels = imgCropped.total();
	
	cv::cvtColor(imgCropped, imgHSV, cv::COLOR_BGR2HSV);

	for (int x = 0; x < imgHSV.cols; x++) {
		for (int y = 0; y < imgHSV.rows; y++) {
			cv::Vec3b colour = imgHSV.at<cv::Vec3b>(cv::Point(x, y));
			int colorBrightness = (colour.val[2]);

			if (colorBrightness <= Bmax && colorBrightness > Omax)
				B++;
			else if (colorBrightness <= Omax && colorBrightness > Dmax)
				O++;
			else if (colorBrightness <= Dmax && colorBrightness >= 0)
				D++;
			else
				std::cout << "Unexpected error." << std::endl;
		}
	}

	CM1 = (float)B / (float)totalPixels;
	CM2 = (float)D / (float)totalPixels;
	CM3 = (float)O / (float)totalPixels;

	colourMeasures.push_back(CM1);
	colourMeasures.push_back(CM2);
	colourMeasures.push_back(CM3);

	return colourMeasures;
}

std::string ColourAssessment::getColourQuality(std::vector<float> mesurements)
/**
* Įvertinama nuotraukos kokybė pagal apsimokymo metu gautus rezultatus.
* Skaičiuojamas atstumas tarp nuotraukų vertinimo trimatėje erdvėje.
*/
{
	std::string quality = "please show some exaples of bright dark and normal";
	std::ifstream parameters;
	float a, b, c, l1 = 1, l2 = 1;
	std::string s;
	parameters.open("./qualityParameters/parametersColour.txt");
	while (!parameters.eof())
	{
		parameters >> a;
		parameters >> b;
		parameters >> c;
		parameters >> s;
		l2 = sqrt(pow((a - mesurements[0]), 2) + pow((b - mesurements[1]), 2) + pow((c - mesurements[2]), 2));
		if (l2 <= l1) {
			l1 = l2;
			quality = s;
		}
	}

	parameters.close();
	return quality;
}
