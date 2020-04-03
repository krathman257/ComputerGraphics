#ifndef _CSCI441_VECTOR3_
#define _CSCI441_VECTOR3_

#include <string>

class Vector3 {
public:
	float v[3];

	Vector3() {
		for (int i = 0; i < 3; i++) {
			v[i] = 0;
		}
	}

	Vector3(float x, float y, float z) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	float magnitude() {
		float temp = 0;
		for (int i = 0; i < 3; i++) {
			temp += (v[i] * v[i]);
		}
		return sqrt(temp);
	}

	Vector3 normalize() {
		float temp = magnitude();
		if (temp == 0) {
			return Vector3(0, 0, 0);
		}
		return *this / temp;
	}

	Vector3 crossProduct(Vector3 v1) {
		Vector3 temp;
		temp.v[0] = this->v[1] * v1.v[2] - this->v[2] * v1.v[1];
		temp.v[1] = this->v[2] * v1.v[0] - this->v[0] * v1.v[2];
		temp.v[2] = this->v[0] * v1.v[1] - this->v[1] * v1.v[0];
		return temp;
	}

	void setArray(float temp[]) {
		for (int i = 0; i < 3; i++) {
			v[i] = temp[i];
		}
	}

	void toZero() {
		for (int i = 0; i < 3; i++) {
			v[i] = 0;
		}
	}

	std::string toString() {
		std::string temp = "";
		for (int i = 0; i < 3; i++) {
			temp += std::to_string(v[i]);
			if (i != 2) {
				temp += ", ";
			}
		}
		return temp;
	}

	Vector3 operator/(float d) {
		Vector3 temp;
		for (int i = 0; i < 3; i++) {
			temp.v[i] = this->v[i] / d;
		}
		return temp;
	}

	void operator+=(Vector3 v1) {
		for (int i = 0; i < 3; i++) {
			this->v[i] = this->v[i] + v1.v[i];
		}
	}

	Vector3 operator-(Vector3 v1) {
		Vector3 temp;
		for (int i = 0; i < 3; i++) {
			temp.v[i] = this->v[i] - v1.v[i];
		}
		return temp;
	}
};

#endif