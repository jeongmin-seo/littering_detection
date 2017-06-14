#pragma once

#include "opencv2/opencv.hpp"




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
		assert(vec_frame_indices.back() < frame_index);
		vec_frame_indices.push_back(frame_index);
		vec_bounding_boxes.push_back(bounding_box);
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

//()()
//('')HAANJU.YOO
