#include <graph_engine.h>

GraphEngine::GraphEngine(int winWidth, int winHeight)
{
	// зададим начальные значения
	winName = "Test window";
	width = winWidth;
	height = winHeight;
	image.create(height, width, CV_8UC3);

	// создадим окно
	cv::namedWindow(winName, CV_WINDOW_AUTOSIZE);

	// задаем цвета
	unavailable_color = cv::Scalar(255, 255, 0);
	trace_color = cv::Scalar(0, 255, 0);
	cluster_color = cv::Scalar(0, 0, 0);
}

GraphEngine::~GraphEngine()
{
	cv::destroyWindow(winName);
}

void GraphEngine::process(Field* field, Particle* particle)
{
	// отрисовка поля
	int b_width = static_cast<int>(width / field->get_width());
	int b_height = static_cast<int>(height / field->get_height());
	for (int i = 0; i < field->get_width(); ++i)
		for (int j = 0; j < field->get_height(); ++j)
		{
			// рисуем границу
			if (field->get_available(i, j) == CELL_BORDER)
				cv::rectangle(image, 
					cv::Point(i * b_width, j * b_height),
					cv::Point((i + 1) * b_width, (j + 1) * b_height), unavailable_color, -1);
			// рисуем кластер
			else if (field->get_available(i, j) == CELL_CLUSTER)
			{
				cv::rectangle(image,
					cv::Point(i * b_width, j * b_height),
					cv::Point((i + 1) * b_width, (j + 1) * b_height), cluster_color, -1);
			}
			// рисуем пустую ячейку
			else
			{
				int r = static_cast<int>(field->get_potential(i, j) * 255.0);
				int b = 255 - r;
				cv::rectangle(image,
					cv::Point(i * b_width, j * b_height),
					cv::Point((i+1) * b_width, (j+1) * b_height), cv::Scalar(b, 0, r), -1);
			}
				
		}

	// рисуем траекторию движения частицы
	if (particle->get_trace()->size() != 0)
	{
		for (int k = 1; k < particle->get_trace()->size(); ++k)
		{
			int i_cur = (*(particle->get_trace()))[k].x;
			int j_cur = (*(particle->get_trace()))[k].y;
			int i_prw = (*(particle->get_trace()))[k-1].x;
			int j_prw = (*(particle->get_trace()))[k-1].y;
			cv::line(image, 
				cv::Point(static_cast<int>((i_cur + 0.5)*b_width),
					static_cast<int>((j_cur + 0.5)*b_height)),
				cv::Point(static_cast<int>((i_prw + 0.5)*b_width),
					static_cast<int>((j_prw + 0.5)*b_height)),
				trace_color, 3);
		}
		cv::Point2i cur_center = (*(particle->get_trace()))[particle->get_trace()->size() - 1];
		cv::ellipse(image, 
			cv::Point(static_cast<int>((cur_center.x + 0.5) * b_width),
				static_cast<int>((cur_center.y + 0.5) * b_height)),
			cv::Size(b_width * 0.2, b_height * 0.2), 0, 0, 360, cv::Scalar(0, 255, 0), -1);
	}
	cv::imshow(winName, image);
	cv::waitKey(1);
}