#pragma once

#include "ConvolutionKernel.h"
#include "Image.h"
#include "MathUtils.h"
#include "memory"

#define M_CLAMP(a, b, v) ((a) > (v) ? (a) : ((b) < (v) ? (b) : (v)))

class Convolution
{
public:
	static void ApplyKernel( const Image& source, const ConvolutionKernel* kernel, Image& destination )
	{
		int kernelWidth = kernel->Width();
		int kernelHeight = kernel->Height();

		int halfkw = kernelWidth / 2;
		int halfkh = kernelHeight / 2;

		int w = source.GetWidth();
		int h = source.GetHeight();

		for( int i = h - 1; i > 0; --i )
		{
			for( int j = w - 1; j > 0; --j )
			{
				double finalValue = 0;
				for( int x = -kernelHeight / 2; x <= kernelHeight / 2; ++x )
				{
					for( int y = -kernelWidth / 2; y <= kernelWidth / 2; ++y )
					{
						//int vx = M_CLAMP(0, h - 1, i - x);
						//int vy = M_CLAMP(0, w - 1, j - y);
						int vx = ( i - x + h ) % h;
						int vy = ( j - y + w ) % w;
						finalValue += (double)source.Get8BitGrayscaleValue( vx, vy ) * kernel->ValueAt( x + kernelHeight / 2, y + kernelWidth / 2 );
					}
				}
				finalValue = clamp( 0.0, 255.0, finalValue );
				destination.SetPixelLUTIndex( i, j, (int) finalValue );
			}
		}
	}
};