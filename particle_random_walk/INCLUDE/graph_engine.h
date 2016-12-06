#pragma once

#include <opencv2\opencv.hpp>
#include <particle.h>
#include <field.h>
#include <string>
#include <cluster.h>
#include <source.h>


// класс для отрисовки
class GraphEngine
{
private:
	int width;						// ширина окна в пикселях
	int height;						// высота окна в пикселях

	cv::Mat image;					// изображение, которое будет отрисовано
	std::string winName;			// имя окна

	cv::Scalar unavailable_color;	// цвет недоступных ячеек - граница поля
	cv::Scalar trace_color;			// цвет траектории движения частицы
	cv::Scalar cluster_color;		// цвет кластера

public:
	GraphEngine(std::string _win_name, int winWidth, int winHeight);
	~GraphEngine();

	void process(Field* field, Particle* particle, Source* source = nullptr); // отрисовать текущее состояние поля
};