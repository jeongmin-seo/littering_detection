#include "EventDetector.h"



CEventDetector::CEventDetector()
	: is_init_(false)
	, n_cur_frame_index_(0)
{
}


CEventDetector::~CEventDetector()
{
}


bool CEventDetector::Init()
{
	if (is_init_)
	{
		Terminate();
	}
	is_init_ = true;
	return is_init_;
}


bool CEventDetector::Terminate()
{
	if (!is_init_)
	{
		return true;
	}
	vec_mat_frame_buffer_.clear();
	vec_prev_detections_.clear();
	is_init_ = false;
	return !is_init_;
}


void CEventDetector::Run(const cv::Mat input_frame, const int frame_number)
{
	assert(is_init_);
	/*
		짜야하는 기능
		1) 입력 frame (input_frame)에 대한  Background subtraction 결과 만들기
			참고1: http://docs.opencv.org/trunk/d1/dc5/tutorial_background_subtraction.html
			참고2: http://study.marearts.com/2014/04/opencv-study-background-subtraction-and.html
		2) background subtraction으로부터 shadow 제거하고 box구하기 (참고2 참고)
		3) 구한 box로 CDetectedObject 객체의 vector 만들기 -> vec_prev_detections_에 저장
		4) 이전 frame의 vec_prev_detections를 이용하여, 현재 frame에서 추적하기
			참고3: https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
	*/
}

//()()
//('')HAANJU.YOO
