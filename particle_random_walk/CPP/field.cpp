#include <field.h>
#include <iostream>
#include <iomanip>


Field::Field(int _width, int _height, int num, float fi, APPROX_FIELD _approx_type)
{
	// инициализируем значения
	width = _width;
	height = _height;
	potentials = new float* [height];
	availables = new int* [height];
	basic_potentials = new float* [height];
	approx_type = _approx_type;
	
	// заполняем матрицы
	for (int i = 0; i < height; ++i)
	{
		potentials[i] = new float[width];
		availables[i] = new int[width];
		basic_potentials[i] = new float[width];
		for (int j = 0; j < width; ++j)
		{
			potentials[i][j] = 0.5;
			availables[i][j] = 0;
			basic_potentials[i][j] = 0.5;
		}
			
		availables[i][0] = availables[i][width - 1] = 1;
	}
	// create_point_source_field();

	// находим коэффициенты аппроксимаций
	if (approx_type == AF_CONST)
		approx_p = approx_q = 0;
	else
	{
		float D = static_cast<float>(width) / static_cast<float>(num + 1);
		float d_max = D; // ??
		float d_min = 0;
		float lambda_max = 1; // ??
		float lambda_min = height - 1;
		if (approx_type == AF_LINEAR)
		{
			approx_p = (d_min - D + 2.0 * D) / (lambda_max - lambda_min);
			approx_q = (lambda_max * (d_max + 2.0 * D) - lambda_min * d_min) / (lambda_max - lambda_min);
		}
		else if (approx_type == AF_LOG)
		{
			approx_p = (d_min - d_max - 2.0 * D) / log(lambda_max / lambda_min);
			approx_q = (d_max*log(lambda_max) + 2.0 * D - d_min*log(lambda_min)) / log(lambda_max / lambda_min);
		}
		else if (approx_type == AF_REVERSE)
		{
			approx_p = (d_max - d_min + 2.0 * D) / (1.0 / lambda_max - 1.0 / lambda_min);
			approx_q = d_max + 2 * D - approx_p / lambda_min;
		}
	}

	// создаем поле
	hyperbolic_field(num, fi);
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
			std::cout << std::setw(4) <<std::setprecision(2) << potentials[j][i] << '\t';
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
	int center[2] = { static_cast<int>(width / 2), height - 1 };
	float max = 0;
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			float r_sqr = static_cast<float>(i - center[0])*(i - center[0]) + (j - center[1])*(j - center[1]) + 1;
			basic_potentials[j][i] = potentials[j][i] = static_cast<float>(1.0 / (0.01 * sqrt(r_sqr)));
			if (max < basic_potentials[j][i])
				max = basic_potentials[j][i];
		}
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			basic_potentials[j][i] = potentials[j][i] = basic_potentials[j][i] / max;
}

bool is_not_more(int x, int y, float l, float beta)
{
	float _x = static_cast<float>(x);
	float _y = static_cast<float>(y);
	float value = (_x*_x) / (l*l) - (_y*_y / (beta*beta - l*l));
	return (value > 1) ? false : true;
}

float sqr(float a)
{
	return a * a;
}

float Field::get_beta_by_lambda(float lambda, APPROX_FIELD aprox_type)
{
	float tmp = 0;
	switch (aprox_type)
	{
		case AF_CONST:
			tmp = static_cast<float>(height);
			break;
		case AF_LINEAR:
			tmp = sqrt(sqr(approx_p*lambda + approx_q) + sqr(lambda));
			break;
		case AF_LOG:
			tmp = sqrt(sqr(approx_p*log(lambda) + approx_q) + sqr(lambda));
			break;
		case AF_REVERSE:
			tmp = sqrt(sqr(approx_p / lambda + approx_q) + sqr(lambda));
			break;
		default:
			tmp = static_cast<float>(height);
	}
	return tmp;
}

float** Field::create_hyperbolic_field(float fi, int width, int height)
{
	float** field = new float*[width];
	for (int j = 0; j < width; ++j)
	{
		field[j] = new float[height];
		for (int i = 0; i < height; ++i)
			field[j][i] = 0;
	}

	for (float lambda = static_cast<float>(height - 1); lambda >= 0; --lambda)
	{
		float b = get_beta_by_lambda(lambda, approx_type);
		for (int y = 0; y < width; ++y)
			for (int x = 0; x < height; ++x)
				if (is_not_more(x, y, lambda, b))
					field[y][x] = fi * log((b + lambda) / (b - lambda));
	}
		

	return field;
}

void Field::hyperbolic_field(int num, float fi)
{
	float** hyp_field = create_hyperbolic_field(fi, width, height);
	int shift = static_cast<int>(static_cast<float>(width) / static_cast<float>(num + 1));
	for (int n = 1; n <= num; ++n)
		for (int j = 0; j < height; ++j)
			for (int i = 0; i < width; ++i)
			{
				float value = 0;
				if (i < n * shift)
					value = hyp_field[n* shift - i][j];
				else
					value = hyp_field[i - n* shift][j];
				potentials[j][i] = basic_potentials[j][i] += value;
			}
	for (int j = 0; j < height; ++j)
		delete[] hyp_field[j];
	delete[] hyp_field;
}

void Field::update_field(Cluster* cluster)
{
	float max = basic_potentials[0][0] + cluster->get_potential(0, 0);
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			potentials[j][i] = basic_potentials[j][i] + cluster->get_potential(i, j);
			if (availables[j][i] != 1)
				availables[j][i] = (cluster->get_element(i, j) != CLUSTER_EMPTY) ? 2 : 0;
			if (potentials[j][i] > max)
				max = potentials[j][i];
		}
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			potentials[j][i] = potentials[j][i] / max;
}