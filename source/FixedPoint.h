#pragma once
#include <stdint.h>

class FixedPoint
{
public:
	const static int SCALER;
	int32_t value;
	const static int MAX_VALUE;

	FixedPoint() : value(0) { }

	FixedPoint(const int integer) : value(integer*SCALER) { }

	FixedPoint(const bool tmp, const int value) : value(value) { }

	FixedPoint &operator=(const int init)
	{
		value = init*SCALER;
		return *this;
	}

	FixedPoint &operator=(const FixedPoint &fixed)
	{
		value = fixed.value;
		return *this;
	}

	FixedPoint operator*(const int mult)const
	{
		return FixedPoint(true, mult * value);
	}

	FixedPoint operator*(const FixedPoint &mult) const
	{
		return FixedPoint(true, (mult.value * value) / SCALER);
	}

	FixedPoint operator/(const int div)const
	{
		return FixedPoint(true, value / div);
	}

	FixedPoint operator/(const FixedPoint &div) const
	{
		return FixedPoint(true, (value * SCALER) / div.value);
	}

	FixedPoint operator+(const int add)const
	{
		return FixedPoint(true, value + add * SCALER);
	}

	FixedPoint operator+(const FixedPoint &add) const
	{
		return FixedPoint(true, value + add.value);
	}

	FixedPoint &operator+=(const FixedPoint &add)
	{
		value += add.value;
		return *this;
	}

	FixedPoint &operator+=(const int add)
	{
		value += add*SCALER;
		return *this;
	}

	FixedPoint &operator-=(const FixedPoint &sub)
	{
		value -= sub.value;
		return *this;
	}

	FixedPoint &operator-=(const int sub)
	{
		value -= sub*SCALER;
		return *this;
	}

	FixedPoint &operator*=(const int mul)
	{
		value *= mul;
		return *this;
	}

	FixedPoint &operator*=(const FixedPoint &fixed)
	{
		value *= fixed.value;
		value /= SCALER;
		return *this;
	}

	FixedPoint &operator/=(const int div)
	{
		value /= div;
		return *this;
	}

	FixedPoint &operator/=(const FixedPoint &fixed)
	{
		value *= SCALER;
		value /= fixed.value;
		return *this;
	}

	FixedPoint operator-(const FixedPoint &sub) const
	{
		return FixedPoint(true, value - sub.value);
	}

	FixedPoint operator-(const int sub)const
	{
		return FixedPoint(true, value - sub * SCALER);
	}

	operator int() const
	{
		return value/ SCALER;
	}

	bool operator<(const int val)const
	{
		return value < val*SCALER;
	}

	bool operator>(const int val)const
	{
		return value > val*SCALER;
	}

	bool operator>(const FixedPoint &fixed) const
	{
		return value > fixed.value;
	}

	bool operator<(const FixedPoint &fixed) const
	{
		return value < fixed.value;
	}
};

inline FixedPoint operator*(const int mult, const FixedPoint &fixed)
{
	return FixedPoint(true, mult * fixed.value);
}

inline FixedPoint operator/(const int div, const FixedPoint &fixed)
{
	return FixedPoint(true, (div * FixedPoint::SCALER) / fixed.value);
}

inline FixedPoint operator+(const int add, const FixedPoint &fixed)
{
	return FixedPoint(true, fixed.value + add * FixedPoint::SCALER);
}

inline FixedPoint operator-(const int sub, const FixedPoint &fixed)
{
	return FixedPoint(true, sub * FixedPoint::SCALER - fixed.value);
}

inline FixedPoint operator-(const FixedPoint &fixed)
{
	return FixedPoint(true, -fixed.value);
}

inline bool operator<(const int val, const FixedPoint &fixed)
{
	return val * FixedPoint::SCALER < fixed.value;
}

inline bool operator>(const int val, const FixedPoint &fixed)
{
	return val * FixedPoint::SCALER > fixed.value;
}