#include "FocusAssessment.h"
#include "SingletonUtilities.h"
#include "ColourAssessment.h"
#include "IlluminationAssessment.h"
#include "ContrastAssesment.h"

void SetImagesNamesAndQuality(std::vector<std::vector<std::string>>*);
int main()
{
	std::vector< std::vector< int > > myVector({ { 3,4,3 },{ 2,5,2 },{ 1,6,1 },{ 5,6,1 } });
	std::sort(myVector.begin(), myVector.end(), [](const std::vector< int >& a, const std::vector< int >& b) { return a[0] < b[0]; });

	std::cout << "{";
	for (auto i : myVector) {
		std::cout << "[";
		for (auto j : i)
			std::cout << j << ",";
		std::cout << "],";
	}
	std::cout << "}" << std::endl;


	std::vector<std::vector<std::string>> namesAndQuality;
	SetImagesNamesAndQuality(&namesAndQuality);

	/*ContrastAssesment* objContrastAssessment = new ContrastAssesment(namesAndQuality);
	std::vector<float> contrastQuality = objContrastAssessment->GetContrastQuality("images/image_normal.jpg");
	delete objContrastAssessment;

	std::cout << "Contrast quality measures of image are:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << contrastQuality[i];
	}
	std::cout << std::endl;*/


	// This constructor sets classifier data automatically
	ContrastAssesment* objContrastAssessment = new ContrastAssesment(namesAndQuality);
	std::vector<float> contrastQuality = objContrastAssessment->GetContrastQuality("images/image_good.jpg");
	delete objContrastAssessment;

	std::cout << "Contrast measures of image are:";
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << contrastQuality[i];
	}
	std::cout << "\nContrast quality of image is ";
	contrastQuality[4] == o.good ? printf("good") : contrastQuality[4] == o.normal ? printf("normal") : printf("bad");
	std::cout << std::endl;

	// FOR DEBUGGING
	/*std::cout << "Images paths are:\n";
	for (int i = 0; i < namesAndQuality.size(); i++)
	{
		std::cout << namesAndQuality[i][o.path] << " " << namesAndQuality[i][o.quality] << std::endl;
	}
	std::cout << std::endl;*/

	// This constructor sets classifier data automatically
	FocusAssessment* objFocusAssessment = new FocusAssessment(namesAndQuality);
	std::vector<float> focusQuality = objFocusAssessment->GetFocusQuality("images/image_bad.jpg");
	delete objFocusAssessment;

	std::cout << "Focus measures of image are:";
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << focusQuality[i];
	}
	std::cout << "\nFocus quality of image is ";
	focusQuality[3] == o.good ? printf("good") : focusQuality[3] == o.normal ? printf("normal") : printf("bad");
	std::cout << std::endl;
	
	ColourAssessment* objColourAssessment = new ColourAssessment(namesAndQuality);
	std::vector<float> colourQuality = objColourAssessment->GetColourQuality("images/image_good.jpg");
	delete objColourAssessment;

	IlluminationAssessment* objIlluminationAssessment = new IlluminationAssessment(namesAndQuality);
	std::vector<float> illuminationQuality = objIlluminationAssessment->GetIlluminationQuality("images/image_good.jpg");
	delete objIlluminationAssessment;
	
	std::cout << "Quality measures of image are:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << contrastQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << focusQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << " " << colourQuality[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << " " << illuminationQuality[i];
	}
	std::cout << std::endl;

	cv::waitKey(0);
	return(0);
}

void SetImagesNamesAndQuality(std::vector<std::vector<std::string>>* namesAndQuality)
{
	std::vector<std::string> tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/good");
	size_t sizeGood = tempVector.size();
	for (int i = 0; i < sizeGood; i++)
	{
		namesAndQuality->push_back({ "images/good/"+tempVector[i], "good" });
	}


	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/normal");
	size_t sizeNormal = tempVector.size();
	for (int i = 0; i < sizeNormal; i++)
	{
		namesAndQuality->push_back({ "images/normal/"+tempVector[i], "normal" });
	}

	tempVector = SingletonUtilities::Instance()->GetFilesNamesInFolder("images/bad");
	size_t sizeBad = tempVector.size();
	for (int i = 0; i < sizeBad; i++)
	{
		namesAndQuality->push_back({ "images/bad/"+tempVector[i], "bad" });
	}
}