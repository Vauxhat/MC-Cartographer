#ifndef VECTOR_3_H_
#define VECTOR_3_H_

#include <cmath>

namespace Vaux
{
	template <class Type = float> class Vector3
	{
	public:
		Type x, y, z;

	public:
		Vector3();
		Vector3(const Type& x, const Type& y, const Type& z);
		template<class Type2> Vector3(const Vector3<Type2>& v);

		// Vector functions.
		static Type Dot(const Vector3<Type>& a, const Vector3<Type>& b);
		static Vector3<Type> Cross(const Vector3<Type>& a, const Vector3<Type>& b);
		static Vector3<Type> Lerp(const Vector3<Type>& a, const Vector3<Type>& b, const Type& t);

		// Length functions.
		static auto Length(const Vector3<Type>& v);
		static Type LengthSqr(const Vector3<Type>& v);

		// Assignment operator.
		template<class Type2> Vector3<Type>& operator=(const Vector3<Type2>& other);

		// Binary arithmetic operators.
		template<class Type2> Vector3<Type>& operator+=(const Vector3<Type2>& rhs);
		template<class Type2> Vector3<Type>& operator-=(const Vector3<Type2>& rhs);
		template<class Type2> Vector3<Type>& operator*=(const Type2& rhs);
		template<class Type2> Vector3<Type>& operator/=(const Type2& rhs);

		template<class Type2> Vector3<Type> operator+(const Vector3<Type2>& rhs) const;
		template<class Type2> Vector3<Type> operator-(const Vector3<Type2>& rhs) const;
		template<class Type2> Vector3<Type> operator*(const Type2& rhs) const;
		template<class Type2> Vector3<Type> operator/(const Type2& rhs) const;

		// Comparison operators.
		template<class Type2> bool operator==(const Vector3<Type2>& rhs) const;
	};

	template <class Type> Vector3<Type>::Vector3() : x(0), y(0), z(0)
	{
		// Default constructor.
	}
	template <class Type> Vector3<Type>::Vector3(const Type& x, const Type& y, const Type& z) : x(x), y(y), z(z)
	{
		// Default constructor.
	}
	template <class Type> template <class Type2> Vector3<Type>::Vector3(const Vector3<Type2>& v) : x(v.x), y(v.y), z(v.z)
	{
		// Default constructor.
	}

	// Returns the dot product of vectors A and B.
	template <class Type> static Type Vector3<Type>::Dot(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	// Returns the cross product of vectors A and B.
	template <class Type> static Vector3<Type> Vector3<Type>::Cross(const Vector3<Type>& a, const Vector3<Type>& b)
	{
		Vector3<Type> c;
		c.x = a.y * b.z - a.z * b.y;
		c.y = a.z * b.x - a.x * b.z;
		c.z = a.x * b.y - a.y * b.x;
		return c;
	}
	// Returns a vector interpolated linearly between A and B using a factor of T.
	template <class Type> Vector3<Type> Vector3<Type>::Lerp(const Vector3<Type>& a, const Vector3<Type>& b, const Type& t)
	{
		Vector3<Type> c;
		c.x = a.x + t * (b.x - a.x);
		c.y = a.y + t * (b.y - a.y);
		c.z = a.z + t * (b.z - a.z);
		return c;
	}

	// Returns the length of the target vector.
	template <class Type> auto Vector3<Type>::Length(const Vector3<Type>& v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	// Returns the squared lengthd of the target vector.
	template <class Type> Type Vector3<Type>::LengthSqr(const Vector3<Type>& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	template<class Type> template <class Type2> Vector3<Type>& Vector3<Type>::operator=(const Vector3<Type2>& other)
	{
		// Set components based on other, return pointer.
		x = static_cast<Type>(other.x);
		y = static_cast<Type>(other.y);
		z = static_cast<Type>(other.z);
		return *this;
	}

	template <class Type> template <class Type2> Vector3<Type>& Vector3<Type>::operator+=(const Vector3<Type2>& rhs)
	{
		// Add components from rhs into current vector, return pointer.
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	template <class Type> template <class Type2> Vector3<Type>& Vector3<Type>::operator-=(const Vector3<Type2>& rhs)
	{
		// Subtract components from rhs into current vector, return pointer.
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	template <class Type> template <class Type2> Vector3<Type>& Vector3<Type>::operator*=(const Type2& rhs)
	{
		// Multiply components of current vector by rhs, return pointer.
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	template <class Type> template <class Type2> Vector3<Type>& Vector3<Type>::operator/=(const Type2& rhs)
	{
		// Multiply components of current vector by rhs, return pointer.
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	template <class Type> template <class Type2> Vector3<Type> Vector3<Type>::operator+(const Vector3<Type2>& rhs) const
	{
		// Add components from rhs into current vector, return value.
		return Vector3<Type>(static_cast<Type>(x + rhs.x), static_cast<Type>(y + rhs.y), static_cast<Type>(z + rhs.z));
	}
	template <class Type> template <class Type2> Vector3<Type> Vector3<Type>::operator-(const Vector3<Type2>& rhs) const
	{
		// Subtract components from rhs into current vector, return value.
		return Vector3<Type>(static_cast<Type>(x - rhs.x), static_cast<Type>(y - rhs.y), static_cast<Type>(z - rhs.z));
	}
	template <class Type> template <class Type2> Vector3<Type> Vector3<Type>::operator*(const Type2& rhs) const
	{
		//Type factor = static_cast<Type>(rhs);

		// Multiply components of current vector by rhs, return value.
		return Vector3<Type>(static_cast<Type>(x * rhs), static_cast<Type>(y * rhs), static_cast<Type>(z * rhs));
	}
	template <class Type> template<class Type2> Vector3<Type> Vector3<Type>::operator/(const Type2& rhs) const
	{
		// Divide components of current vector by rhs, return value.
		return Vector3<Type>(static_cast<Type>(x / rhs), static_cast<Type>(y / rhs), static_cast<Type>(z / rhs));
	}

	template<class Type> template<class Type2> bool Vector3<Type>::operator==(const Vector3<Type2>& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	// Define Useful types.
	typedef Vector3<int> Vector3i;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
}

#endif //VECTOR_3_H_
