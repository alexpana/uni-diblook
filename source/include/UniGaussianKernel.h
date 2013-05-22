#pragma once

#include "ConvolutionKernel.h"
#include "vector"

enum Direction {
	HORIZONTAL,
	VERTICAL
};

class UniGaussianKernel : public ConvolutionKernel{
public:

	UniGaussianKernel( double sigma, int direction )
	{
		BuildKernel( sigma );
		this->direction = direction;
	}

	int Width() const 
	{
		if( direction == Direction::HORIZONTAL )
		{
			return size;
		} else {
			return 1;
		}
	}

	int Height() const 
	{
		if( direction == Direction::VERTICAL )
		{
			return size;
		} else {
			return 1;
		}
	}

	virtual double ValueAt( int x, int y ) const
	{
		if( direction == Direction::VERTICAL ) {
			return values[x];
		} else {
			return values[y];
		}
	}

private:
	int size;
	int direction;
	double sigma;
	vector<double> values;

	void BuildKernel( double sigma ){
		size = (int) (sigma * 6.0 + 0.5);
		if( size % 2 == 0 ) size -= 1;
		this->sigma = sigma;

		int x0 = size / 2;
		double coef = 1 / (sqrt((double)(2 * 3.1415)) * sigma );

		for( int x = 0; x < size; ++x ){
			double _power = -(x-x0)*(x-x0) / (2 * sigma * sigma);
			double ex = exp(_power);
			values.push_back( coef * exp(_power) );
		}
	}
};