#pragma once

#include <field.h>
class Field;

// список возможных состояний ячеек поля, относительно кластера
enum Cluster_elements{
	CLUSTER_EMPTY,		// ячейка не занята кластером
	CLUSTER_INTERN,		// ячейка занята внутренним элеметом кластера
	CLUSTER_PERIMETER	// ячейка занята граничным элеметом кластера
};

// класс, описывающий кластер
class Cluster
{
private:
	float sigma;			// потенциал одного элемента кластера
	float beta;				// параметр, отвечающий за экранирование
	float radius;			// расстояние, но котором кластер влияет на поле затравок
	int width;				// ширина поля
	int height;				// высота поля
	Field* field;			// указатель на класс поля
	float** cluster_field;	// двумерный массив значений потенциалов, создаваемых кластером
	int** cluster_elements;	// двументрый массив состяний поля отностиельно кластера

public:
	Cluster(Field* field, float sigma, float beta, float radius);
	~Cluster();

	// getters
	inline float get_potential(int x, int y) { return cluster_field[y][x]; }
	inline int get_element(int x, int y) { return cluster_elements[y][x]; }
	inline int get_width() { return width; }
	inline int get_height() { return height; }
	inline float get_sigma() { return sigma; }
	inline float get_beta() { return beta; }

	void add_element(int x, int y); //добавить элемент в кластер
	void print_field();
	void print_elements();
	bool is_on_top(int top); // проверить добрался ли кластер до верхней границы
};