#include <particle.h>
#include <misc.h>

#include <iostream>
#include <ctime>

Particle::Particle(Field* _field, Cluster* _cluster)
{
	field = _field;
	cluster = _cluster;
	srand((unsigned int)time(0));
	xPos = 1 + (rand() % (field->get_width() - 2));
	yPos = 1;
	trace.push_back(cv::Point2i(xPos, yPos));
	isFinished = false;
}

Particle::~Particle()
{

}

cv::Point2i Particle::move()
{
	float potentials[9] = {0};
	float cur = field->get_potential(xPos, yPos);
	for (int j = -1; j < 2; ++j)
		for (int i = -1; i < 2; ++i)
			if (((i == 0) && (j == 0)) || (field->get_available(xPos + i, yPos + j) != CELL_EMPTY))
				potentials[(j + 1) * 3 + (i + 1)] = 0;
			else
				potentials[(j + 1) * 3 + (i + 1)] = field->get_potential(xPos + i, yPos + j);
	float sum = 0;
	for (int i = 0; i < 9; ++i)
		if (potentials[i] != 0)
			sum += abs(potentials[i]-cur);
	for (int i = 0; i < 9; ++i)
		if (potentials[i] != 0)
			potentials[i] = abs(potentials[i] - cur) / sum;
	int destination = num_from_distribution(potentials, 9);
	xPos += (destination % 3) - 1;
	yPos += (int)(destination / 3) - 1;
	cv::Point2i point = cv::Point2i(xPos, yPos);
	trace.push_back(point);
	if (yPos == field->get_width() - 2)
		isFinished = true;
	return point;
}