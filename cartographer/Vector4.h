#ifndef VECTOR_4_H_
#define VECTOR_4_H_

#include <cmath>

namespace Vaux
{
	template <class Type = float> class Vector3;

	template <class Type = float> class Vector4
	{
	public:
		Type x, y, z, w;

	public:
		Vector4();
		Vector4(const Type& x, const Type& y, const Type& z, const Type& w = 1);
		template<class Type2> Vector4(const Vector3<Type2>& v, const Type& w = 1);
		template<class Type2> Vector4(const Vector4<Type2>& v);

		// Vector functions.
		static Type Dot(const Vector4<Type>& a, const Vector4<Type>& b);
		static Vector4<Type> Lerp(const Vector4<Type>& a, const Vector4<Type>& b, const float& t);

		// Length functions.
		static auto Length(const Vector4<Type>& v);
		static Type LengthSqr(const Vector4<Type>& v);

		// Assignment operator.
		template<class Type2> Vector4<Type>& operator=(const Vector4<Type2>& other);

		// Binary arithmetic operators.
		template<class Type2> Vector4<Type>& operator+=(const Vector4<Type2>& rhs);
		template<class Type2> Vector4<Type>& operator-=(const Vector4<Type2>& rhs);
		template<class Type2> Vector4<Type>& operator*=(const Type2& rhs);
		template<class Type2> Vector4<Type>& operator/=(const Type2& rhs);

		template<class Type2> Vector4<Type> operator+(const Vector4<Type2>& rhs) const;
		template<class Type2> Vector4<Type> operator-(const Vector4<Type2>& rhs) const;
		template<class Type2> Vector4<Type> operator*(const Type2& rhs) const;
		template<class Type2> Vector4<Type> operator/(const Type2& rhs) const;

		// Comparison operators.
		template<class Type2> bool operator==(const Vector4<Type2>& rhs) const;
	};

	template <class Type> Vector4<Type>::Vector4() : x(0), y(0), z(0), w(1)
	{
		// Default constructor.
	}
	template <class Type> Vector4<Type>::Vector4(const Type& x, const Type& y, const Type& z, const Type& w) : x(x), y(y), z(z), w(w)
	{
		// Default constructor.
	}
	template <class Type> template <class Type2> Vector4<Type>::Vector4(const Vector4<Type2>& v) : x(v.x), y(v.y), z(v.z), w(v.w)
	{
		// Default constructor.
	}
	template <class Type> template<class Type2> Vector4<Type>::Vector4(const Vector3<Type2>& v, const Type& w) : x(v.x), y(v.y), z(v.z), w(w)
	{
		// Default constructor.
	}

	// Returns the dot product of vectors A and B.
	template <class Type> static Type Vector4<Type>::Dot(const Vector4<Type>& a, const Vector4<Type>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	// Returns a vector interpolated linearly between A and B using a factor of T.
	template <class Type> Vector4<Type> Vector4<Type>::Lerp(const Vector4<Type>& a, const Vector4<Type>& b, const float& t)
	{
		Vector4<Type> c;
		c.x = static_cast<Type>(a.x + t * (b.x - a.x));
		c.y = static_cast<Type>(a.y + t * (b.y - a.y));
		c.z = static_cast<Type>(a.z + t * (b.z - a.z));
		c.w = static_cast<Type>(a.w + t * (b.w - a.w));
		return c;
	}

	// Returns the length of the target vector.
	template <class Type> auto Vector4<Type>::Length(const Vector4<Type>& v)
	{
		return sqrt(LengthSqr());
	}
	// Returns the squared lengthd of the target vector.
	template <class Type> Type Vector4<Type>::LengthSqr(const Vector4<Type>& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}

	template<class Type> template <class Type2> Vector4<Type>& Vector4<Type>::operator=(const Vector4<Type2>& other)
	{
		// Set components based on other, return pointer.
		x = static_cast<Type>(other.x);
		y = static_cast<Type>(other.y);
		z = static_cast<Type>(other.z);
		w = static_cast<Type>(other.w);
		return *this;
	}

	template <class Type> template <class Type2> Vector4<Type>& Vector4<Type>::operator+=(const Vector4<Type2>& rhs)
	{
		// Add components from rhs into current vector, return pointer.
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	template <class Type> template <class Type2> Vector4<Type>& Vector4<Type>::operator-=(const Vector4<Type2>& rhs)
	{
		// Subtract components from rhs into current vector, return pointer.
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	template <class Type> template <class Type2> Vector4<Type>& Vector4<Type>::operator*=(const Type2& rhs)
	{
		//Type factor = static_cast<Type>(rhs);

		// Multiply components of current vector by rhs, return pointer.
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}
	template <class Type> template <class Type2> Vector4<Type>& Vector4<Type>::operator/=(const Type2& rhs)
	{
		//Type factor = static_cast<Type>(rhs);

		// Multiply components of current vector by rhs, return pointer.
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	template <class Type> template <class Type2> Vector4<Type> Vector4<Type>::operator+(const Vector4<Type2>& rhs) const
	{
		// Add components from rhs into current vector, return value.
		return Vector4<Type>(static_cast<Type>(x + rhs.x), static_cast<Type>(y + rhs.y), static_cast<Type>(z + rhs.z), static_cast<Type>(w + rhs.w));
	}
	template <class Type> template <class Type2> Vector4<Type> Vector4<Type>::operator-(const Vector4<Type2>& rhs) const
	{
		// Subtract components from rhs into current vector, return value.
		return Vector4<Type>(static_cast<Type>(x - rhs.x), static_cast<Type>(y - rhs.y), static_cast<Type>(z - rhs.z), static_cast<Type>(w - rhs.w));
	}
	template <class Type> template <class Type2> Vector4<Type> Vector4<Type>::operator*(const Type2& rhs) const
	{
		//Type factor = static_cast<Type>(rhs);

		// Multiply components of current vector by rhs, return value.
		return Vector4<Type>(static_cast<Type>(x * rhs), static_cast<Type>(y * rhs), static_cast<Type>(z * rhs), static_cast<Type>(w * rhs));
	}
	template <class Type> template<class Type2> Vector4<Type> Vector4<Type>::operator/(const Type2& rhs) const
	{
		//Type factor = static_cast<Type>(rhs);

		// Divide components of current vector by rhs, return value.
		return Vector4<Type>(static_cast<Type>(x / rhs), static_cast<Type>(y / rhs), static_cast<Type>(z / rhs), static_cast<Type>(w / rhs));
	}

	template<class Type> template<class Type2> bool Vector4<Type>::operator==(const Vector4<Type2>& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
	}

	// Define Useful types.
	typedef Vector4<int> Vector4i;
	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
}

#endif //VECTOR_3_H_
