#pragma once

#include<cmath>

class Vector2
{
public:
	float x = 0;
	float y = 0;

public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y) : x(x), y(y) {}

	Vector2 operator + (const Vector2& vec) const {
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator += (const Vector2& vec) {
		x += vec.x, y += vec.y;
	}

	void operator -= (const Vector2& vec) {
		x -= vec.x, y -= vec.y;
	}

	Vector2 operator - (const Vector2& vec) const {
		return Vector2(x - vec.x, y - vec.y);
	}

	friend Vector2 operator-(const Vector2& lhs, float rhs) {
		return Vector2(lhs.x - rhs, lhs.y - rhs);
	}

	friend Vector2 operator-(float lhs, const Vector2& rhs) {
		return Vector2(lhs - rhs.x, lhs - rhs.y);
	}

	Vector2 operator - (int val) const {
		return Vector2(x - val, y - val);
	}

	friend Vector2 operator*(const Vector2& lhs, float val) {
		return Vector2(lhs.x * val, lhs.y * val);
	}

	friend Vector2 operator*(float val, const Vector2& rhs) {
		return Vector2(rhs.x * val, rhs.y * val);
	}

	Vector2 operator*(const Vector2& rhs) const {
		return Vector2(x * rhs.x, y * rhs.y);
	}

	Vector2 operator*(int val) const {
		return Vector2(x * val, y * val);
	}

	void operator *= (float val) {
		x *= val, y *= val;
	}

	float length() {
		return sqrt(x * x + y * y);
	}

	Vector2 normalize() {
		float len = length();
		if (len == 0) {
			return Vector2(0, 0);
		}
		return Vector2(x / len, y / len);
	}
	// 点积
	float dot(const Vector2& other) const {
		return x * other.x + y * other.y;
	}
	// 弧度制旋转（返回新向量）
	Vector2 rotate(float radians) const {
		float cosVal = cos(radians);
		float sinVal = sin(radians);
		return Vector2(
			x * cosVal - y * sinVal,
			x * sinVal + y * cosVal
		);
	}

};
