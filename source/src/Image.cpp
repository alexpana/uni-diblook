#include "Image.h"

const Color Color::BLACK = Color( 0 );

void Image::SetPixelLUTIndex( const int x, const int y, const int index ){
	source[ x * GetWidthInBytes() + y ] = index;
}

void Image::SetLUTColor( int index, const Color& color ){
	lut[ index ].rgbBlue = color.b;
	lut[ index ].rgbGreen = color.g;
	lut[ index ].rgbRed = color.r;
}

Color Image::GetPixelColor( const int x, const int y) const {
	int w = GetWidthInBytes();

	switch( bpp ){
	case 24:
		return Color( 
			source[ x * w + 3 * y ],
			source[ x * w + 3 * y + 1],
			source[ x * w + 3 * y + 2] );

	case 8:
		return Color( lut[source[ x * w + y ]] );

	case 4:
		if( y % 2 == 0 ){
			return Color( lut[ source[ x * w + y / 2 ] >> 4 ]);
		} else {
			return Color( lut[ source[ x * w + y / 2 ] & 0xf ]);
		}

	default:
		return Color(-1);
	}
}

int Image::GetLutIndex( const int x, const int y ) const {
	int w = GetWidthInBytes();

	switch( bpp ){
	case 8:
		return source[ x * w + y ];

	case 4:
		if( y % 2 == 0 ){
			return source[ x * w + y / 2 ] >> 4;
		} else {
			return source[ x * w + y / 2 ] & 0xf;
		}

	default:
		return -1;
	}
}