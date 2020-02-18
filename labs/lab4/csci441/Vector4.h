#ifndef _CSCI441_VECTOR4_
#define _CSCI441_VECTOR4_

#include <string>

class Vector4 {
public:
	float v[4];

	Vector4() {
		for (int i = 0; i < 4; i++) {
			v[i] = 0;
		}
	}

	void setArray(float temp[]) {
		for (int i = 0; i < 4; i++) {
			v[i] = temp[i];
		}
	}

	void toZero() {
		for (int i = 0; i < 4; i++) {
			v[i] = 0;
		}
	}

	float magnitude() {
		float temp = 0;
		for (int i = 0; i < 4; i++) {
			temp += (v[i] * v[i]);
		}
		return sqrt(temp);
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
};

#endif