#pragma once

template <typename T>
T clamp( const T& min, const T& max, const T& value ){
	if( value < min ){
		return min;
	}
	if( value > max ){
		return max;
	}
	return value;
}