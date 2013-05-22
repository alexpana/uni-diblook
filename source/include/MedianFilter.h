#pragma once

#include <vector>
#include <algorithm>
#include "MathUtils.h"

class MedianFilter{
public:
	static void ApplyKernel(const Image& source, Image& destination, int kernelWidth, int kernelHeight ){
		using namespace std;
		for( int i = 0; i < source.GetHeight(); ++i )
		{
			for( int j = 0; j < source.GetWidth(); ++j )
			{
				vector<int> values;
				double finalValue = 0;
				for( int x = -kernelHeight / 2; x <= kernelHeight / 2; ++x )
				{
					for( int y = -kernelWidth / 2; y <= kernelWidth / 2; ++y )
					{
						int vx = clamp( 0, source.GetHeight() - 1, i - x );
						int vy = clamp( 0, source.GetWidth() - 1, j - y );
						values.push_back( source.GetLutIndex( vx, vy ) );
					}
				}
				sort( values.begin(), values.end() );
				destination.SetPixelLUTIndex( i, j, values.at( (kernelHeight / 2) * kernelWidth) + kernelWidth / 2 + 1 );
			}
		}
	}
};