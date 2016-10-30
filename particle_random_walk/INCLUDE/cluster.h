#pragma once

#include <field.h>
class Field;

enum Cluster_elements{ CLUSTER_EMPTY, CLUSTER_INTERN, CLUSTER_PERIMETER };

class Cluster
{
private:
	float sigma;
	float beta;
	float radius;
	int width;
	int height;
	Field* field;
	float** cluster_field;
	int** cluster_elements;

public:
	Cluster(Field* field, float sigma, float beta, float radius);
	~Cluster();

	inline float get_potential(int x, int y) { return cluster_field[y][x]; }
	inline int get_element(int x, int y) { return cluster_elements[y][x]; }
	inline int get_width() { return width; }
	inline int get_height() { return height; }
	inline float get_sigma() { return sigma; }
	inline float get_beta() { return beta; }

	void add_element(int x, int y);
	void print_field();
	void print_elements();
	bool is_on_top();
};