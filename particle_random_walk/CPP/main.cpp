#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>

const int HEIGHT = 25;
const int WIDTH = 25;

int main()
{
	Field field(WIDTH, HEIGHT);
	GraphEngine g_engine(500, 500);
	// field.print_potentials();
	// field.print_availables();
	for (int i = 0; i < 100; i++)
	{
		Particle particle(&field);
		while (!particle.is_finished())
		{
			//std::cout << particle.move() << '\n';
			particle.move();
			g_engine.process(&field, &particle);
		}
	}
	system("Pause");
	return 0;
}