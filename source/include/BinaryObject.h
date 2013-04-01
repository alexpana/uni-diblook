#pragma once

#include <Windows.h>

class BinaryObject{
public:
	int area;
	POINT centerOfMass;
	float elongationAxis;
	int perimeter;
	float thinnessRatio;
	float aspectRatio;
	int* projectionsX;
	int projectionsXSize;
	int* projectionsY;
	int projectionsYSize;

	BinaryObject() :
		area(0),
		elongationAxis(0),
		perimeter(0),
		thinnessRatio(0),
		aspectRatio(0)
	{
		centerOfMass.x = centerOfMass.y = 0;
		projectionsX = projectionsY = nullptr;
	}

	~BinaryObject(){
		//delete[] projectionsX;
		//delete[] projectionsY;
	}

	CString toCString(){
		CString result;
		result.Format(
			"Area:\t\t%d\nPerimeter:\t%d\nOrigin X:\t\t%d\nOrigin Y:\t\t%d\nCircularity:\t%.4f\nAspect Ratio:\t%.4f\nElongation:\t%.4f", 
			area, 
			perimeter, 
			centerOfMass.x, 
			centerOfMass.y,
			thinnessRatio,
			aspectRatio,
			elongationAxis * 180.0f / 3.1416f
		);
		return result;
	}
};