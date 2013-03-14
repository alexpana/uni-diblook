#include "stdafx.h"
#include "Histogram.h"

CHistogram::CHistogram()
{
	values = new int[256];
	length = 256;

	memset(values, 0, sizeof(values) * 256);
	normalized = false;
}

CHistogram::~CHistogram()
{
	delete[] values;
}

