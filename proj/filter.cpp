#include "filter.h"


Filter::Filter()
{
    alpha = 0;
    filtered_readings = 0;
}

Filter::Filter(float alpha_initial)
{
    alpha = alpha_initial;
    filtered_readings = 0;
}

void Filter::Set_alpha(float new_alpha)
{
	alpha = new_alpha;
}

float Filter::Get_alpha()
{
	return alpha;
}

double Filter::apply_filter(double val)
{
	filtered_readings = alpha*filtered_readings + (1-alpha) * val;
	return filtered_readings;
}

double Filter::Get_filtered_readings()
{
	return filtered_readings;
}

