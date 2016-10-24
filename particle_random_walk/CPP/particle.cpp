#include <particle.h>
#include <misc.h>

#include <iostream>
#include <ctime>

Particle::Particle(Field* _field)
{
	field = _field;
	srand(time(0));
	xPos = 1 + (rand() % (field->get_width() - 2));
	yPos = 1;
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
		{
			if (((i == 0) && (j == 0)) || (field->get_available(xPos + i, yPos + j) != 0))
				potentials[(j + 1) * 3 + (i + 1)] = 0;
			else
				potentials[(j + 1) * 3 + (i + 1)] = field->get_potential(xPos + i, yPos + j);
		}
	float sum = 0;
	for (int i = 0; i < 9; ++i)
		sum += potentials[i];
	for (int i = 0; i < 9; ++i)
		potentials[i] = abs(potentials[i] - cur) / sum;
	for (int i = 0; i < 9; ++i)
		std::cout << potentials[i] << ' ';
	std::cout << '\n';
	int destination = num_from_distribution(potentials, 9);
	xPos += (destination % 3) - 1;
	yPos += (int)(destination / 3) - 1;
	if (yPos == field->get_width() - 2)
		isFinished = true;
	return cv::Point2i(xPos, yPos);
}