#include "stdafx.h"
#include "Histogram.h"

CHistogram::CHistogram()
{
	length = 256;
	values = new int[256];
	memset(values, 0, sizeof(values) * 256);
	normalized = false;
}

CHistogram::~CHistogram()
{
	delete[] values;
}

