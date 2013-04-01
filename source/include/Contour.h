#pragma once
#include "Vector2.h"
#include <vector>
using namespace std;

class Contour{
public:
	Contour(){
		Rewind();
	}

	void SetStartPosition( const Vector2& position ){
		startPosition = position;
		Rewind();
	}

	void AddDirection( int direction ){
		directions.push_back( direction );
	}

	void Rewind(){
		iterator = 0;
		currentPosition = startPosition;
	}

	bool HasNext(){
		return iterator < directions.size();
	}

	Vector2 NextPosition(){
		currentPosition += GetDelta(directions[iterator]);
		iterator += 1;
		return currentPosition;
	}

	vector<int> Derivative(){
		vector<int> result;

		return result;
	}

	Vector2 GetDelta( int direction ){
		static int x[] = {1, 1, 0, -1, -1, -1,  0,  1};
		static int y[] = {0, 1, 1,  1,  0, -1, -1, -1};
		return Vector2( x[direction], y[direction] );
	}

private:
	Vector2 startPosition;
	Vector2 currentPosition;

	int iterator;
	std::vector<int> directions;
};