#pragma once

#include <cluster.h>
class Cluster;

// ����� ���������� ���������� � ����
class Field
{
private:
	int width;					// ������ ���� � �������
	int height;					// ������ ���� � �������

	float** basic_potentials;	// ������� ����������� ����, ��������� ����������

	float** potentials;			// ������� �������� ����������� ����
	int** availables;			// ������� ����������� ������ ����

	Field();					// hide default constructor

	void create_point_source_field();					// ������� �������� ���� � �������� ����������
	void hyperbolic_field(float b, int num, float fi);	// ������� �������� ���������������� ����

public:
	// constructor & destructor
	Field(int _width, int _height, int num, float fi);
	~Field();

	// getters
	inline float get_potential(int x, int y) { return potentials[y][x]; }
	inline int   get_available(int x, int y) { return availables[y][x]; }
	inline int   get_width() { return width; }
	inline int   get_height() { return height; }

	void print_potentials();
	void print_availables();

	void update_field(Cluster* cluster);
};
