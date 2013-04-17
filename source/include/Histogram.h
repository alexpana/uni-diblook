#pragma once


// CHistogram

class CHistogram
{
public:
	int* values;
	int length;
	int maxValue;

	CHistogram();
	virtual ~CHistogram();

	float normalizedValue( int index ) const {
		if( !normalized ){
			normalize();
		}
		return valuesNormalized[index];
	}

	float normalizedMax() const {
		return maxValueNormalized;
	}

protected:

	void normalize() const{
		valuesNormalized = new float[length];
		long sum = 0;
		maxValueNormalized = 0;
		for( int i = 0; i < length; ++i ){
			sum += values[i];
		}

		for( int i = 0; i < length; ++i ){
			valuesNormalized[i] = (float)values[i] / (float)sum;
			if( valuesNormalized[i] > maxValueNormalized ){
				maxValueNormalized = valuesNormalized[i];
			}
		}

	}

private:
	mutable bool normalized;
	mutable float* valuesNormalized;
	mutable float maxValueNormalized;
};


