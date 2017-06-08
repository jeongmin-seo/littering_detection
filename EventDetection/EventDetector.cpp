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
	m_pBGS = cv::createBackgroundSubtractorMOG2();
	
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

void CEventDetector::SavePrevDetection()
{
	//현재 찾은 box들 prev에 저장.
	for (int i = 0;i < vec_curr_detections_.size();i++)
		vec_prev_detections_.push_back(vec_curr_detections_[i]);
	
	vec_curr_detections_.clear();
}
//
//void CEventDetector::DetectionCurrFrame(cv::Mat contourImg, int index)
//{
//	CDetectedObject buffDetectedObject;
//	std::vector<std::vector<cv::Point>> contours;
//	cv::findContours(contourImg,
//		contours,
//		CV_RETR_EXTERNAL,       // retrieve the external contours
//		CV_CHAIN_APPROX_NONE);  // all pixels of each contours
//
//	std::vector< std::vector< cv::Point> >::iterator itc = contours.begin();
//	int curr_id = 0;
//	while (itc != contours.end()) {
//		//Create bounding rect of object
//		//rect draw on origin image 
//		buffDetectedObject.id = curr_id;
//		buffDetectedObject.frame_index = index;
//		buffDetectedObject.bounding_box = boundingRect(cv::Mat(*itc));
//		vec_curr_detections_.push_back(buffDetectedObject);
//
//		//cv::rectangle(input_frame, buffDetectedObject.bounding_box.tl(), buffDetectedObject.bounding_box.br(), CV_RGB(255, 0, 0), 2);
//		++itc;
//		curr_id++;
//	}
//}

void CEventDetector::Run(const cv::Mat input_frame, const int frame_number)
{
	assert(is_init_);

	CDetectedObject buffDetectedObject;
	cv::Mat fgMaskMOG2, binaryImg;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(3, 3));
	
	
	cv::blur(input_frame, input_frame, cv::Size(20, 20));
	m_pBGS->apply(input_frame, fgMaskMOG2);
	cv::morphologyEx(fgMaskMOG2, binaryImg, CV_MOP_CLOSE, element);
	cv::threshold(binaryImg, binaryImg, 128, 255, CV_THRESH_BINARY);  //Shadow delete, Binary
	//cv::imshow("FG Mask MOG 2", binaryImg);


	//********************************************** 멤버함수로 빠질 수 있을까!
	//contourImg = binaryImg.clone();
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(binaryImg,
					contours,
					CV_RETR_EXTERNAL,       // retrieve the external contours
					CV_CHAIN_APPROX_NONE);  // all pixels of each contours

	std::vector< std::vector< cv::Point> >::iterator itc = contours.begin();
	int curr_id = 0;
	while (itc != contours.end()) {
		//Create bounding rect of object
		//rect draw on origin image 
		buffDetectedObject.id = curr_id;
		buffDetectedObject.frame_index = frame_number;
		buffDetectedObject.bounding_box = boundingRect(cv::Mat(*itc));
		vec_curr_detections_.push_back(buffDetectedObject);
		
		cv::rectangle(input_frame, buffDetectedObject.bounding_box.tl(),buffDetectedObject.bounding_box.br(), CV_RGB(255, 0, 0),2);
		++itc;
		curr_id++;
	}
	//************************************************

	SavePrevDetection();
	cv::imshow("labeling", input_frame);


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
