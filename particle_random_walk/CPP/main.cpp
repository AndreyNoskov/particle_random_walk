#include <field.h>
#include <particle.h>
#include <misc.h>

const int HEIGHT = 20;
const int WIDTH = 20;

int main()
{
	Field field(WIDTH, HEIGHT);
	field.print_potentials();
	field.print_availables();
	Particle particle( &field );
	for (int i = 0; i < 1000; i++)
		std::cout << particle.move() << '\n';
	return 0;
}