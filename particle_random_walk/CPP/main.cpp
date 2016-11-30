#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>
#include <cluster.h>

//================SETTINGS=================
// field settings
const int HEIGHT = 101;				// высота поля
const int WIDTH = 201;				// ширина поля
const int NUM_OF_FUSES = 4;			// число затравок
const float FUSE_POTENTIAL = 100;	// потенциал одной затравки

// particle settings
const int INVOLVEMENT = 1;				// элементов кластера для присоединения
const int NUMBER_OF_PARTICLES = 10000;	// число частиц в эксперименте
const bool SAVE_TRACE = false;			// сохранять ли траекторию (для скорости)

// cluster settings
const float SIGMA = 1;				// потенциал добавляемый одним элементом кластера
const float BETA = 0.01;			// коэффициент экранирования
const float CLUSTER_RADIUS = 20.0;	// расстояние на которое воздействует элемент кластера

// graph engine settings
const int WINDOW_WIDTH = WIDTH * 3;						// ширина окна отрисовка
const int WINDOW_HEIGHT = HEIGHT * 3;					// высота окна отрисовки
const std::string WIN_NAME = "Pore formation modeling"; // имя окна
const bool DRAW_EVERY_MOVE = false;						// рисовать ли каждое движение частицы отдельно
//============END OF SETTINGS==============


int main() // начало выполнения программы
{
	srand((unsigned int)time(0)); // зерно генератора случайных чисел
	Field field(WIDTH, HEIGHT, NUM_OF_FUSES, FUSE_POTENTIAL); // создаем поле
	GraphEngine g_engine(WIN_NAME, WINDOW_WIDTH, WINDOW_HEIGHT); // создаем графический движок
	Cluster cluster(&field, SIGMA, BETA, CLUSTER_RADIUS); // создаем кластер

	for (int i = 0; i < NUMBER_OF_PARTICLES; ++i) // запускаем частицы по одной
	{
		cv::Point2i point(0, 0);
		Particle particle(&field, &cluster, INVOLVEMENT, SAVE_TRACE); // создаем частицу
		while (!particle.is_finished()) {
			point = particle.move(); // двигаемся пока не встретим дно или кластер
			if (DRAW_EVERY_MOVE)
				g_engine.process(&field, &particle);
		}
		g_engine.process(&field, &particle); // рисуем что получилось, когда частица остановилась
		cluster.add_element(point.x, point.y); // добавляем элемент кластера в точке остановки
		field.update_field(&cluster); // пересчитываем поле
		if (cluster.is_on_top()) // проверяем не добрался ли кластер до верха
			break; // если добрался, завершаем моделирование
	}
	system("Pause");
	return 0;
}