#ifndef _CSCI441_VECTOR3_
#define _CSCI441_VECTOR3_

class Vector3 {
public:
	float v[3];

	Vector3() {
		for (int i = 0; i < 3; i++) {
			v[i] = 0;
		}
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
};

#endif