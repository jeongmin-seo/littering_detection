/******************************************************************************
* Title        : CFrameGrabber
* Author       : Haanju Yoo
* Initial Date : 2017.06.07
* Version Num. : 0.9
* Description  : Grabbing frame image
******************************************************************************/
#pragma once

#include "opencv2/opencv.hpp"


class CFrameGrabber
{
	//----------------------------------------------------------------
	// METHODS
	//----------------------------------------------------------------
public:
	CFrameGrabber();
	~CFrameGrabber();

	bool Init(const cv::String& filename);
	bool Terminate();
	cv::Mat GetFrame();
	int GetCurFrameIndex() { return cur_frame_index_; }

	//----------------------------------------------------------------
	// VARIABLES
	//----------------------------------------------------------------
protected:
	bool is_capture_opened_;
	int cur_frame_index_;
	cv::VideoCapture video_capture_;
};

//()()
//('')HAANJU.YOO
