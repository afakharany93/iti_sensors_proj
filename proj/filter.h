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

	    double aplly_comp_filter(double val1, double val2); //http://www.pieter-jan.com/node/11


	protected:
	    
	private:
    	float alpha;
    	double filtered_readings;
};

#endif // _FIL_TER_H