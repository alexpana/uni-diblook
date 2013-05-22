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
		sharedData( true ), bpp(bpp), height( height ), width( width ), lutSize( lutSize ), lut( lut ), source( source )
	{
		widthInBytes = ( width * bpp  + 31 ) / 32 * 4;
	}

	Image( int bpp, int width, int height ) :
		sharedData( false ), bpp(bpp), height( height ), width( width ), lutSize( 256 )
	{
		widthInBytes = ( width * bpp  + 31 ) / 32 * 4;
		lut = new RGBQUAD[ lutSize ];
		source = new BYTE[ GetWidthInBytes() * height ];
	}

	Image( const Image& rhs ) :
		sharedData( false ), bpp( rhs.bpp ), width( rhs.width ), height( rhs.height ), lutSize( rhs.lutSize )
	{
		widthInBytes = ( width * bpp  + 31 ) / 32 * 4;
		lut = new RGBQUAD[ lutSize ];
		source = new BYTE[ GetWidthInBytes() * height ];

		for( int i = 0; i < lutSize; ++i ){
			lut[i] = rhs.lut[i];
		}

		for( int i = 0; i < width * height; ++i ){
			source[i] = rhs.source[i];
		}
	}

	Image& operator=( const Image& rhs ){
		// Handle assignation to self
		if( this == &rhs ){
			return *this;
		}

		bpp = rhs.bpp;
		width = rhs.width;
		height = rhs.height;
		lutSize = rhs.lutSize;
		widthInBytes = ( width * bpp  + 31 ) / 32 * 4;

		if( !sharedData ){
			delete[] lut;
			delete[] source;

			lut = new RGBQUAD[ lutSize ];
			source = new BYTE[ GetWidthInBytes() * height ];
		}

		for( int i = 0; i < lutSize; ++i ){
			lut[i] = rhs.lut[i];
		}

		for( int i = 0; i < width * height; ++i ){
			source[i] = rhs.source[i];
		}

		return *this;
	}

	~Image(){
		if( !sharedData ){
			delete[] source;
			delete[] lut;
		}
	}

	inline int GetWidth() const { return width; };
	inline int GetHeight() const{ return height; };
	inline int GetWidthInBytes() const { 
		return widthInBytes; 
	}

	void SetPixelLUTIndex( const int x, const int y, const int index );

	void SetLUTColor( int index, const Color& color );

	Color GetPixelColor( const int x, const int y) const;

	int Get8BitGrayscaleValue( int x, int y ) const;

	int GetLutIndex( const int x, const int y ) const;

	void Clear();

	bool PositionInRange( int x, int y ){
		return 0 <= x && x < GetHeight() && 0 <= y && y < GetWidth();
	}

private:
	bool sharedData;

	int bpp, width, height, lutSize, widthInBytes;
	RGBQUAD* lut;
	BYTE* source;
};