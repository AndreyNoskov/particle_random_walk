#include <particle.h>
#include <misc.h>

#include <iostream>
#include <ctime>

Particle::Particle(Field* _field, Cluster* _cluster, int _cluster_elements = 1)
{
	cluster_elements = _cluster_elements;
	field = _field;
	cluster = _cluster;
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
	std::vector<Pair> distribution;
	distribution.reserve(9);
	float cur = field->get_potential(xPos, yPos);
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
				
	float sum = 0;
	for (int i = 0; i < distribution.size(); ++i)
		sum += abs(distribution[i].value - cur);
	
	for (int i = 0; i < distribution.size(); ++i)
		distribution[i].value = abs(distribution[i].value - cur) / sum;
	
	if (distribution.size() > 1)
		for (int i = 1; i < distribution.size(); ++i)
			distribution[i].value += distribution[i - 1].value;

	float generated = (float)rand() / (float)RAND_MAX;
	int destination = 0;
	for (int i = 0; i < distribution.size(); ++i)
		if (generated < distribution[i].value || (i == distribution.size() - 1))
		{
			destination = distribution[i].index;
			break;
		}
	xPos += (destination % 3) - 1;
	yPos += (int)(destination / 3) - 1;
	cv::Point2i point = cv::Point2i(xPos, yPos);
	trace.push_back(point);
	int near_cluster = 0;
	for (int j = -1; j < 2; ++j)
		for (int i = -1; i < 2; ++i)
			if (field->get_available(xPos + i, yPos + j) == CELL_CLUSTER)
				near_cluster++;
	if (near_cluster >= cluster_elements) 
	{
		cluster->add_element(xPos, yPos);
		isFinished = true;
	}
	if (yPos == field->get_height() - 2)
		isFinished = true;
	return point;
}