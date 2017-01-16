#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>
#include <cluster.h>
#include <source.h>

//================SETTINGS=================
// field settings
const int HEIGHT = 101;				// высота поля
const int WIDTH = 201;				// ширина поля
const int NUM_OF_FUSES = 2;			// число затравок
const float FUSE_POTENTIAL = 100;	// потенциал одной затравки
const APPROX_FIELD APPROX_TYPE = AF_LINEAR; // тип аппроксимации поля

// particle settings
const int INVOLVEMENT = 1;				// элементов кластера для присоединения
const int NUMBER_OF_PARTICLES = 10000;	// число частиц в эксперименте
const bool SAVE_TRACE = false;			// сохранять ли траекторию (для скорости)

// cluster settings
const float SIGMA = 1;				// потенциал добавляемый одним элементом кластера
const float BETA = 0.01f;			// коэффициент экранирования
const float CLUSTER_RADIUS = 20.0;	// расстояние на которое воздействует элемент кластера

// graph engine settings
const int WINDOW_WIDTH = WIDTH * 3;						// ширина окна отрисовка
const int WINDOW_HEIGHT = HEIGHT * 3;					// высота окна отрисовки
const std::string WIN_NAME = "Pore formation modeling"; // имя окна
const bool DRAW_EVERY_MOVE = false;						// рисовать ли каждое движение частицы отдельно

//source settings
const bool USE_SOURCE = false;	// использовать ли иссякающий источник
const int SOURCE_HEIGHT = 30;	// высота источника
//============END OF SETTINGS==============


int main()																		// начало выполнения программы
{
	srand((unsigned int)time(0));												// зерно генератора случайных чисел
	Field field(WIDTH, HEIGHT, NUM_OF_FUSES, FUSE_POTENTIAL, APPROX_TYPE);		// создаем поле
	GraphEngine g_engine(WIN_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);				// создаем графический движок
	Cluster cluster(&field, SIGMA, BETA, CLUSTER_RADIUS);						// создаем кластер
	Source source(&field, SOURCE_HEIGHT);										// создаем источник
	Source* p_source = (USE_SOURCE)? &source : nullptr;
	int num = (USE_SOURCE) ?
		source.get_height() * source.get_width() :
		NUMBER_OF_PARTICLES;													// сколько частиц моделировать
	for (int i = 0; i < num; ++i)												// запускаем частицы по одной
	{
		cv::Point2i point(0, 0);
		Particle particle(&field, &cluster, p_source, INVOLVEMENT, SAVE_TRACE); // создаем частицу
		while (!particle.is_finished()) {
			point = particle.move();											// двигаемся пока не встретим дно или кластер
			if (DRAW_EVERY_MOVE)
				g_engine.process(&field, &particle, p_source);
		}
		g_engine.process(&field, &particle, p_source);							// рисуем что получилось, когда частица остановилась
		cluster.add_element(point.x, point.y);									// добавляем элемент кластера в точке остановки
		field.update_field(&cluster);											// пересчитываем поле
		if (cluster.is_on_top((p_source) ? p_source->get_height() : 1))			// проверяем не добрался ли кластер до верха
			break;																// если добрался, завершаем моделирование
	}
	system("Pause");
	return 0;
}