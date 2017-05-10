#include "IlluminationAssessment.h"

IlluminationAssessment::IlluminationAssessment(std::string imagePath)
/**
* Tuščias konstruktorius objekto sukūrimui.
*/
{
}
IlluminationAssessment::IlluminationAssessment(std::vector<std::string> &data)
/**
* Konstruktorius, iškviečiantis "Illumination Assessment" (apšvietimo vertinimo) apmokymą.
*/
{
	namesOfImages = data;
	setIlluminationClassifierData();
}

IlluminationAssessment::~IlluminationAssessment()
/**
*Destruktorius objekto sunaikinimui.
*/
{
}

void IlluminationAssessment::setIlluminationClassifierData()
/**
* Programos apmokymas apdorojant mokomasias nuotraukas.
* Iškviečiamas metodas, nustatantis spalvos įvertinimo matus, rezultatai išsugomi faile.
*/
{
	imagePool = namesOfImages.size();
	std::ofstream out;
	out.open("./qualityParameters/parametersIllumination.txt");
	for (int i = 0; i < imagePool; i++)
	{
		gradesOfImages.push_back(getIlluminationMeasuresBGR("./images/Illumination/" + namesOfImages[i]));
		out << gradesOfImages[i][0] << " " << gradesOfImages[i][1] << " " << gradesOfImages[i][2] << " " << gradesOfImages[i][3] << " " << namesOfImages[i] << std::endl;

	}
}
void IlluminationAssessment::makeBackProjections(std::string path) 
/**
* Padaro backprojection su even.jpg ir uneven.jpg nuotraukomis ir issaugo naujus gautus paveikslėlius,
* kurie vėliau bus naudojami kokybės nustatymui.
*/
{
	cv::Mat imgIlluminationMapEven = SingletonUtilities::Instance()->ReadImage("./images/Illumination/even.jpg");
	cv::Mat imgIlluminationMapUneven = SingletonUtilities::Instance()->ReadImage("./images/Illumination/uneven.jpg");
	cv::Mat imgCroppedIlluminationMapEven = SingletonUtilities::Instance()->CropToROI(&imgIlluminationMapEven);
	cv::Mat imgCroppedIlluminationMapUneven = SingletonUtilities::Instance()->CropToROI(&imgIlluminationMapUneven);
	/*	cv::Mat imgHSVCroppedIlluminationMapBright;
	cv::Mat imgHSVCroppedIlluminationMapDark;
	cv::Mat imgHSVCroppedIlluminationMapNormal;
	cv::cvtIllumination(imgCroppedIlluminationMapBright, imgHSVCroppedIlluminationMapBright, cv::Illumination_BGR2HSV);
	cv::cvtIllumination(imgCroppedIlluminationMapDark, imgHSVCroppedIlluminationMapDark, cv::Illumination_BGR2HSV);
	cv::cvtIllumination(imgCroppedIlluminationMapNormal, imgHSVCroppedIlluminationMapDark, cv::Illumination_BGR2HSV);*/

	cv::Mat imgOriginal;
	cv::Mat imgCropped;
	//	cv::Mat imgHSV;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);
	//	cv::cvtIllumination(imgCropped, imgHSV, cv::Illumination_BGR2HSV);

	cv::Mat imgBackProjectionEven = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgCroppedIlluminationMapEven);
	SingletonUtilities::Instance()->SaveBackProjection("images/Illumination/eb/", &imgBackProjectionEven);
	cv::Mat imgBackProjectionUneven = SingletonUtilities::Instance()->ApplyColorMap(imgOriginal, imgCroppedIlluminationMapUneven);
	SingletonUtilities::Instance()->SaveBackProjection("images/Illumination/ub/", &imgBackProjectionUneven);
}
std::vector<float> IlluminationAssessment::getIlluminationMeasuresBGR(std::string path)
/**
* Metodas apskaičiuojantis apšvietimo vertinimo matus.
* Naudojama BGR spalvų skalė. Pikselio apšvietimas apskaičiuojamas pagal rastą formulę internete:
* apšvietimas = (max(BGR) + min(BGR)) / 2
* apskaičiuojamas vidutinis nuotraukos apšvietimas ir 3 skirtingi nuokrypiai nuo vidurkio
*/
{
	cv::Mat imgOriginal;
	cv::Mat imgBlurred;
	cv::Mat imgCropped;
	imgOriginal = SingletonUtilities::Instance()->ReadImage(path);
	imgBlurred = SingletonUtilities::Instance()->ApplyMovingAverageFilter(&imgOriginal, 3);
	imgCropped = SingletonUtilities::Instance()->CropToROI(&imgOriginal);

	

	std::vector<float> illuminationMeasures;
	// See documentation - Illumination assessment algorithm flowchart
	float IM1 = 0, IM2=0, IM3=0, IM4;

	for (int x = 0; x < imgCropped.cols; x++) {
		for (int y = 0; y < imgCropped.rows; y++) {
			cv::Vec3b illumination = imgCropped.at<cv::Vec3b>(cv::Point(x, y));
			int cmin = min(illumination.val[0], min(illumination.val[1], illumination.val[2]));
			int cmax = max(illumination.val[0], max(illumination.val[1], illumination.val[2]));
			float ill = (cmin + cmax) / 2;
			IM1 += ill;
		}
	}
	IM1 /= imgCropped.total();
	float sumIM2 = 0, sumIM3 = 0, sumIM4 = 0;
	for (int x = 0; x < imgCropped.cols; x++) {
		for (int y = 0; y < imgCropped.rows; y++) {
			cv::Vec3b illumination = imgCropped.at<cv::Vec3b>(cv::Point(x, y));
			int cmin = min(illumination.val[0], min(illumination.val[1], illumination.val[2]));
			int cmax = max(illumination.val[0], max(illumination.val[1], illumination.val[2]));
			float ill = (cmin + cmax) / 2;
			if (ill < IM1) {
				IM2++;
				sumIM2 += pow((ill - IM1), 2);
			}
			else if (ill > IM1) {
				IM3++;
				sumIM3 += pow((ill - IM1), 2);
			}
			sumIM4 += pow((ill - IM1), 2);
		}
	}

	IM2 = sqrt(sumIM2 / (IM2 - 1));
	IM3 = sqrt(sumIM3 / (IM3 - 1));
	IM4 = sqrt(sumIM4 / (imgCropped.total() - 1));
	 
	illuminationMeasures.push_back(IM1);
	illuminationMeasures.push_back(IM2);
	illuminationMeasures.push_back(IM3);
	illuminationMeasures.push_back(IM4);

	return illuminationMeasures;
}

std::string IlluminationAssessment::getIlluminationQuality(std::vector<float> mesurements)
/**
* Įvertinama nuotraukos kokybė pagal apsimokymo metu gautus rezultatus.
* Skaičiuojamas atstumas tarp nuotraukų vertinimo keturmatėje erdvėje.
*/
{
	std::string quality = "please show some exaples of even and uneven illumination";
	std::ifstream parameters;
	float a, b, c, d, l1 = FLT_MAX, l2 = 1;
	std::string s;
	parameters.open("./qualityParameters/parametersIllumination.txt");
	int i = 0;
	while (true && i != 10)
	{
		parameters >> a;
		parameters >> b;
		parameters >> c;
		parameters >> d;
		parameters >> s;
		l2 = sqrt(pow((a - mesurements[0]), 2) + pow((b - mesurements[1]), 2) + pow((c - mesurements[2]), 2) + pow((d - mesurements[3]), 2));
		if (l2 < l1) {
			l1 = l2;
			quality = s;
		}
		if (parameters.eof()) break;
		i++;
	}

	parameters.close();
	return quality;
}