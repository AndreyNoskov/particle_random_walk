#include <misc.h>

int num_from_distribution(float* distribution, size_t length)
{
	float* tmp_dist = new float[length];
	tmp_dist[0] = distribution[0];
	for (int i = 1; i < length; ++i)
		tmp_dist[i] = tmp_dist[i - 1] + distribution[i];
	float generated = (float)rand() / (float)RAND_MAX;
	int returned = 0;
	for (int i = 0; i < length; ++i)
		if (generated < tmp_dist[i])
		{
			returned = i;
			break;
		}
	delete[] tmp_dist;
	return returned;
}