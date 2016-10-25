#pragma once

class Cluster
{
private:
	int width;
	int height;
	float** cluster_field;
public:
	inline float get_potential(int x, int y) { return cluster_field[y][x]; }
	inline int get_width() { return width; }
	inline int get_height() { return height; }
};