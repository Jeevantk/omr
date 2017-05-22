#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include <dirent.h>
#include <fstream>
#include "svmlight/svmlight.h"


using namespace std;
using namespace cv;


static string featuresFile="features.dat";

static string toLowerCase(const string& in) {
    string t;
    for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
        t += tolower(*i);
    }
    return t;
}

static void storeCursor(void) {
    printf("\033[s");
}

static void resetCursor(void) {
    printf("\033[u");
}


static void getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions) {
    printf("Opening directory %s\n", dirName.c_str());
    DIR *dir;
    struct dirent *ent;
    size_t extensionLocation;
    if((dir=opendir(dirName.c_str())))
    {
    	//Read all files in that directory.
    	while((ent=readdir(dir))!=NULL)
    	{
    		extensionLocation = string(ep->d_name).find_last_of(".");
			string tempExt = toLowerCase(string(ep->d_name).substr(extensionLocation + 1));
			if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end())
			{
				printf("Found matching data file '%s'\n", ep->d_name);
				fileNames.push_back((string) dirName + ep->d_name);
			}
			else
			{
				printf("Found file does not match required file type, skipping: '%s'\n", ep->d_name);
            }

    	}
    	(void) closedir(dp);

    }
    else 
    {
		printf("Error opening directory '%s'!\n", dirName.c_str());
    }
	return;

}

static void calculateFeaturesFromInput(const string& imageFilename,vector<float> &featureVector,HOGDescriptor& hog)
{
	Mat imageData=imread(imageFilename,0);
	if(imageData.empty())
	{
		featureVector.clear();
		printf("Error Image %s is empty ,feature calculation skipped\n",imageFilename.c_str());
		return;
	}
	
}


int main(int argc,char** argv)
{
	HOGDescriptor hog;

	static vector<string> positiveTrainingImages;
	static vector<string> negativeTrainingImages;
	static vector<string> validExtensions;
	validExtensions.push_back("jpg");
	validExtensions.push_back("png");
	validExtensions.push_back("jpeg")
	validExtensions.push_back("ppm");
	getFilesInDirectory(posSamplesDir, positiveTrainingImages, validExtensions);
	getFilesInDirectory(negSamplesDir, negativeTrainingImages, validExtensions);

	unsigned long overallSamples = positiveTrainingImages.size() + negativeTrainingImages.size();

	if (overallSamples == 0) {
		printf("No training sample files found, nothing to do!\n");
		return EXIT_SUCCESS;
	}

	printf("Reading files, generating HOG features and save them to file '%s':\n", featuresFile.c_str());
	float percent;

	fstream File;
	File.open(featuresFile.c_str(), ios::out);
	if(file.good() && file.is_open())
	{
		File << "# Use this file to train, e.g. SVMlight by issuing $ svm_learn -i 1 -a weights.txt " << featuresFile.c_str() << endl;
		for(int currentFile=0;currentFile<overallSamples;currentFile++)
		{
			storeCursor(); // Can be used to print over stuff already written
			vector<float> featureVector;
			const string currentImageFile=(currentFile < positiveTrainingImages.size() ? positiveTrainingImages.at(currentFile):negativeTrainingImages.at(currentFile-positiveTrainingImages.size()))
			if((currentFile+1)%10==0 || (currentFile+1)==overallSamples)
			{
				percent = ((currentFile+1) * 100 / overallSamples);
				printf("%d (%3.0f%%):\tFile '%s'", (currentFile+1), percent, currentImageFile.c_str());
				fflush(stdout);
                resetCursor();
			}

			calculateFeaturesFromInput(currentImageFile, featureVector, hog);
		}
	}




}