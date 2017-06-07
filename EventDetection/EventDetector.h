/******************************************************************************
* Title        : CEventDetector
* Author       : Haanju Yoo
* Initial Date : 2017.06.07
* Version Num. : 0.9
* Description  : Detect event in interest
******************************************************************************/
#pragma once

#include "dku_types.hpp"


class CEventDetector
{
	//----------------------------------------------------------------
	// METHODS
	//----------------------------------------------------------------
public:
	CEventDetector();
	~CEventDetector();
	bool Init();
	bool Terminate();
	void Run(const cv::Mat input_frame, const int frame_number = -1);

protected:

	//----------------------------------------------------------------
	// VARIABLES
	//----------------------------------------------------------------
	bool is_init_;
	int n_cur_frame_index_;
	std::vector<cv::Mat> vec_mat_frame_buffer_;
	std::vector<CDetectedObject> vec_prev_detections_;
};

//()()
//('')HAANJU.YOO