#include <field.h>
#include <iostream>

Field::Field(int _width, int _height)
{
	width = _width;
	height = _height;
	potentials = new float* [height];
	availables = new int*[height];
	for (int i = 0; i < height; ++i)
	{
		potentials[i] = new float[width];
		availables[i] = new int[width];
		for (int j = 0; j < width; ++j)
		{
			potentials[i][j] = 0.5;
			availables[i][j] = 0;
		}
			
		availables[i][0] = availables[i][width - 1] = 1;
	}
	for (int i = 0; i < width; ++i)
	{
		availables[0][i] = 1;
		availables[height-1][i] = 1;
	}
}

Field::~Field()
{
	for (int i = 0; i < height; ++i)
	{
		delete[] potentials[i];
		delete[] availables[i];
	}
	delete[] potentials;
	delete[] availables;
}

void Field::print_potentials()
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
			std::cout << potentials[j][i] << ' ';
		std::cout << '\n';
	}
}

void Field::print_availables()
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
			std::cout << availables[j][i] << ' ';
		std::cout << '\n';
	}
}