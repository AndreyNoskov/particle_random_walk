#include "source.h"

Source::Source(Field * _field, int source_position)
{
	field = _field;
	height = source_position;
	width = field->get_width() - 2;

	// создаем массив частиц источника
	source_array = new Source_Cell* [height];
	for (int j = 0; j < height; ++j)
	{
		source_array[j] = new Source_Cell[width];
		for (int i = 0; i < width; ++i)
			source_array[j][i] = SOURCE_FULL;
	}
	
	// создаем список элементов периметра источника
	for (int i = 0; i < width; ++i)
	{
		perimeter.push_back(cv::Point2i(i, height - 1));
		source_array[height - 1][i] = SOURCE_PERIMETER;
	}
		
}

Source::~Source()
{
	for (int i = 0; i < height; ++i)
		delete[] source_array[i];
	delete[] source_array;
}

void Source::add_finished_point(cv::Point2i point)
{
	std::vector<cv::Point2i> directions;
	point.x -= 1;
	point.y -= 1;

	// thats fucking hardcoded shit!!!
	directions.push_back(cv::Point2i( 0, -1));
	directions.push_back(cv::Point2i(-1, -1));
	directions.push_back(cv::Point2i(-1,  0));
	directions.push_back(cv::Point2i(-1,  1));
	directions.push_back(cv::Point2i( 0,  1));
	directions.push_back(cv::Point2i( 1,  1));
	directions.push_back(cv::Point2i( 1,  0));
	directions.push_back(cv::Point2i( 1, -1));

	for (std::list<cv::Point2i>::iterator it = perimeter.begin(); it != perimeter.end(); ++it)
		if (it->x == point.x)
		{
			source_array[it->y][it->x] = SOURCE_EMPTY;
			for (int i = 0; i < directions.size(); ++i)
			{
				cv::Point2i source_point(it->x + directions[i].x, it->y + directions[i].y);
				if (source_point.x >= 0 &&
					source_point.y >= 0 &&
					source_point.x < width &&
					source_point.y < height &&
					source_array[source_point.y][source_point.x] == SOURCE_FULL)
				{
					perimeter.insert(it, source_point);
					source_array[source_point.y][source_point.x] = SOURCE_PERIMETER;
				}
			}
			perimeter.erase(it);
			break;
		}
}

cv::Point2i Source::get_next_point()
{
	std::list<cv::Point2i>::iterator it = perimeter.begin();
	int num = rand() % (perimeter.size());
	for (int i = 0; i < num; ++i)
		++it;
	return cv::Point2i(it->x + 1, it->y + 1);
}
