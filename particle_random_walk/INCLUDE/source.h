#pragma once

#include <list>
#include <opencv2\opencv.hpp>

#include "field.h"

class Source
{
private:
	std::list<cv::Point2i> perimeter;
	Field* field;
	int** source_array;
	int height;

public:
	Source(Field* _field, size_t source_position);
	~Source();

	void add_finished_point(cv::Point2i point);
	cv::Point2i get_next_point();
};