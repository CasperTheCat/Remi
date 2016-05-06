#include "Selector.h"

Selector::Selector() : bCatchError(false), mLayersTaken(0)
{
	//mPaths = new boost::filesystem::path[4];
	mPaths.resize(4);
	//
	hasIndex[0] = false;
	hasIndex[1] = false;
	hasIndex[2] = false;
	hasIndex[3] = false;
}

Selector::~Selector()
{
	//delete mPaths;
	if (mImageData) delete mImageData;
}

bool Selector::sContains(boost::filesystem::path _path, std::string pre)
{
	return (_path.string().find(pre) != std::string::npos);
}

void Selector::fillVec()
{


	int j;
	// Fill the other channels with data!
	for (int i = 0; i < input.size(); i++)
	{

		// Try to add to mPaths
		for (j = 0; j < 4; j++)
		{

			if (mLayersTaken > 4)
			{
				detectChannel();
				return;
			}


			if (!hasIndex[j] && input.size() != 0)
			{
				mPaths.at(j) = loadWithCV(input[i]);
				if (!mPaths.at(j).bSuccess) return;
				mLayersTaken += mPaths.at(j).layers;
				hasIndex[j] = true;
				std::cout << "Setting Item " << j << " to " << input[i].string() << std::endl;
				input.erase(input.begin() + i);
			}
		}
	}
}

void Selector::detectChannel()
{
	std::cout
		<< "\n\t**\t**\t**\n"
		<< "Too many layers... Dumping layer counts" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		if (mPaths.at(i).layers > 0) std::cout << "Item " << i << " has " << mPaths.at(i).layers << " layers" << std::endl;
	}
}


void Selector::iterateOverVec(std::string suffix, int forceIndex)
{
	if (mLayersTaken > 4)
	{
		detectChannel();
		return;
	}

	for (int i = 0; i < input.size(); i++)
	{
		if (sContains(input[i], suffix))
		{
			mPaths.at(forceIndex) = loadWithCV(input[i]);
			if (!mPaths.at(forceIndex).bSuccess) return;
			mLayersTaken += mPaths.at(forceIndex).layers;
			hasIndex[forceIndex] = true;
			std::cout << "Setting Item " << forceIndex << " to " << input[i].string() << std::endl;
			input.erase(input.begin() + i);
			return;
		}
	}
}

bool Selector::isValid()
{
	int itemsDeleted = 0;
	for (int i = 0; i < 4; i++)
	{
		if (!hasIndex[i]) mPaths.erase(mPaths.begin() + i - itemsDeleted++);
	}

	// Check the image dimensions
	bool bRollingCheck = true;
	for (int i = 0; i < mPaths.size() - 1; i++)
	{
		bRollingCheck = bRollingCheck &&
			(mPaths.at(i).height == mPaths.at(i + 1).height) &&
			(mPaths.at(i).width == mPaths.at(i + 1).width);
	}
	if (!bRollingCheck)
	{
		std::cout << "[ERROR] Image dimension mismatch" << std::endl;
	}

	// Lets also set up some things here
	mWidth = mPaths[0].width;
	mHeight = mPaths[0].height;

	return bRollingCheck;

}


bool Selector::processInput(std::vector<boost::filesystem::path> input)
{
	this->input = input;
	this->dupInput = input;
	iterateOverVec("_m.", 0);
	iterateOverVec("_s.", 1);
	iterateOverVec("_r.", 2);
	iterateOverVec("_a.", 3);
	fillVec();
	// check we are valid
	return isValid();
}

ImageData Selector::loadWithCV(boost::filesystem::path _path)
{
	cv::Mat image = cv::imread(_path.string(), CV_LOAD_IMAGE_UNCHANGED);
	if (!image.data) std::cout << "FAILURE TO OPEN" << std::endl;

	ImageData ret;
	ret.width = image.size().width;
	ret.height = image.size().height;
	ret.layers = image.channels();
	ret.bSuccess = image.data ? true : false;
	ret.imageData = new unsigned char[ret.height * ret.width * ret.layers];
	for (int i = 0; i < ret.height * ret.width * ret.layers; i++)
	{
		ret.imageData[i] = image.data[i];
	}

	return ret;
}

void Selector::packImage()
{
	// Get image dims

	uint64_t i2DSize = mWidth * mHeight;
	uint64_t iDim = i2DSize * mLayersTaken;

	// Init buffer
	mImageData = new unsigned char[iDim];

	// Take the first image and fill X channels with it
	int x, y, z;
	uint32_t workingLayers;
	int layersProcessed = 0, layersTemp;

	for (x = 0; x < mPaths.size(); x++)
	{
		workingLayers = mPaths[x].layers;
		layersTemp = layersProcessed;
		for (y = layersProcessed; y < workingLayers + layersTemp; y++)
		{
			for (z = 0; z < i2DSize; z++)
			{
				//std::cout << mLayersTaken << std::endl
				//	<< workingLayers << std::endl
				//	<< workingLayers + layersTemp << std::endl
				//	<< y << std::endl
				//	<< z + y * mLayersTaken << std::endl;
				mImageData[y + z * mLayersTaken] = mPaths[x].imageData[y - layersProcessed + z * workingLayers];
				//mImageData[z + y * mLayersTaken] = 255;
			}
			layersProcessed++;
		}
	}

}

void Selector::saveImage(std::string fName)
{
	cv::Mat outImage(mHeight, mWidth, CV_MAKETYPE(CV_8U, mLayersTaken));
	outImage.data = mImageData;
	cv::imwrite(fName, outImage);
}


