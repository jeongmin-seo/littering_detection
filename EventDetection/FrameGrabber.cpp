#include "FrameGrabber.h"



CFrameGrabber::CFrameGrabber()
	: is_capture_opened_(false)
	, cur_frame_index_(0)
{
}


CFrameGrabber::~CFrameGrabber()
{
	Terminate();
}


bool CFrameGrabber::Init(const cv::String& filename)
{
	if (is_capture_opened_) { Terminate(); }

	// open video capture
	video_capture_.open(filename);
	is_capture_opened_ = video_capture_.isOpened();
	if (!is_capture_opened_) { return false; }
	cur_frame_index_ = -1;
	return is_capture_opened_;
}


bool CFrameGrabber::Terminate()
{
	if (is_capture_opened_)
	{
		video_capture_.release();
		is_capture_opened_ = false;
	}
	return !is_capture_opened_;
}


cv::Mat CFrameGrabber::GetFrame()
{
	cv::Mat mat_cur_frame;
	video_capture_ >> mat_cur_frame;
	if (!mat_cur_frame.empty())
	{
		cur_frame_index_++;
	}
	return mat_cur_frame;
}


//()()
//('')HAANJU.YOO
