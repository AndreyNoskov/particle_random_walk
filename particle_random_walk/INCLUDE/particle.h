#pragma once

#include <vector>
#include <opencv2\opencv.hpp>

#include <field.h>
#include <cluster.h>

// список возможных состояний клетки поля
enum Cells 
{ 
	CELL_EMPTY,		// клетка пуста
	CELL_BORDER,	// клетка принадлежит границе поля
	CELL_CLUSTER	// клетка заполнена кластером
};

//класс описывающий частицу перемещающуюся по полю
class Particle 
{
private:
	// внутренний класс - введен для удобства
	struct Pair {
		Pair(float _value, int _index) : value(_value), index(_index) {}
		int index;
		float value;
	};
	int xPos;						// позиция частицы по оси Х
	int yPos;						// позиция частицы по оси Y
	Field* field;					// поле для хранения указателя на объеки поля
	Cluster* cluster;				// поля для хранения указателя на объект кластера
	bool isFinished;				// поле-индикатор, завершила ли частица движение
	int cluster_elements;			// количество соседних элементов кластера, необходимых для присоединения
	bool save_trace;				// хранить ли траекторию частицы
	std::vector<cv::Point2i> trace;	//поле-вектор, хранящий траекторию частицы

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