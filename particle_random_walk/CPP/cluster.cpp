#include <cluster.h>
#include <particle.h>
#include <math.h>

Cluster::Cluster(Field* _field, float _sigma, float _beta, float _radius)
{
	sigma = _sigma;
	beta = _beta;
	radius = _radius;
	field = _field;
	width = field->get_width();
	height = field->get_height();
	cluster_field = new float*[height];
	cluster_elements = new int*[height];
	for (int j = 0; j < height; ++j)
	{ 
		cluster_field[j] = new float[width];
		cluster_elements[j] = new int[width];
		for (int i = 0; i < width; ++i)
		{
			cluster_field[j][i] = 0;
			cluster_elements[j][i] = CLUSTER_EMPTY;
		}
	}
}

Cluster::~Cluster()
{
	for (int i = 0; i < height; ++i)
		delete[] cluster_field[i];
	delete[] cluster_field;
}

bool is_perimeter(Field* field, int x, int y)
{
	int neighbors = 0;
	if (field->get_available(x - 1, y) != CELL_EMPTY)
		neighbors++;
	if (field->get_available(x, y - 1) != CELL_EMPTY)
		neighbors++;
	if (field->get_available(x + 1, y) != CELL_EMPTY)
		neighbors++;
	if (field->get_available(x, y + 1) != CELL_EMPTY)
		neighbors++;
	return (neighbors < 4) ? true : false;
}

void Cluster::add_element(int x, int y)
{
	cluster_elements[y][x] = (is_perimeter(field, x, y)) ? CLUSTER_PERIMETER : CLUSTER_INTERN;
	for (int j = -1; j < 2; ++j)
		for (int i = -1; i < 2; ++i)
			if ( !( abs(i) == abs(j) ) )
				if (field->get_available[x+i][y+j] != CELL_BORDER)
					cluster_elements[y+j][x+i] = (is_perimeter(field, x+i, y-1)) ? CLUSTER_PERIMETER : CLUSTER_INTERN;
	cluster_elements = new int*[height];
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
			cluster_field[j][i] = 0;
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
			if (cluster_elements[j][i] == CLUSTER_PERIMETER)
			{
				int rad = int(radius);
				for (int p = -rad; p < rad; ++p)
					for (int q = -rad; q < rad; ++q)
					{
						if (p*p + q*q < radius*radius)
						{
							bool condition = (j + p < width - 1) && (j + p > 1) 
								&& (i + q < height - 1) && (i + q > 1);
							if (condition)
							{
								float dist = sqrt(p*p + q*q);
								float value = sigma / dist * exp(beta * dist);
								cluster_field[j + p][i + q] = value;
							}
						}
					}
			}
}