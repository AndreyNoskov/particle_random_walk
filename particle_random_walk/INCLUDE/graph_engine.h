#pragma once

#include <opencv2\opencv.hpp>
#include <particle.h>
#include <field.h>
#include <string>
#include <cluster.h>
#include <source.h>


// ����� ��� ���������
class GraphEngine
{
private:
	int width;						// ������ ���� � ��������
	int height;						// ������ ���� � ��������

	cv::Mat image;					// �����������, ������� ����� ����������
	std::string winName;			// ��� ����

	cv::Scalar unavailable_color;	// ���� ����������� ����� - ������� ����
	cv::Scalar trace_color;			// ���� ���������� �������� �������
	cv::Scalar cluster_color;		// ���� ��������

public:
	GraphEngine(std::string _win_name, int winWidth, int winHeight);
	~GraphEngine();

	void process(Field* field, Particle* particle, Source* source = nullptr); // ���������� ������� ��������� ����
};