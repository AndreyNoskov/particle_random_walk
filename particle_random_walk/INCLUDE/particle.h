#pragma once

#include <vector>
#include <opencv2\opencv.hpp>

#include <field.h>
#include <cluster.h>

// ������ ��������� ��������� ������ ����
enum Cells 
{ 
	CELL_EMPTY,		// ������ �����
	CELL_BORDER,	// ������ ����������� ������� ����
	CELL_CLUSTER	// ������ ��������� ���������
};

//����� ����������� ������� �������������� �� ����
class Particle 
{
private:
	// ���������� ����� - ������ ��� ��������
	struct Pair {
		Pair(float _value, int _index) : value(_value), index(_index) {}
		int index;
		float value;
	};
	int xPos;						// ������� ������� �� ��� �
	int yPos;						// ������� ������� �� ��� Y
	Field* field;					// ���� ��� �������� ��������� �� ������ ����
	Cluster* cluster;				// ���� ��� �������� ��������� �� ������ ��������
	bool isFinished;				// ����-���������, ��������� �� ������� ��������
	int cluster_elements;			// ���������� �������� ��������� ��������, ����������� ��� �������������
	bool save_trace;				// ������� �� ���������� �������
	std::vector<cv::Point2i> trace;	//����-������, �������� ���������� �������

public:
	// Constructor & destructor
	Particle(Field* _field, 
			 Cluster* _cluster,
			 int cluster_elements,
			 bool save_trace);
	~Particle();

	// getters 
	inline int get_x_pos() { return xPos; }
	inline int get_y_pos() { return yPos; }
	inline bool is_finished() { return isFinished; }
	inline std::vector<cv::Point2i>* get_trace() { return &trace; }

	// actions
	cv::Point2i move();

};