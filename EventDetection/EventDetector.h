/******************************************************************************
* Title        : CEventDetector
* Author       : Haanju Yoo
* Initial Date : 2017.06.07
* Version Num. : 0.9
* Description  : Detect event in interest
******************************************************************************/
#pragma once

#include "opencv2\videoio.hpp"
#include "dku_types.hpp"
#include "DetectorCrosstalk.hpp"

class CEventDetector
{
	//----------------------------------------------------------------
	// METHODS
	//----------------------------------------------------------------
public:
	CEventDetector();
	~CEventDetector();
	bool Init(bool _b_record = false);
	bool Terminate();
	void Run(const cv::Mat input_frame, const int frame_number = -1);

protected:
	std::vector<CDetectedObject> BackgroundSubtraction(
		const cv::Mat _input_frame, 
		int _frame_number, 
		cv::Mat &_binary_map);
	std::vector<CTrackedObject> Tracking(
		const cv::Mat _input_frame,
		const int _frame_index,
		const std::vector<CDetectedObject> &_detections, 
		std::vector<CTracker> &_trackers,
		int max_duration = 2);

	void Visualize(const cv::Mat _input_frame);

	//----------------------------------------------------------------
	// VARIABLES
	//----------------------------------------------------------------
	bool is_init_;
	bool b_record_;
	int n_cur_frame_index_;
	std::vector<cv::Mat> vec_mat_frame_buffer_;

	// background subtraction
	cv::Ptr<cv::BackgroundSubtractor> pBGS_;
	std::vector<CDetectedObject> vec_prev_detections_;
	std::vector<CDetectedObject> vec_curr_detections_;

	// object detector
	CDetectorCrosstalk object_detector_;

	// tracking
	std::vector<CTracker> vec_trackers_;
	std::vector<CTrackedObject> vec_tracking_result_;
	cv::Rect result;

	// visualization
	std::vector<cv::Scalar> vec_colors_;
	cv::Mat mat_result_image_;

	// record
	cv::VideoWriter video_writer_;
};

//()()
//('')HAANJU.YOO
