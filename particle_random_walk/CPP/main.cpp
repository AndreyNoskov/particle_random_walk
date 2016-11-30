#include <field.h>
#include <particle.h>
#include <misc.h>
#include <graph_engine.h>
#include <cluster.h>

//================SETTINGS=================
// field settings
const int HEIGHT = 101;				// ������ ����
const int WIDTH = 201;				// ������ ����
const int NUM_OF_FUSES = 4;			// ����� ��������
const float FUSE_POTENTIAL = 100;	// ��������� ����� ��������

// particle settings
const int INVOLVEMENT = 1;				// ��������� �������� ��� �������������
const int NUMBER_OF_PARTICLES = 10000;	// ����� ������ � ������������
const bool SAVE_TRACE = false;			// ��������� �� ���������� (��� ��������)

// cluster settings
const float SIGMA = 1;				// ��������� ����������� ����� ��������� ��������
const float BETA = 0.01;			// ����������� �������������
const float CLUSTER_RADIUS = 20.0;	// ���������� �� ������� ������������ ������� ��������

// graph engine settings
const int WINDOW_WIDTH = WIDTH * 3;						// ������ ���� ���������
const int WINDOW_HEIGHT = HEIGHT * 3;					// ������ ���� ���������
const std::string WIN_NAME = "Pore formation modeling"; // ��� ����
const bool DRAW_EVERY_MOVE = false;						// �������� �� ������ �������� ������� ��������
//============END OF SETTINGS==============


int main() // ������ ���������� ���������
{
	srand((unsigned int)time(0)); // ����� ���������� ��������� �����
	Field field(WIDTH, HEIGHT, NUM_OF_FUSES, FUSE_POTENTIAL); // ������� ����
	GraphEngine g_engine(WIN_NAME, WINDOW_WIDTH, WINDOW_HEIGHT); // ������� ����������� ������
	Cluster cluster(&field, SIGMA, BETA, CLUSTER_RADIUS); // ������� �������

	for (int i = 0; i < NUMBER_OF_PARTICLES; ++i) // ��������� ������� �� �����
	{
		cv::Point2i point(0, 0);
		Particle particle(&field, &cluster, INVOLVEMENT, SAVE_TRACE); // ������� �������
		while (!particle.is_finished()) {
			point = particle.move(); // ��������� ���� �� �������� ��� ��� �������
			if (DRAW_EVERY_MOVE)
				g_engine.process(&field, &particle);
		}
		g_engine.process(&field, &particle); // ������ ��� ����������, ����� ������� ������������
		cluster.add_element(point.x, point.y); // ��������� ������� �������� � ����� ���������
		field.update_field(&cluster); // ������������� ����
		if (cluster.is_on_top()) // ��������� �� �������� �� ������� �� �����
			break; // ���� ��������, ��������� �������������
	}
	system("Pause");
	return 0;
}