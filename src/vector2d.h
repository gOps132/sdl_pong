#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include <cmath>

struct Vector2D
{
public:
	float m_x;
	float m_y;
	
	Vector2D() : m_x(0.0f), m_y(0.0f) {}	
	Vector2D(float p_x, float p_y) : m_x(p_x), m_y(p_y) {}
	Vector2D(Vector2D& p_vector) : m_x(p_vector.m_x), m_y(p_vector.m_y) {}

	inline float dot(const Vector2D &p_vector) const
	{
		return m_x * p_vector.m_x + m_y * p_vector.m_y;
	}

	// subtract 2 vectors
	Vector2D operator-(const Vector2D &p_vector) const {
		return {m_x - p_vector.m_x, m_y - p_vector.m_y};
	}
	
	// multiply vectors by scalar
	Vector2D operator*(const float scalar) const 
	{
		return {m_x * scalar, m_y * scalar};
	}

	// add 2 vectors
	Vector2D operator+(const Vector2D &p_vector) const {
		return {m_x + p_vector.m_x, m_y + p_vector.m_y};
	}

	// normalize a vector
	void normalize()
	{
		float magnitude = std::sqrt(m_x * m_x + m_y * m_y);
		m_x /= magnitude;
		m_y /= magnitude;
	}
	
};

#endif // __VECTOR2D_H__