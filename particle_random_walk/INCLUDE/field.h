#pragma once

#include <cluster.h>
class Cluster;

enum APPROX_FIELD {
	AF_CONST,
	AF_LINEAR,
	AF_LOG,
	AF_REVERSE
};

// класс содержащий информацию о поле
class Field
{
private:
	int width;					// ширина пол€ в клетках
	int height;					// высота пол€ в клетках

	float** basic_potentials;	// матрица потенциалов пол€, созданных изначально

	float** potentials;			// текущее значение потенциалов пол€
	int** availables;			// матрица доступности €чейки пол€

	Field();					// hide default constructor

	APPROX_FIELD approx_type;

	float approx_p;
	float approx_q;
	float get_beta_by_lambda(float lambda, APPROX_FIELD aprox_type);

	void create_point_source_field();					// функци€ создани€ пол€ с точечным источником
	void hyperbolic_field(int num, float fi);	// функци€ создани€ гиперболического пол€
	float** create_hyperbolic_field(float fi, int width, int height);

public:
	// constructor & destructor
	Field(int _width, int _height, int num, float fi, APPROX_FIELD approx_type);
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
