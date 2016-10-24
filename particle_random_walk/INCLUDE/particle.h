#pragma once

#include <vector>
#include <opencv2\opencv.hpp>

#include <Field.h>

class Particle
{
private:
	int xPos;
	int yPos;
	Field* field;
	bool isFinished;
	std::vector<cv::Point2i> trace;

public:
	// Constructor & destructor
	Particle(Field* _field);
	~Particle();

	// getters 
	inline int get_x_pos() { return xPos; }
	inline int get_y_pos() { return yPos; }
	inline bool is_finished() { return isFinished; }

	// actions
	cv::Point2i move();

};