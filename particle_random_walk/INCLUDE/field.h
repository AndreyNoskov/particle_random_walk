#pragma once

#include <cluster.h>
class Cluster;

class Field
{
private:
	int width;
	int height;

	float** basic_potentials;

	float** potentials;
	int** availables;

	Field(); // hide default constructor

	// create potential field with point source
	void create_point_source_field();
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
