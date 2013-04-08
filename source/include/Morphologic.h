#pragma once
#include "Image.h"
#include <string>

class StructuralElement{
public:
	StructuralElement() :
		_size(0),
		mask( nullptr )
	{
	}

	StructuralElement( std::string structure ){
		_size = (int)sqrt( structure.length() );
		mask = new bool[_size*_size];

		for( int i = 0; i < _size; ++i ){
			for( int j = 0; j < _size; ++j ){
				mask[ i * _size + j ] = structure[i * _size + j] == '1' ? true : false;
			}
		}
	}

	~StructuralElement(){
		delete[] mask;
	}

	bool get( int x, int y ) const{
		return mask[ x * _size + y ];
	}

	int size() const {
		return _size;
	}

private:
	int _size;
	bool* mask;
};

class Morphologic{
public:
	static const StructuralElement* const ElementFourNeighbours;
	static const StructuralElement* const ElementEightNeighbours;

	static void Dilate( const Image& src, const StructuralElement* const element, Image& destination ){
		for( int i = 0; i < src.GetHeight(); ++i ){
			for( int j = 0; j < src.GetWidth(); ++j ){

				// If the current color is BLACK
				int es = element->size();
				if( src.GetLutIndex(i, j) == 0 ){
					for( int x = 0; x < es; ++x ){
						for( int y = 0; y < es; ++y ){

							int ix = i - es / 2 + x;
							int iy = j - es / 2 + y;
							if( element->get( x, y ) && destination.PositionInRange( ix, iy ) ){
								//destination.SetPixelLUTIndex( 10, 10, 0 );
								destination.SetPixelLUTIndex( ix, iy, 0 );
							}
						}
					}
				}
			}
		}
	}

	static Image Erode( const Image& src, const StructuralElement* const element ){
		Image dst( src );
		return dst;
	}

};