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


	//********************************************** 
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
		if (buffDetectedObject.bounding_box.width <= 0 || buffDetectedObject.bounding_box.height <= 0) { continue; }
		vec_curr_detections_.push_back(buffDetectedObject);
		
		cv::rectangle(input_frame, buffDetectedObject.bounding_box.tl(),buffDetectedObject.bounding_box.br(), CV_RGB(255, 0, 0),2);
		++itc;
		curr_id++;
	}
	//************************************************
	
	//*************************** tracking code 수정해야할 부분!
	tracker.resize(vec_prev_detections_.size());
	if(!vec_prev_detections_.empty())
	{
		for (int prevDetectionSize = 0; prevDetectionSize < vec_prev_detections_.size();prevDetectionSize++)
		{
			tracker[prevDetectionSize].init(vec_prev_detections_[prevDetectionSize].bounding_box,input_frame);
			cv::rectangle(input_frame, vec_prev_detections_[prevDetectionSize].bounding_box.tl(), 
				vec_prev_detections_[prevDetectionSize].bounding_box.br(), cv::Scalar(0, 255, 255), 1, 8);

		}
	}
	
	if (!tracker.empty())
	{
		for (int i = 0;i < tracker.size();i++)
		{
			result = tracker[i].update(input_frame);
			cv::rectangle(input_frame, result.tl(), result.br(), cv::Scalar(0, 255, 255), 1, 8);
		}
	}
	//***********************************
	cv::imshow("labeling", input_frame);
	
	
	tracker.clear();
	vec_prev_detections_.clear();
	SavePrevDetection();


}

//()()
//('')HAANJU.YOO
