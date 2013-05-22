#pragma once

template <typename T>
inline T fmin( const T& a, const T& b ){
	return (a + b - abs(a-b)) / 2;
}

template <typename T>
inline T fmax( const T& a, const T& b ){
	return (a + b + abs(a-b)) / 2;
}

template <typename T>
inline T clamp( const T& min, const T& max, const T& value ){
	if( value < min ){
		return min;
	} else {
		if ( value > max ){
			return max;
		} else {
			return value;
		}
	}
}