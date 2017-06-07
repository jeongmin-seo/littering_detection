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
}


bool CEventDetector::Terminate()
{
	if (!is_init_)
	{
		return true;
	}
}


void CEventDetector::Run(const cv::Mat input_frame, const int frame_number)
{
	/*
		¥���ϴ� ���
		1) �Է� frame (input_frame)�� ����  Background subtraction ��� �����
			����1: http://docs.opencv.org/trunk/d1/dc5/tutorial_background_subtraction.html
			����2: http://study.marearts.com/2014/04/opencv-study-background-subtraction-and.html
		2) background subtraction���κ��� shadow �����ϰ� box���ϱ� (����2 ����)
		3) ���� box�� CDetectedObject ��ü�� vector ����� -> vec_prev_detections_�� ����
		4) ���� frame�� vec_prev_detections�� �̿��Ͽ�, ���� frame���� �����ϱ�
			����3: https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
	*/
}

//()()
//('')HAANJU.YOO
