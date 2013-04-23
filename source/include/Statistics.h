#pragma once

#include "Histogram.h"
#include "Image.h"
#include <math.h>

class Statistics{
public:

	Statistics( CHistogram* histogram, int pixelCount ):
		histogram( histogram ), pixelCount( pixelCount ), maxIntensity( 255 )
	{
	}

	~Statistics()
	{
		delete histogram;
	}

	float MeanIntensity()
	{
		float mean = 0;
		for( int g = 0; g < maxIntensity; ++g )
		{
			mean += g * histogram->normalizedValue( g );
		}
		return mean;
	}

	float StandardDeviation()
	{
		float mean = MeanIntensity();
		float stdev = 0;
		for( int g = 0; g < maxIntensity; ++g )
		{
			stdev += ( g - mean ) * ( g - mean ) * intensityProbability( g );
		}
		return sqrt(stdev);
	}

	int ManhattanThreshold()
	{
		float* A = new float[maxIntensity];
		A[0] = pixelCount;
		int maxArg = 0;

		for( int i = 1; i < maxIntensity; ++i )
		{
			A[i] = A[i-1] - histogram->values[i-1];
			if( i * A[i] > maxArg * A[maxArg] )
			{
				maxArg = i;

			}
		}

		return maxArg;
	}

	void ShowStatistics()
	{
		CString stats;
		stats.Format( "Mean Value: %f\nStandard Deviation: %f\nBinary Distance: %d\n", MeanIntensity(), StandardDeviation(), ManhattanThreshold() );
		AfxMessageBox( stats );
	}

private:
	CHistogram* histogram;
	int pixelCount;
	int maxIntensity;

	float intensityProbability( int intensity )
	{
		return histogram->values[intensity] / (float)pixelCount;
	}
};