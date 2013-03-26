#pragma once
#include <afxwin.h>
#include <WinDef.h>
#include <WinGDI.h>

struct Color{
	int r, g, b, a;
	Color( int r, int g, int b, int a ) : r(r), g(g), b(b), a(a) {}
	Color( int r, int g, int b ): r(r), g(g), b(b), a(255) {}
	Color( int c ): r(c), g(c), b(c), a(255) {}

	Color( RGBQUAD quad ): 
		r(quad.rgbRed),
		g(quad.rgbGreen),
		b(quad.rgbBlue),
		a(255)
	{}

	bool operator == ( const Color& rhs ){
		return( r == rhs.r && g == rhs.g && b == rhs.b );
	}

	static const Color BLACK;
};

class Image{
public:
	Image( int bpp, int width, int height, RGBQUAD* lut, int lutSize, BYTE* source ) :
	  bpp(bpp), height( height ), width( width 	), lutSize( lutSize ), lut( lut ), source( source )
	{
	}

	int GetWidth() const { return width; };
	int GetHeight() const{ return height; };
	int GetWidthInBytes() const { 
		return ( width * bpp  + 31 ) / 32 * 4; 
	}

	void SetPixelLUTIndex( const int x, const int y, const int index );

	void SetLUTColor( int index, const Color& color );

	Color GetPixelColor( const int x, const int y) const;

	int GetLutIndex( const int x, const int y ) const;

private:
	int bpp, width, height, lutSize;
	RGBQUAD* lut;
	BYTE* source;
};