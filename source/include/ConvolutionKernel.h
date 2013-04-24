#pragma once

class ConvolutionKernel
{
public:

	static const ConvolutionKernel* AVERAGE;
	static const ConvolutionKernel* GAUSSIAN;
	static const ConvolutionKernel* LAPLACIAN4;
	static const ConvolutionKernel* LAPLACIAN8;
	static const ConvolutionKernel* HIGHPASS5;
	static const ConvolutionKernel* HIGHPASS9;

	ConvolutionKernel( double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33 )
	{
		values[0] = m11; values[1] = m12; values[2] = m13;
		values[3] = m21; values[4] = m22; values[5] = m23;
		values[6] = m31; values[7] = m32; values[8] = m33;
		Normalize();
	}

	int Size() const
	{
		return 3;
	}

	double ValueAt( int x, int y ) const
	{
		if( x < 0 || x > 2 || y < 0 || y > 2 ){
			return 0;
		}
		return values[ x * 3 + y ];
	}

private:
	double values[9];

	void Normalize()
	{
		double sum = 0;
		for( int i = 0; i < 9; ++i )
		{
			sum += values[i];
		}

		if( sum == 0 )
		{
			return;
		}

		for( int i = 0; i < 9; ++i )
		{
			values[i] /= sum;
		}
	}

};