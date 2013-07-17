// -*- c++ *--
// Copyright 2008 Isis Innovation Limited
//
// VideoSource.h
// Declares the VideoSource class
// 
// This is a very simple class to provide video input; this can be
// replaced with whatever form of video input that is needed.  It
// should open the video input on construction, and provide two
// function calls after construction: Size() must return the video
// format as an ImageRef, and GetAndFillFrameBWandRGB should wait for
// a new frame and then overwrite the passed-as-reference images with
// GreyScale and Colour versions of the new frame.
//
#include <cvd/image.h>
#include <cvd/byte.h>
#include <cvd/rgb.h>

#include <iostream>
#include <string>
#include <vector>

struct VideoSourceData;

class VideoSource
{
 public:
  VideoSource(bool FILE=false, double s=0);
  void GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB);
  /* Teesid Leelasawassuk: 27th April 2011 */
  void GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB, int &i);
  CVD::ImageRef Size();
  /* Teesid Leelasawassuk: 23rd April 2011 */
  void SetNameList(std::string path, bool backward=false);	// Will be called in System.cc at the initialized state
  std::vector<std::string> sNameList;
  int sFrameNumber;
 private:
  double n;
  double speed;
  bool file;
  void *mptr;
  CVD::ImageRef mirSize;
};
