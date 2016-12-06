#pragma once

#include <list>
#include <opencv2\opencv.hpp>

#include "field.h"

enum Source_Cell {
	SOURCE_EMPTY = 0,
	SOURCE_FULL = 1,
	SOURCE_PERIMETER = 2
};

class Source
{
private:
	std::list<cv::Point2i>	perimeter;
	Field*					field;
	Source_Cell**			source_array;
	int						height;
	int						width;

public:
	Source(Field* _field, int source_position);
	~Source();

	void			add_finished_point(cv::Point2i point);
	cv::Point2i		get_next_point();
	inline int		get_height() { return height; }
	inline int		get_width() { return width; }
	Source_Cell		get_value(int y, int x) { return source_array[y][x]; }
};