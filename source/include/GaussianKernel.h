#pragma once

#include "ConvolutionKernel.h"
#include "vector"

class GaussianKernel : public ConvolutionKernel{
public:

	GaussianKernel()
	{
		BuildKernel( 0.8 );
	}

	GaussianKernel( double sigma )
	{
		BuildKernel( sigma );
	}

	~GaussianKernel(){
		delete[] values;
	}

	int Width() const 
	{
		return size;
	}

	int Height() const 
	{
		return size;
	}

	virtual double ValueAt( int x, int y ) const
	{
		return values[x * size + y];
	}

private:
	int size;
	double sigma;
	double* values;

	void BuildKernel( double sigma ){
		size = (int) (sigma * 6.0 + 0.5);
		if( size % 2 == 0 ) size -= 1;
		this->sigma = sigma;

		int x0 = size / 2;
		int y0 = size / 2;
		double coef = 1 / ( 2 * 3.1415 * sigma * sigma );

		values = new double[size*size];

		for( int x = 0; x < size; ++x ){
			for( int y = 0; y < size; ++y ){
				double _power = -((x-x0)*(x-x0)+(y-y0)*(y-y0)) / (2 * sigma * sigma);
				double ex = exp(_power);
				values[x * size + y] = coef * exp(_power);
			}
		}
	}
};