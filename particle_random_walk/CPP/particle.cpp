#include <particle.h>
#include <misc.h>

#include <iostream>
#include <ctime>

Particle::Particle(Field* _field, Cluster* _cluster,
				   int _cluster_elements = 1, bool _save_trace = true)
{
	// присваиваем исходные значения
	cluster_elements = _cluster_elements;
	save_trace = _save_trace;
	field = _field;
	cluster = _cluster;

	// частица генерируется на верхней границе поля
	xPos = 1 + (rand() % (field->get_width() - 2));
	yPos = 1;

	// записываем первую координату
	if (save_trace)
		trace.push_back(cv::Point2i(xPos, yPos));

	// пока не закончили двигаться
	isFinished = false;
}

Particle::~Particle()
{

}

cv::Point2i Particle::move()
{
	// вектор для хранения возможных направлений и вероятностей перехода
	std::vector<Pair> distribution;
	distribution.reserve(9); // резервируем до 9 элементов (оптимизация)
	float cur = field->get_potential(xPos, yPos); // значение потенциала в текущей ячейке

	// ищем возможные направления перехода
	for (int j = -1; j < 2; ++j)
		for (int i = -1; i < 2; ++i)
		{
			bool condition = ((i == 0) && (j == 0)) ||
				(field->get_available(xPos + i, yPos + j) != CELL_EMPTY);
			if (!condition)
			{
				float potential = field->get_potential(xPos + i, yPos + j);
				Pair pair(potential, (j + 1) * 3 + (i + 1));
				distribution.push_back(pair);
			}
		}
				
	// определяем вероятности перехода в ячейки
	float sum = 0;
	for (int i = 0; i < distribution.size(); ++i)
		sum += abs(distribution[i].value - cur);
	
	for (int i = 0; i < distribution.size(); ++i)
		distribution[i].value = abs(distribution[i].value - cur) / sum;
	
	// интегральные вероятности перехода
	if (distribution.size() > 1)
		for (int i = 1; i < distribution.size(); ++i)
			distribution[i].value += distribution[i - 1].value;

	// выбираем ячейку для перехода (почти метод Монте-Карло)
	float generated = (float)rand() / (float)RAND_MAX;
	int destination = 0;
	for (int i = 0; i < distribution.size(); ++i)
		if (generated < distribution[i].value || (i == distribution.size() - 1))
		{
			destination = distribution[i].index;
			break;
		}

	// записываем новые координаты частицы
	xPos += (destination % 3) - 1;
	yPos += (int)(destination / 3) - 1;
	cv::Point2i point = cv::Point2i(xPos, yPos);

	// добавим координату в траекторию
	if (save_trace)
		trace.push_back(point);

	// проверим, не попала ли частица в кластер
	int near_cluster = 0;
	for (int j = -1; j < 2; ++j)
		for (int i = -1; i < 2; ++i)
			if (field->get_available(xPos + i, yPos + j) == CELL_CLUSTER)
				near_cluster++;

	// если попала - закончим работу с частицей и добавим ее в кластер
	if (near_cluster >= cluster_elements) 
	{
		cluster->add_element(xPos, yPos);
		isFinished = true;
	}

	// если частица достигла нижней границы - добавим в кластер 
	if (yPos == field->get_height() - 2)
		isFinished = true;
	return point;
}