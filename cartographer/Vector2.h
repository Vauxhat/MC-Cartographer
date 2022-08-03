#ifndef VECTOR_2_H_
#define VECTOR_2_H_

#include <cmath>

namespace Vaux
{
	template <class Type = float> class Vector2
	{
	public:
		Type x, y;

	public:
		Vector2();
		Vector2(Type x, Type y);

		// Vector functions.
		static Type Dot(const Vector2<Type>& a, const Vector2<Type>& b);
		static Vector2<Type> Normalise(const Vector2<Type>& v);
		static Vector2<Type> Lerp(const Vector2<Type>& a, const Vector2<Type>& b, const Type& t);

		// Length functions.
		static auto Length(const Vector2<Type>& v);
		static Type LengthSqr(const Vector2<Type>& v);

		// Assignment operator.
		Vector2<Type>& operator=(const Vector2<Type>& other);

		// Binary arithmetic operators.
		Vector2<Type>& operator+=(const Vector2<Type>& rhs);
		Vector2<Type>& operator-=(const Vector2<Type>& rhs);
		template<class Type2> Vector2<Type>& operator*=(const Type2& rhs);
		template<class Type2> Vector2<Type>& operator/=(const Type2& rhs);

		Vector2<Type> operator+(const Vector2<Type>& rhs) const;
		Vector2<Type> operator-(const Vector2<Type>& rhs) const;
		template<class Type2> Vector2<Type> operator*(const Type2& rhs) const;
		template<class Type2> Vector2<Type> operator/(const Type2& rhs) const;

		// Comparison operators.
		template<class Type2> bool operator==(const Type2& rhs) const;
	};

	template <class Type> Vector2<Type>::Vector2() : x(0), y(0)
	{
		// Default constructor.
	}
	template <class Type> Vector2<Type>::Vector2(Type x, Type y) : x(x), y(y)
	{
		// Default constructor.
	}

	// Returns the dot product of vectors A and B.
	template <class Type> Type Vector2<Type>::Dot(const Vector2<Type>& a, const Vector2<Type>& b)
	{
		return a.x * b.x + a.y * b.y;
	}
	// Returns a normalised vector based on the input vector.
	template <class Type> Vector2<Type> Vector2<Type>::Normalise(const Vector2<Type>& v)
	{
		float length = Length(v);
		return Vector2<Type>(v.x / length, v.y / length);
	}
	// Returns a vector interpolated linearly between A and B using a factor of T.
	template <class Type> Vector2<Type> Vector2<Type>::Lerp(const Vector2<Type>& a, const Vector2<Type>& b, const Type& t)
	{
		// Interpolate for x and y.
		float x = a.x + t * (b.x - a.x);
		float y = a.y + t * (b.y - a.y);

		// Return interpolated vector.
		return Vector2<Type>(x, y);
	}

	// Returns the length of the target vector.
	template <class Type> auto Vector2<Type>::Length(const Vector2<Type>& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}
	// Returns the squared vector length.
	template <class Type> Type Vector2<Type>::LengthSqr(const Vector2<Type>& v)
	{
		return v.x * v.x + v.y * v.y;
	}

	template <class Type> Vector2<Type>& Vector2<Type>::operator=(const Vector2<Type>& other)
	{
		// Set components based on other, return pointer.
		x = other.x;
		y = other.y;
		return *this;
	}

	template <class Type> Vector2<Type>& Vector2<Type>::operator+=(const Vector2<Type>& rhs)
	{
		// Add components from rhs into current vector, return pointer.
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	template <class Type> Vector2<Type>& Vector2<Type>::operator-=(const Vector2<Type>& rhs)
	{
		// Subtract components from rhs into current vector, return pointer.
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	template <class Type> template<class Type2> Vector2<Type>& Vector2<Type>::operator*=(const Type2& rhs)
	{
		// Multiply components of current vector by rhs, return pointer.
		x *= rhs;
		y *= rhs;
		return *this;
	}
	template <class Type> template<class Type2> Vector2<Type>& Vector2<Type>::operator/=(const Type2& rhs)
	{
		// Divide components of current vector by rhs, return pointer.
		x /= rhs;
		y /= rhs;
		return *this;
	}

	template <class Type> Vector2<Type> Vector2<Type>::operator+(const Vector2<Type>& rhs) const
	{
		// Add components from rhs into current vector, return value.
		return Vector2<Type>(x + rhs.x, y + rhs.y);
	}
	template <class Type> Vector2<Type> Vector2<Type>::operator-(const Vector2<Type>& rhs) const
	{
		// Subtract components from rhs into current vector, return value.
		return Vector2<Type>(x - rhs.x, y - rhs.y);
	}
	template <class Type> template<class Type2> Vector2<Type> Vector2<Type>::operator*(const Type2& rhs) const
	{
		// Multiply components of current vector by rhs, return value.
		return Vector2<Type>(x * rhs, y * rhs);
	}
	template <class Type> template<class Type2> Vector2<Type> Vector2<Type>::operator/(const Type2& rhs) const
	{
		// Divide components of current vector by rhs, return value.
		return Vector2<Type>(x / rhs, y / rhs);
	}

	template<class Type> template<class Type2> bool Vector2<Type>::operator==(const Type2& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	// Define Useful types.
	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
}

#endif //VECTOR_2_H_
