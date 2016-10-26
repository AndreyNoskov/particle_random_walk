#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>
#include <cluster.h>

const int HEIGHT = 55;
const int WIDTH = 55;

int main()
{
	Field field(WIDTH, HEIGHT);
	GraphEngine g_engine(500, 500);
	Cluster cluster(&field, 1, 1, 3);

	for (int i = 0; i < 100; i++)
	{
		cv::Point2i point(0, 0);
		Particle particle(&field, &cluster);
		while (!particle.is_finished())
		{
			point = particle.move();
			g_engine.process(&field, &particle);
		}
		cluster.add_element(point.x, point.y);
		field.update_field(&cluster);
		g_engine.process_stage(&field, &cluster);
	}
	system("Pause");
	return 0;
}