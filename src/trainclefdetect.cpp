#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include <dirent.h>
#include <fstream>
#include "svmlight/svmlight.h"


using namespace std;
using namespace cv;


#define EXIT_FAILURE 1
#define TRAINHOG_SVM_TO_TRAIN SVMlight

static string posSamplesDir = "pos/";
static string negSamplesDir = "neg/";

static string featuresFile="features.dat";
static string svmModelFile = "svmlightmodel.dat";

static string cvHOGFile = "cvHOGClassifier.yaml";

static string descriptorVectorFile = "descriptorvector.dat";

static const Size trainingPadding = Size(0, 0);
static const Size winStride = Size(8, 8);


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

static void saveDescriptorVectorToFile(vector<float>& descriptorVector, vector<unsigned int>& _vectorIndices, string fileName) {
    printf("Saving descriptor vector to file '%s'\n", fileName.c_str());
    string separator = " "; // Use blank as default separator between single features
    fstream File;
    float percent;
    File.open(fileName.c_str(), ios::out);
    if (File.good() && File.is_open()) {
        printf("Saving %lu descriptor vector features:\t", descriptorVector.size());
        storeCursor();
        for (int feature = 0; feature < descriptorVector.size(); ++feature) {
            if ((feature % 10 == 0) || (feature == (descriptorVector.size()-1)) ) {
                percent = ((1 + feature) * 100 / descriptorVector.size());
                printf("%4u (%3.0f%%)", feature, percent);
                fflush(stdout);
                resetCursor();
            }
            File << descriptorVector.at(feature) << separator;
        }
        printf("\n");
        File << endl;
        File.flush();
        File.close();
    }
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

	if(imageData.cols!=hog.winSize.width || imageData.rows!=hog.winSize.height)
	{
		featureVector.clear();
		printf("Error: Image '%s' dimensions (%u x %u) do not match HOG window size (%u x %u)!\n", imageFilename.c_str(), imageData.cols, imageData.rows, hog.winSize.width, hog.winSize.height);
		return;
	}
	vector<Point> locations;
	hog.compute(imageData, featureVector, winStride, trainingPadding, locations);
	imageData.release();

}

static void showDetections(const vector<Point>& found, Mat& imageData) {
    size_t i, j;
    for (i = 0; i < found.size(); ++i) {
        Point r = found[i];
        // Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
        rectangle(imageData, Rect(r.x-16, r.y-32, 32, 64), Scalar(64, 255, 64), 3);
    }
}

static void showDetections(const vector<Rect>& found, Mat& imageData) {
    vector<Rect> found_filtered;
    size_t i, j;
    for (i = 0; i < found.size(); ++i) {
        Rect r = found[i];
        for (j = 0; j < found.size(); ++j)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    for (i = 0; i < found_filtered.size(); i++) {
        Rect r = found_filtered[i];
        rectangle(imageData, r.tl(), r.br(), Scalar(64, 255, 64), 3);
    }
}


static void detectTrainingSetTest(const HOGDescriptor& hog, const double hitThreshold, const vector<string>& posFileNames, const vector<string>& negFileNames) {
	unsigned int truePositives = 0;
	unsigned int trueNegatives = 0;
	unsigned int falsePositives = 0;
	unsigned int falseNegatives = 0;
	vector<Point> foundDetection;
	// Walk over positive training samples, generate images and detect
	for (vector<string>::const_iterator posTrainingIterator = posFileNames.begin(); posTrainingIterator != posFileNames.end(); ++posTrainingIterator) {
		const Mat imageData = imread(*posTrainingIterator, IMREAD_GRAYSCALE);
		hog.detect(imageData, foundDetection, hitThreshold, winStride, trainingPadding);
		if (foundDetection.size() > 0) 
		{
			++truePositives;
			falseNegatives += foundDetection.size() - 1;
		} 
		else 
		{
			++falseNegatives;
		}
	}
	// Walk over negative training samples, generate images and detect
	for (vector<string>::const_iterator negTrainingIterator = negFileNames.begin(); negTrainingIterator != negFileNames.end(); ++negTrainingIterator) {
	    const Mat imageData = imread(*negTrainingIterator, IMREAD_GRAYSCALE);
	    hog.detect(imageData, foundDetection, hitThreshold, winStride, trainingPadding);
	    if (foundDetection.size() > 0) {
	        falsePositives += foundDetection.size();
	    } else {
	        ++trueNegatives;
	    }        
	}

	printf("Results:\n\tTrue Positives: %u\n\tTrue Negatives: %u\n\tFalse Positives: %u\n\tFalse Negatives: %u\n", truePositives, trueNegatives, falsePositives, falseNegatives);
	}



static void detectTest(const HOGDescriptor& hog, const double hitThreshold, Mat& imageData) {
	vector<Rect> found;
	Size padding(Size(8, 8));
	Size winStride(Size(8, 8));
	hog.detectMultiScale(imageData, found, hitThreshold, winStride, padding);
	showDetections(found, imageData);
}



int main(int argc,char** argv)
{
	HOGDescriptor hog;
	hog.winSize = Size(64, 128);

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
			if(!featureVector.empty())
			{
				File << ((currentFile < positiveTrainingImages.size()) ? "+1" : "-1");
				for(int feature=0;feature<featureVector.size();feature++)
				{
					File << " " << (feature + 1) << ":" << featureVector.at(feature);	
				}
				File<<endl;
			}
		}
		printf("\n");
        File.flush();
        File.close();
	}
	else
	{
		printf("Error Opening File '%s'!\n", featuresFile.c_str());
		return EXIT_FAILURE
	}

	printf("Calling %s\n", TRAINHOG_SVM_TO_TRAIN::getInstance()->getSVMName());
	TRAINHOG_SVM_TO_TRAIN::getInstance()->read_problem(const_cast<char*> (featuresFile.c_str()));
	TRAINHOG_SVM_TO_TRAIN::getInstance()->train(); // Call the core libsvm training procedure
	printf("Training done, saving model file!\n");
	TRAINHOG_SVM_TO_TRAIN::getInstance()->saveModelToFile(svmModelFile);

	printf("Generating representative single HOG feature vector using svmlight!\n");
	vector<float> descriptorVector;
	vector<unsigned int> descriptorVectorIndices;

	TRAINHOG_SVM_TO_TRAIN::getInstance()->getSingleDetectingVector(descriptorVector, descriptorVectorIndices);

	saveDescriptorVectorToFile(descriptorVector, descriptorVectorIndices, descriptorVectorFile);

	const double hitThreshold = TRAINHOG_SVM_TO_TRAIN::getInstance()->getThreshold();

	hog.setSVMDetector(descriptorVector);
	hog.save(cvHOGFile);

	printf("Testing training phase using training set as test set (just to check if training is ok - no detection quality conclusion with this!)\n");
	detectTrainingSetTest(hog, hitThreshold, positiveTrainingImages, negativeTrainingImages);

	printf("Testing custom detection using camera\n");



}