#include <field.h>
#include <iostream>
#include <iomanip>

Field::Field(int _width, int _height)
{
	width = _width;
	height = _height;
	potentials = new float* [height];
	availables = new int* [height];
	basic_potentials = new float* [height];
	for (int i = 0; i < height; ++i)
	{
		potentials[i] = new float[width];
		availables[i] = new int[width];
		potentials[i] = new float[width];
		for (int j = 0; j < width; ++j)
		{
			potentials[i][j] = 0.5;
			availables[i][j] = 0;
			basic_potentials[i][j] = 0.5;
		}
			
		availables[i][0] = availables[i][width - 1] = 1;
	}
	create_point_source_field();
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
		delete[] basic_potentials[i];
	}
	delete[] potentials;
	delete[] availables;
	delete[] basic_potentials;
}

void Field::print_potentials()
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
			std::cout << std::setw(4) <<std::setprecision(2) << potentials[i][j] << '\t';
		std::cout << '\n';
	}
}

void Field::print_availables()
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
			std::cout << availables[j][i] << '\t';
		std::cout << '\n';
	}
}

void Field::create_point_source_field()
{
	int center[2] = { (int)(width / 2), height - 1 };
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			float r_sqr = (i - center[0])*(i - center[0]) + (j - center[1])*(j - center[1]) + 1;
			basic_potentials[j][i] = potentials[j][i] = 1 / (0.01 * r_sqr);
		}
}

void Field::update_field(Cluster* cluster)
{
	float max = potentials[0][0] = basic_potentials[0][0] + cluster->get_potential(0, 0);
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			potentials[j][i] = basic_potentials[j][i] + cluster->get_potential(i, j);
			if (potentials[j][i] < max)
				max = potentials[j][i];
		}
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			potentials[j][i] = potentials[j][i] / max;
}