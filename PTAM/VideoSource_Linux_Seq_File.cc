/*
* Autor : Teesid Leelasawassuk
* This file allows to use sequence of images as the video source input for PTAM
*
*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "VideoSource.h"
//#include <cvd/Linux/v4lbuffer.h>
#include <cvd/colourspace_convert.h>
#include <cvd/colourspaces.h>
#include <cvd/image_io.h>
#include <gvars3/instances.h>

#include <cvd/Linux/dvbuffer3.h>

using namespace CVD;
using namespace std;
using namespace GVars3;

#define IMAGE_W 640
#define IMAGE_H 480

/* Read filename contain in directory */
VideoSource::VideoSource(bool FILE, double s)
{
	n = 0;
	speed = s;
	file = FILE;

	if (!file)
	{
		cout << "  VideoSource_Linux: Opening video source..." << endl;
		DVBuffer3<yuv411>* pvb= new DVBuffer3<yuv411>();
		mptr = pvb;
		mirSize = pvb->size();
		cout << "  ... got video source." << endl;
	}
	else
	{
		cout << "  VideoSource_Linux: Reading image sequence..." << endl;
		mirSize = ImageRef(IMAGE_W, IMAGE_H);
	}
};

ImageRef VideoSource::Size()
{ 
	return mirSize;
};

void VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW, Image<Rgb<byte> > &imRGB)
{
	if (!file)
	{
		DVBuffer3<yuv411>* pvb = (DVBuffer3<yuv411>*) mptr;
		VideoFrame<yuv411> *pVidFrame = pvb->get_frame();
		convert_image(*pVidFrame, imBW);
		convert_image(*pVidFrame, imRGB);
		pvb->put_frame(pVidFrame);
	}
	else
	{
		if (sFrameNumber == (int)sNameList.size())
		{
			n = 0;
			sFrameNumber = 0;
		}
		// Read the image file
		Image<byte>	sBW;
		Image<Rgb<byte> > sRGB;
		sBW = img_load(sNameList[sFrameNumber]);
		sRGB = img_load(sNameList[sFrameNumber]);
		// Convert and return
		imBW.copy_from(sBW);
		imRGB.copy_from(sRGB);
		// Frame number increment
		//sFrameNumber++;
		n += speed;
		sFrameNumber = (int)n;
	}
}
/* Teesid Leelasawassuk: 27th April 2011*/
void VideoSource::GetAndFillFrameBWandRGB(Image<byte> &imBW, Image<Rgb<byte> > &imRGB, int &i)
{
	if (!file)
	{
		DVBuffer3<yuv411>* pvb = (DVBuffer3<yuv411>*) mptr;
		VideoFrame<yuv411> *pVidFrame = pvb->get_frame();
		convert_image(*pVidFrame, imBW);
		convert_image(*pVidFrame, imRGB);
		pvb->put_frame(pVidFrame);
	}
	else
	{
		if (sFrameNumber == (int)sNameList.size())
		{
			n = 0;
			sFrameNumber = 0;
		}
		// Read the image file
		Image<byte>	sBW;
		Image<Rgb<byte> > sRGB;
		sBW = img_load(sNameList[sFrameNumber]);
		sRGB = img_load(sNameList[sFrameNumber]);
		// Convert and return
		imBW.copy_from(sBW);
		imRGB.copy_from(sRGB);
		i = sFrameNumber;
		// Frame number increment
		n += speed;
		sFrameNumber = (int)n;
	}
}

void VideoSource::SetNameList(string path, bool backward)
{
	ifstream file;
	string dir, filepath;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;

	cout << "	VideoSource_Linux_Seq_File: Opening image sequence..." << endl;
	dir = path;
	cout << " Image sequence path: " << dir << endl;
	dp = opendir(dir.c_str());
	if (dp == NULL)
	{
		cout << "Error(" << errno << ") opening" << dir << endl;
		exit(-1);
	}
	sFrameNumber = 0;
	sNameList.clear();
	cout << "Start reading image sequence" << endl;
	cout << "Source is " << dir << endl;
	// List of image filenames
	sNameList.clear();
	while ((dirp = readdir(dp)))
	{
		filepath = dir + "/" + dirp->d_name;

		if (stat(filepath.c_str(), &filestat)) continue;
		if (S_ISDIR(filestat.st_mode)) continue;

		sNameList.push_back(filepath.c_str());
	}
	closedir(dp);
	std::sort(sNameList.begin(), sNameList.end());

	// Add backward sequence to the list
	if (backward)
	{
		std::vector<std::string> bList;
		bList = sNameList;

		std::reverse(bList.begin(), bList.end());
		sNameList.insert(sNameList.end(),bList.begin(),bList.end());
	}
}
