#ifndef _CSCI441_CYLINDER_
#define _CSCI441_CYLINDER_

class Cylinder {
private:
	const float PI = 3.141592653589793;
public:
	//Res=10
	float v[720];

	Cylinder(float x, float y, float h, float r) {
		
		//Bottom face
		for (int i = 0; i < 180; i+=18) {
			float theta = 2.0 * PI * (float(i) / 18.0f) / 10.0f;
			v[i] = r * cos(theta);
			v[i + 1] = r * sin(theta);
			v[i + 2] = h / -2.0f;
			v[i + 3] = 1;
			v[i + 4] = 0;
			v[i + 5] = 0;

			theta = 2.0 * PI * (float(i + 18) / 18.0f) / 10.0f;
			v[i + 6] = r * cos(theta);
			v[i + 7] = r * sin(theta);
			v[i + 8] = h / -2.0f;
			v[i + 9] = 1;
			v[i + 10] = 0;
			v[i + 11] = 0;
			
			v[i + 12] = 0;
			v[i + 13] = 0;
			v[i + 14] = h / -2.0f;
			v[i + 15] = 1;
			v[i + 16] = 0;
			v[i + 17] = 0;
		}

		//Top face
		for (int i = 180; i < 360; i += 18) {
			float theta = 2.0 * PI * (float(i - 180) / 18.0f) / 10.0f;
			v[i] = r * cos(theta);
			v[i + 1] = r * sin(theta);
			v[i + 2] = h / 2.0f;
			v[i + 3] = 0;
			v[i + 4] = 0;
			v[i + 5] = 1;

			theta = 2.0 * PI * (float(i - 162) / 18.0f) / 10.0f;
			v[i + 6] = r * cos(theta);
			v[i + 7] = r * sin(theta);
			v[i + 8] = h / 2.0f;
			v[i + 9] = 0;
			v[i + 10] = 0;
			v[i + 11] = 1;

			v[i + 12] = 0;
			v[i + 13] = 0;
			v[i + 14] = h / 2.0f;
			v[i + 15] = 0;
			v[i + 16] = 0;
			v[i + 17] = 1;
		}

		//Sides
		for (int i = 360; i < 720; i += 36) {
			float theta = 2.0 * PI * ((float(i) / 2.0f - 360.0f) / 18.0f) / 10.0f;
			v[i] = r * cos(theta);
			v[i + 1] = r * sin(theta);
			v[i + 2] = h / 2.0f;
			v[i + 3] = 0;
			v[i + 4] = 1;
			v[i + 5] = 0;

			theta = 2.0 * PI * ((float(i) / 2.0f - 342.0f) / 18.0f) / 10.0f;
			v[i + 6] = r * cos(theta);
			v[i + 7] = r * sin(theta);
			v[i + 8] = h / 2.0f;
			v[i + 9] = 0;
			v[i + 10] = 1;
			v[i + 11] = 0;

			theta = 2.0 * PI * ((float(i) / 2.0f - 342.0f) / 18.0f) / 10.0f;
			v[i + 12] = r * cos(theta);
			v[i + 13] = r * sin(theta);
			v[i + 14] = h / -2.0f;
			v[i + 15] = 0;
			v[i + 16] = 1;
			v[i + 17] = 0;

			theta = 2.0 * PI * ((float(i) / 2.0f - 342.0f) / 18.0f) / 10.0f;
			v[i + 18] = r * cos(theta);
			v[i + 19] = r * sin(theta);
			v[i + 20] = h / -2.0f;
			v[i + 21] = 0;
			v[i + 22] = 1;
			v[i + 23] = 0;

			theta = 2.0 * PI * ((float(i) / 2.0f - 360.0f) / 18.0f) / 10.0f;
			v[i + 24] = r * cos(theta);
			v[i + 25] = r * sin(theta);
			v[i + 26] = h / -2.0f;
			v[i + 27] = 0;
			v[i + 28] = 1;
			v[i + 29] = 0;

			theta = 2.0 * PI * ((float(i) / 2.0f - 360.0f) / 18.0f) / 10.0f;
			v[i + 30] = r * cos(theta);
			v[i + 31] = r * sin(theta);
			v[i + 32] = h / 2.0f;
			v[i + 33] = 0;
			v[i + 34] = 1;
			v[i + 35] = 0;
		}
		
		//Position
		for (int i = 0; i < 720; i++) {
			if (i % 6 == 0) {
				v[i] += x;
			}
			else if (i % 6 == 1) {
				v[i] += y;
			}
		}
	}

	void setArray(float input[]) {
		for (int i = 0; i < 720; i++) {
			v[i] = input[i];
		}
	}

	std::string toString() {
		std::string temp = "";
		for (int i = 0; i < 720; i++) {
			temp += std::to_string(v[i]);
			if (i != 719) {
				temp += ", ";
			}
			if (i % 6 == 5) {
				temp += '\n';
			}
			if (i % 18 == 17) {
				temp += '\n';
			}
		}
		return temp;
	}
};

#endif