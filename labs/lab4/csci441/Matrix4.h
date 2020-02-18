#ifndef _CSCI441_MATRIX4_
#define _CSCI441_MATRIX4_

#include "Vector3.h"
#include "Vector4.h"

class Matrix4 {
private:
	const float PI = 3.141592653589793;

public:
	float m[16];

	Matrix4() {
		for (int i = 0; i < 16; i++) {
			m[i] = 0;
		}
	}

	Matrix4(float input[]) {
		setArray(input);
	}

	void setArray(float temp[]) {
		for (int i = 0; i < 16; i++) {
			m[i] = temp[i];
		}
	}

	void toZero() {
		for (int i = 0; i < 16; i++) {
			m[i] = 0;
		}
	}

	void toIdent() {
		toZero();
		m[0] = 1;
		m[5] = 1;
		m[10] = 1;
		m[15] = 1;
	}

	void toOrth(float l, float r, float b, float t, float n, float f) {
		float temp[] = {
			2 / (r - l), 0, 0, -1 * (r + l) / (r - l),
			0, 2 / (t - b), 0, -1 * (t + b) / (t - b),
			0, 0, 2 / (f - n), -1 * (f + n) / (f - n),
			0, 0, 0, 1
		};
		setArray(temp);
	}

	void toCam(Vector3 e, Vector3 g, Vector3 t) {
		Vector3 w = g.normalize();
		Vector3 crossTW = t.crossProduct(w);
		Vector3 u = crossTW.normalize();
		Vector3 v = w.crossProduct(u);

		float a = g.v[0], b = g.v[1], c = g.v[2];

		float tempR[] = {
			cos(a) * cos(b), cos(a) * sin(b) * sin(c) - sin(a) * cos(c), cos(a) * sin(b) * cos(c) + sin(a) * sin(c), 0,
			sin(a) * cos(b), sin(a) * sin(b) * sin(c) + cos(a) * cos(c), sin(a) * sin(b) * cos(c) - cos(a) * sin(c), 0,
			-1 * sin(b), cos(b) * sin(c), cos(b) * cos(c), 1,
			0, 0, 0, 1
		};
		float tempT[] = {
			1, 0, 0, -1 * e.v[0],
			0, 1, 0, -1 * e.v[1],
			0, 0, 1, -1 * e.v[2],
			0, 0, 0, 1
		};
		Matrix4 mR(tempR);
		Matrix4 mT(tempT);
		Matrix4 final = mT * mR;
		setArray(final.m);
	}

	void toPerspective(float n, float f, float fov) {
		float s = 1 / (tan(fov * PI / 360));
		float temp[] = {
			s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, f / (f - n), -1,
			0, 0, f * n / (f - n), 0
		};
		setArray(temp);
	}

	void toScale(float s) {
		for (int i = 0; i < 16; i++) {
			if (i == 0 || i == 5 || i == 10) {
				m[i] = s;
			}
			else if(i == 15) {
				m[i] = 1;
			}
			else {
				m[i] = 0;
			}
		}
	}
	
	void toTranslate(float x, float y, float z) {
		float temp[] = {
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		};
		setArray(temp);
	}

	std::string toString() {
		std::string temp = "";
		for (int i = 0; i < 16; i++) {
			temp += std::to_string(m[i]);
			if (i != 15) {
				temp += ", ";
			}
			if (i == 3 || i == 7 || i == 11 || i == 15) {
				temp += '\n';
			}
		}
		return temp;
	}

	Vector4 operator*(Vector4 v) {
		Vector4 temp;
		for (int i = 0; i < 4; i++) {
			temp.v[i] = this->m[i * 4] * v.v[0] +
				this->m[i * 4 + 1] * v.v[1] +
				this->m[i * 4 + 2] * v.v[2] +
				this->m[i * 4 + 3] * v.v[3];
		}
		return temp;
	}

	Matrix4 operator*(Matrix4 m2) {
		Matrix4 temp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.m[i * 4 + j] = this->m[i * 4] * m2.m[j] +
					this->m[i * 4 + 1] * m2.m[j + 4] +
					this->m[i * 4 + 2] * m2.m[j + 8] +
					this->m[i * 4 + 3] * m2.m[j + 12];
			}
		}
		return temp;
	}
};

#endif
