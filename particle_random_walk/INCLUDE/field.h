#pragma once

#include <cluster.h>
class Cluster;

// класс содержащий информацию о поле
class Field
{
private:
	int width; // ширина поля в клетках
	int height; // высота поля в клетках

	float** basic_potentials; // матрица потенциалов поля, созданных изначально

	float** potentials; // текущее значение потенциалов поля
	int** availables; // матрица доступности ячейки поля

	Field(); // hide default constructor

	// create potential field with point source
	void create_point_source_field(); // функция создания поля
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
