#ifndef _CSCI441_MATRIX3_
#define _CSCI441_MATRIX3_

#include "Vector3.h"

class Matrix3 {
private:
	const float PI = 3.141592653589793;

public:
	float m[9];

	Matrix3() {
		for (int i = 0; i < 9; i++) {
			m[i] = 0;
		}
	}

	void setArray(float temp[]) {
		for (int i = 0; i < 9; i++) {
			m[i] = temp[i];
		}
	}

	Vector3 multiply(Vector3 v1) {
		Vector3 temp;
		for (int i = 0; i < 3; i++) {
			temp.v[i] = this->m[i * 3]     * v1.v[0] +
						this->m[i * 3 + 1] * v1.v[1] +
						this->m[i * 3 + 2] * v1.v[2];
		}
		return temp;
	}

	Matrix3 multiply(Matrix3 m2) {
		Matrix3 temp;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				temp.m[i * 3 + j] = this->m[i * 3]     * m2.m[j] +
									this->m[i * 3 + 1] * m2.m[j + 3] +
									this->m[i * 3 + 2] * m2.m[j + 6];
			}
		}
		return temp;
	}

	void toZero() {
		for (int i = 0; i < 9; i++) {
			m[i] = 0;
		}
	}

	void toIdent() {
		toZero();
		m[0] = 1;
		m[4] = 1;
		m[8] = 1;
	}

	void toScale(float s) {
		for (int i = 0; i < 9; i++) {
			if (i == 0 || i == 4 || i == 8) {
				m[i] = s;
			}
			else {
				m[i] = 0;
			}
		}
	}

	void toZRotation(float a) {
		a *= 180 / PI;
		float temp[] = {
			cos(a), -1 * sin(a), 0,
			sin(a),      cos(a), 0,
				 0,           0, 1
		};
		setArray(temp);
	}

	void toTranslate(float x, float y) {
		float temp[] = {
			1, 0, x,
			0, 1, y,
			0, 0, 1
		};
		setArray(temp);
	}

	std::string toString() {
		std::string temp = "";
		for (int i = 0; i < 9; i++) {
			temp += std::to_string(m[i]);
			if (i != 8) {
				temp += ", ";
			}
			if (i == 2 || i == 5 || i == 8) {
				temp += '\n';
			}
		}
		return temp;
	}
};

#endif
