#pragma once

class Vector2{
public:
	int x, y;

	Vector2( int x = 0, int y = 0 ):
		x(x), y(y)
	{
	}

	Vector2& operator += ( const Vector2& rhs ){
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};