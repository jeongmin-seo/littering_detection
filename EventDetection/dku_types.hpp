#pragma once

#include "opencv2/opencv.hpp"
#include "kcftracker.hpp"


enum OBJECT_CATEGORY 
{ 
	PEDESTRIAN = 0, 
	GARBAGE, 
	NUM_OBJECT_CATEGORY
};

class CDetectedObject
{
public:
	CDetectedObject() : id(0), category(PEDESTRIAN), frame_index(0), bounding_box(0, 0, 0, 0) {}
	~CDetectedObject() {}

	int id;
	OBJECT_CATEGORY category;
	int frame_index;
	cv::Rect2d bounding_box; //
};

class CTrackedObject
{
public:
	CTrackedObject() : id(0), category(PEDESTRIAN) {}
	~CTrackedObject() {}
	bool InsertBoundingBox(const int frame_index, const cv::Rect bounding_box)
	{
		assert(0 == vec_frame_indices.size() || vec_frame_indices.back() < frame_index);
		vec_frame_indices.push_back(frame_index);
		vec_bounding_boxes.push_back(bounding_box);
		return true;
	}
	cv::Rect GetBoundingBox(const int frame_index)
	{
		assert(vec_frame_indices.front() <= frame_index
			&& vec_frame_indices.back() >= frame_index);
		int found_index = -1;
		for (int i = 0; i < vec_frame_indices.size(); i++)
		{
			if (frame_index == vec_frame_indices[i])
			{
				found_index = i;
				return vec_bounding_boxes[i];
			}
		}
		assert(found_index > 0);
		return cv::Rect(0, 0, 0, 0);
	}
	cv::Rect GetLastBoundingBox() { return vec_bounding_boxes.back(); }
	int GetFirstFrameIndex() { return vec_frame_indices.front(); }
	int GetLastFrameIndex() { return vec_frame_indices.back(); }

public:
	int id;
	OBJECT_CATEGORY category;

protected:
	std::deque<int>      vec_frame_indices;
	std::deque<cv::Rect> vec_bounding_boxes;
};

class CTracker
{
public:
	CTracker() : id_(0), duration_(0) {}
	~CTracker() {}
	void Init(const cv::Rect _target_rect, const cv::Mat _frame_image, int _id)
	{
		cur_rect_ = _target_rect;
		tracking_object_.init(cur_rect_, _frame_image);
		b_init_ = true;
		id_ = _id;
		duration_ = 1;
	}
	cv::Rect Update(const cv::Mat _frame_image)
	{
		assert(b_init_);
		cur_rect_ = tracking_object_.update(_frame_image);
		duration_++;
		return cur_rect_;
	}
	int id() { return id_; }
	int duration() { return duration_; }
	cv::Rect GetRect() { return cur_rect_; }

protected:
	bool b_init_;
	int id_;
	int duration_;
	cv::Rect cur_rect_;
	KCFTracker tracking_object_;
};

//()()
//('')HAANJU.YOO
