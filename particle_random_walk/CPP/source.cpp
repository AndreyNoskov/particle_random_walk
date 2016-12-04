#include "source.h"

enum Source_Cell {
	SOURCE_EMPTY	 = 0,
	SOURCE_FULL		 = 1,
	SOURCE_PERIMETER = 2
};

Source::Source(Field * _field, size_t source_position)
{
	field = _field;
	height = source_position;

	// создаем массив частиц источника
	source_array = new int* [source_position];
	for (int j = 0; j < source_position; ++j)
	{
		source_array[j] = new int[field->get_width()];
		for (int i = 0; i < field->get_width(); ++i)
			source_array[j][i] = SOURCE_FULL;
	}
	
	// создаем список элементов периметра источника
	for (int i = 0; i < field->get_width(); ++i)
		perimeter.push_back(cv::Point2i(i, source_position - 1));
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
					source_point.x < field->get_width() &&
					source_point.y < height &&
					source_array[source_point.y][source_point.x] == SOURCE_FULL)
				{
					perimeter.insert(it, source_point);
					source_array[source_point.y][source_point.x] = SOURCE_PERIMETER;
				}
								
			}
		}
}

cv::Point2i Source::get_next_point()
{
	std::list<cv::Point2i>::iterator it = perimeter.begin();
	int num = rand() % (perimeter.size());
	for (int i = 0; i < num; ++i)
		++it;
	// source_array[it->x][it->y] = SOURCE_EMPTY;
	return cv::Point2i(it->x, it->y);

}
