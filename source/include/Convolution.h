#pragma once

#include "ConvolutionKernel.h"
#include "Image.h"
#include "MathUtils.h"
#include "memory"

class Convolution
{
public:
	static void ApplyKernel( const Image& source, const ConvolutionKernel* kernel, Image& destination )
	{
		int kernelSize = kernel->Size();
		for( int i = kernelSize / 2; i < source.GetHeight() - kernelSize / 2; ++i )
		{
			for( int j = kernelSize / 2; j < source.GetWidth() - kernelSize / 2; ++j )
			{
				double finalValue = 0;
				for( int x = -kernelSize / 2; x <= kernelSize / 2; ++x )
				{
					for( int y = -kernelSize / 2; y <= kernelSize / 2; ++y )
					{
						finalValue += (double)source.GetPixelColor( i - x, j - y ).r * kernel->ValueAt( x + kernelSize / 2, y + kernelSize / 2 );
					}
				}
				finalValue = clamp( 0.0, 255.0, finalValue );
				destination.SetPixelLUTIndex( i, j, (int) finalValue );
			}
		}
	}
};