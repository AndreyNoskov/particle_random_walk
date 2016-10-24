#pragma once

#include <opencv2\opencv.hpp>
#include <particle.h>
#include <field.h>
#include <string>

class GraphEngine
{
private:
	int width;
	int height;

	cv::Mat image;
	std::string winName;

	cv::Scalar unavailable_color;
	cv::Scalar trace_color;
public:
	GraphEngine(int winWidth, int winHeight);
	~GraphEngine();

	void process(Field* field, Particle* particle);
};