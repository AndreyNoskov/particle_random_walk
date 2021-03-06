#include <graph_engine.h>

GraphEngine::GraphEngine(std::string _win_name, int winWidth, int winHeight)
{
	// ������� ��������� ��������
	winName = _win_name;
	width = winWidth;
	height = winHeight;
	image.create(height, width, CV_8UC3);

	// �������� ����
	cv::namedWindow(winName, CV_WINDOW_AUTOSIZE);

	// ������ �����
	unavailable_color = cv::Scalar(0, 0, 0);
	trace_color = cv::Scalar(0, 255, 0);
	cluster_color = cv::Scalar(0, 255, 255);
}

GraphEngine::~GraphEngine()
{
	cv::destroyWindow(winName);
}

void GraphEngine::process(Field* field, Particle* particle, Source* source)
{
	// ��������� ����
	int b_width = static_cast<int>(width / field->get_width());
	int b_height = static_cast<int>(height / field->get_height());
	for (int i = 0; i < field->get_width(); ++i)
		for (int j = 0; j < field->get_height(); ++j)
		{
			// ������ �������
			if (field->get_available(i, j) == CELL_BORDER)
				cv::rectangle(image, 
					cv::Point(i * b_width, j * b_height),
					cv::Point((i + 1) * b_width, (j + 1) * b_height), unavailable_color, -1);
			// ������ �������
			else if (field->get_available(i, j) == CELL_CLUSTER)
				cv::rectangle(image,
					cv::Point(i * b_width, j * b_height),
					cv::Point((i + 1) * b_width, (j + 1) * b_height), cluster_color, -1);
			// ������ ������ ������
			else
			{
				int r = static_cast<int>(field->get_potential(i, j) * 255.0);
				int b = 255 - r;
				cv::rectangle(image,
					cv::Point(i * b_width, j * b_height),
					cv::Point((i+1) * b_width, (j+1) * b_height), cv::Scalar(b, 0, r), -1);
			}
		}
	// ������ ��������
	if (source != nullptr)
	{
		for (int i = 1; i < field->get_width() - 1; ++i)
			for (int j = 1; j < source->get_height(); ++j)
			{
				if (source->get_value(j - 1, i - 1) == SOURCE_FULL)
				{
					cv::rectangle(image,
						cv::Point(i * b_width, j * b_height),
						cv::Point((i + 1) * b_width, (j + 1) * b_height), cv::Scalar(150, 150, 150), -1);
				}
				else if (source->get_value(j - 1, i - 1) == SOURCE_PERIMETER)
				{
					cv::rectangle(image,
						cv::Point(i * b_width, j * b_height),
						cv::Point((i + 1) * b_width, (j + 1) * b_height), cv::Scalar(100, 100, 100), -1);
				}
				else
					continue;
			}
	}

	// ������ ���������� �������� �������
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
			cv::Size(static_cast<int>(b_width * 0.2), static_cast<int>(b_height * 0.2)),
				0, 0, 360, cv::Scalar(0, 255, 0), -1);
	}
	cv::flip(image, image, -1);
	cv::imshow(winName, image);
	cv::waitKey(1);
}