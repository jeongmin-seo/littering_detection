#include "EventDetector.h"
#include "haanju_visualize.hpp"


CEventDetector::CEventDetector()
	: is_init_(false)
	, b_record_(false)
	, n_cur_frame_index_(0)
{
}


CEventDetector::~CEventDetector()
{
}


bool CEventDetector::Init(bool _b_record)
{
	if (is_init_)
		Terminate();

	is_init_ = true;
	pBGS_ = cv::createBackgroundSubtractorMOG2();

	vec_colors_ = hj::GenerateColors(400);

	b_record_ = _b_record;
	if (b_record_)
	{
		//cv::VideoWriter::VideoWriter();
		int fourcc = cv::VideoWriter::fourcc('F', 'M', 'P', '4');
		video_writer_.open("output.avi", fourcc, 15, cv::Size(1280, 720), true);
	}

	return is_init_;
}


bool CEventDetector::Terminate()
{
	if (!is_init_)
		return true;

	vec_mat_frame_buffer_.clear();
	vec_prev_detections_.clear();
	pBGS_->clear();
	is_init_ = false;
	if (b_record_ && video_writer_.isOpened())
		video_writer_.release();

	return !is_init_;
}


void CEventDetector::Run(const cv::Mat input_frame, const int frame_number)
{
	assert(is_init_);

	// backgroud subtraction
	cv::Mat binray_map;
	vec_curr_detections_ = BackgroundSubtraction(input_frame, n_cur_frame_index_, binray_map);
	
	// tracking
	vec_tracking_result_ = Tracking(input_frame, frame_number, vec_curr_detections_, vec_trackers_);
	
	// visualization
	Visualize(input_frame);

	// recording
	if (b_record_)
		video_writer_.write(mat_result_image_);

	vec_prev_detections_ = vec_curr_detections_;
}


std::vector<CDetectedObject> 
CEventDetector::BackgroundSubtraction(
	const cv::Mat _input_frame, 
	int _frame_number,
	cv::Mat &_binary_map)
{
	assert(is_init_);

	std::vector<CDetectedObject> detected_objects;
	cv::Mat fg_mask, blurred_image;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(3, 3));

	// background subtraction
	cv::blur(_input_frame, blurred_image, cv::Size(20, 20));
	pBGS_->apply(blurred_image, fg_mask);

	// morphology
	cv::morphologyEx(fg_mask, _binary_map, CV_MOP_CLOSE, element);
	cv::threshold(_binary_map, _binary_map, 128, 255, CV_THRESH_BINARY);  //Shadow delete, Binary

	// segmentation
	int new_id = 0;
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(_binary_map,
		contours,
		CV_RETR_EXTERNAL,       // retrieve the external contours
		CV_CHAIN_APPROX_NONE);  // all pixels of each contours
	for (std::vector< std::vector< cv::Point> >::iterator itc = contours.begin();
		itc != contours.end();
		itc++)
	{
		// validation on the rect size
		if (boundingRect(cv::Mat(*itc)).width < 2 || boundingRect(cv::Mat(*itc)).height < 2)
			continue;

		CDetectedObject new_object;
		new_object.id = new_id++;
		new_object.frame_index = _frame_number;
		new_object.bounding_box = boundingRect(cv::Mat(*itc));
		detected_objects.push_back(new_object);
	}

	return detected_objects;
}

std::vector<CTrackedObject>
CEventDetector::Tracking(
	const cv::Mat _input_frame,
	const int _frame_index,
	const std::vector<CDetectedObject> &_detections,
	std::vector<CTracker> &_trackers,
	int max_duration)
{
	std::vector<CTrackedObject> tracking_result;

	// track
	std::vector<CTracker> new_trackers;
	for (std::vector<CTracker>::iterator trackerIter = _trackers.begin();
		trackerIter != _trackers.end();
		trackerIter++)
	{
		// clear overlengthed trackers
		if (trackerIter->duration() >= max_duration)
			continue;

		// track and save
		CTrackedObject new_tracking_result;
		new_tracking_result.id = trackerIter->id();
		new_tracking_result.InsertBoundingBox(_frame_index, trackerIter->Update(_input_frame));
		tracking_result.push_back(new_tracking_result);
		new_trackers.push_back(*trackerIter);
	}

	// init with new detections
	int new_id = 0;
	for (int i = 0; i < _detections.size(); i++)
	{
		CTracker new_tracker;
		new_tracker.Init(_detections[i].bounding_box, _input_frame, new_id++);
		new_trackers.push_back(new_tracker);
	}

	_trackers = new_trackers;
	return tracking_result;
}


void CEventDetector::Visualize(const cv::Mat _input_frame)
{
	if (!mat_result_image_.empty())
		mat_result_image_.release();
	mat_result_image_ = _input_frame.clone();

	// draw detections
	for (std::vector<CDetectedObject>::iterator detIter = vec_curr_detections_.begin();
		detIter != vec_curr_detections_.end();
		detIter++)
	{
		cv::rectangle(mat_result_image_, detIter->bounding_box, cv::Scalar(0, 255, 255), 1, 8);
	}

	// draw tracking results
	for (std::vector<CTrackedObject>::iterator trackIter = vec_tracking_result_.begin();
		trackIter != vec_tracking_result_.end();
		trackIter++)
	{
		hj::DrawBoxWithID(mat_result_image_, trackIter->GetLastBoundingBox(), trackIter->id, 1, 0, &vec_colors_);
	}

	cv::namedWindow("result");
	cv::imshow("result", mat_result_image_);
}


//()()
//('')HAANJU.YOO

/*
nexttracker.resize(vec_prev_detections_.size());
if (!vec_prev_detections_.empty())
{
for (int prevDetectionSize = 0; prevDetectionSize < vec_prev_detections_.size();prevDetectionSize++)
{
nexttracker[prevDetectionSize].init(vec_prev_detections_[prevDetectionSize].bounding_box, input_frame);
cv::rectangle(input_frame, vec_prev_detections_[prevDetectionSize].bounding_box.tl(),
vec_prev_detections_[prevDetectionSize].bounding_box.br(), cv::Scalar(0, 255, 0), 1, 8);

}
}

if (!currtracker.empty())
{
for (int i = 0;i < currtracker.size();i++)
{
result = currtracker[i].update(input_frame);
cv::rectangle(input_frame, result.tl(), result.br(), cv::Scalar(0, 255, 255), 1, 8);
}
}

currtracker.clear();

currtracker.resize(nexttracker.size());
for (int i = 0; i < nexttracker.size();i++)
{
currtracker[i] = nexttracker[i];
}
nexttracker.clear();
*/
//***********************************
//
//if (buffDetectedObject.bounding_box.x + buffDetectedObject.bounding_box.width >= input_frame.size().width)
//{
//	buffDetectedObject.bounding_box.width = input_frame.size().width - buffDetectedObject.bounding_box.x - 1;
//}
//if (buffDetectedObject.bounding_box.y + buffDetectedObject.bounding_box.height >= input_frame.size().height)
//{
//	buffDetectedObject.bounding_box.height = input_frame.size().height - buffDetectedObject.bounding_box.y - 1;
//}
