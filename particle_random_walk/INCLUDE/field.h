#pragma once

#include <cluster.h>
class Cluster;

// класс содержащий информацию о поле
class Field
{
private:
	int width; // ширина пол€ в клетках
	int height; // высота пол€ в клетках

	float** basic_potentials; // матрица потенциалов пол€, созданных изначально

	float** potentials; // текущее значение потенциалов пол€
	int** availables; // матрица доступности €чейки пол€

	Field(); // hide default constructor

	// create potential field with point source
	void create_point_source_field(); // функци€ создани€ пол€ с точечным источником
	void hyperbolic_field(float b, int num); // функци€ создани€ гиперболического пол€
public:
	// constructor & destructor
	Field(int _width, int _height);
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
