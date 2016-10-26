#pragma once

#include <opencv2\opencv.hpp>
#include <particle.h>
#include <field.h>
#include <string>
#include <cluster.h>

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
	void process_stage(Field* field, Cluster* cluster);
};