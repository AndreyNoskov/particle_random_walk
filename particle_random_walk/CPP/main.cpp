#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>
#include <cluster.h>

// field settings
const int HEIGHT = 201;
const int WIDTH = 401;

// particle settings
const int INVOLVEMENT = 1;
const int NUMBER_OF_PARTICLES = 10000;

// cluster settings
const float SIGMA = 1;
const float BETA = 0.01;
const float CLUSTER_RADIUS = 4.0;

// graph engine settings
const int WINDOW_WIDTH = WIDTH * 3;
const int WINDOW_HEIGHT = HEIGHT * 3;

int main()
{
	srand((unsigned int)time(0));

	Field field(WIDTH, HEIGHT);
	GraphEngine g_engine(WINDOW_WIDTH, WINDOW_HEIGHT);
	Cluster cluster(&field, SIGMA, BETA, CLUSTER_RADIUS);

	for (int i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		cv::Point2i point(0, 0);
		Particle particle(&field, &cluster, INVOLVEMENT);
		while (!particle.is_finished())
		{
			point = particle.move();
			//g_engine.process(&field, &particle);
		}
		g_engine.process(&field, &particle);
		cluster.add_element(point.x, point.y);
		field.update_field(&cluster);
		if (cluster.is_on_top())
			break;
	}
	system("Pause");
	return 0;
}