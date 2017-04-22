#ifndef _FIL_TER_H
#define _FIL_TER_H

#include <Arduino.h>


class Filter
{
	public:
	    Filter();
	    Filter(float alpha_initial);

	    void Set_alpha(float new_alpha);
	    float Get_alpha();

	    double Get_filtered_readings();

	    double apply_filter(double val);



	protected:
	    
	private:
    	float alpha;
    	double filtered_readings;
};

#endif // _FIL_TER_H