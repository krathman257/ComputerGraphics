#ifndef _CSCI441_CUBE_
#define _CSCI441_CUBE_

class Cube {
public:
	float v[216];

	Cube(float x, float y, float a) {
		float b = -1 * a;
		float temp[] = {

			//Side 0
			b, b, b, 0.0f, 0.0f, 1.0f,
			a, b, b, 0.0f, 0.0f, 1.0f,
			a, a, b, 0.0f, 0.0f, 1.0f,
			a, a, b, 0.0f, 0.0f, 1.0f,
			b, a, b, 0.0f, 0.0f, 1.0f,
			b, b, b, 0.0f, 0.0f, 1.0f,

			//Side 1
			b, b, a, 0.0f, 0.0f, 1.0f,
			a, b, a, 0.0f, 0.0f, 1.0f,
			a, a, a, 0.0f, 0.0f, 1.0f,
			a, a, a, 0.0f, 0.0f, 1.0f,
			b, a, a, 0.0f, 0.0f, 1.0f,
			b, b, a, 0.0f, 0.0f, 1.0f,

			//Side 2
			b, a, a, 1.0f, 0.0f, 0.0f,
			b, a, b, 1.0f, 0.0f, 0.0f,
			b, b, b, 1.0f, 0.0f, 0.0f,
			b, b, b, 1.0f, 0.0f, 0.0f,
			b, b, a, 1.0f, 0.0f, 0.0f,
			b, a, a, 1.0f, 0.0f, 0.0f,

			//Side 3
			a, a, a, 1.0f, 0.0f, 0.0f,
			a, a, b, 1.0f, 0.0f, 0.0f,
			a, b, b, 1.0f, 0.0f, 0.0f,
			a, b, b, 1.0f, 0.0f, 0.0f,
			a, b, a, 1.0f, 0.0f, 0.0f,
			a, a, a, 1.0f, 0.0f, 0.0f,

			//Side 4
			b, b, b, 0.0f, 1.0f, 0.0f,
			a, b, b, 0.0f, 1.0f, 0.0f,
			a, b, a, 0.0f, 1.0f, 0.0f,
			a, b, a, 0.0f, 1.0f, 0.0f,
			b, b, a, 0.0f, 1.0f, 0.0f,
			b, b, b, 0.0f, 1.0f, 0.0f,

			//Side 5
			b, a, b, 0.0f, 1.0f, 0.0f,
			a, a, b, 0.0f, 1.0f, 0.0f,
			a, a, a, 0.0f, 1.0f, 0.0f,
			a, a, a, 0.0f, 1.0f, 0.0f,
			b, a, a, 0.0f, 1.0f, 0.0f,
			b, a, b, 0.0f, 1.0f, 0.0f,
		};

		setArray(temp);
		
		//Position
		for (int i = 0; i < 216; i++) {
			if (i % 6 == 0) {
				v[i] += x;
			}
			else if (i % 6 == 1) {
				v[i] += y;
			}
		}
	}

	void setArray(float input[]) {
		for (int i = 0; i < 216; i++) {
			v[i] = input[i];
		}
	}

	//Choose side 0-5
	void setColor(float side, float r, float g, float b) {
		for (int i = 0; i < 216; i++) {
			if (side * 36 <= i && i < (side + 1) * 36 && i % 6 == 3) {
				v[i] = r;
				v[i + 1] = g;
				v[i + 2] = b;
			}
		}
	}

	std::string toString() {
		std::string temp = "";
		for (int i = 0; i < 216; i++) {
			temp += std::to_string(v[i]);
			if (i != 215) {
				temp += ", ";
			}
			if (i % 6 == 5) {
				temp += '\n';
			}
			if (i % 36 == 35) {
				temp += '\n';
			}
		}
		return temp;
	}
};

#endif