#include <iostream>
#include <boost/filesystem.hpp>
#include <queue>
#include "../functionality.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef REMI_SELECTOR_H
#define REMI_SELECTOR_H

struct ImageData
{
	unsigned char* imageData;
	uint32_t width;
	uint32_t height;
	int layers;
	bool bSuccess;
};

class Selector
{
	unsigned char* mImageData;

	uint32_t mWidth;
	uint32_t mHeight;

	std::vector<ImageData> mPaths;
	bool hasIndex[4];

	bool bCatchError;
	std::vector<boost::filesystem::path> input;
	std::vector<boost::filesystem::path> dupInput;

	int mLayersTaken;

	bool sContains(boost::filesystem::path _path, std::string pre);
	void iterateOverVec(std::string suffix, int fIndex);
	void fillVec();
	bool isValid();
	void detectChannel();
	ImageData loadWithCV(boost::filesystem::path _path);


public:
	Selector();
	~Selector();
	bool processInput(std::vector<boost::filesystem::path> input);
	void packImage();
	void saveImage(std::string fName);
};


#endif //REMI_SELECTOR_H
