#include <iostream>
#include <string>
using namespace std;

class Vector3 {
public:
    float x;
    float y;
    float z;
	
	Vector3() : x(0), y (0), z(0) {
		//cout << "in Vector3 constructor" << endl;
	}

    Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {
        //cout << "in Vector3 constructor" << endl;
    }

    ~Vector3() {
        //cout << "in Vector3 destructor" << endl;
    }
	
	Vector3 add(Vector3& v, Vector3& v2) {
		float x = v.x + v2.x;
		float y = v.y + v2.y;
		float z = v.z + v2.z;
		
		return Vector3 (x,y,z);
	}
};

Vector3 operator+(Vector3& v, Vector3& v2) {
	Vector3 v3 = v3.add(v,v2);
	return v3;
}

ostream& operator<<(ostream& stream, const Vector3& v) {
	stream << v.x << ", " << v.y << ", " << v.z;
    return stream;
}

int main(int argc, char** argv){
	
	//4a
	for(int i = 0; i < argc; i++){
		cout << argv[i] << endl;
	}
	
	//4b
	cout << "What is your name?" << endl;
	string name;
	cin >> name;
	cout << "Hello, " << name << endl;
	
	//5
	Vector3 a(1,2,3);
    Vector3 b(4,5,6);
	
    Vector3 c = c.add(a,b);
	cout << c.x << ", " << c.y << ", " << c.z << endl;
	
	//6
	Vector3 v(1,2,3);
	Vector3 v2(4,5,6);
	cout << v+v2 << endl;
	
	//7
	Vector3* d = new Vector3();
	d->y = 5;
	cout << *d << endl;
	
	//8
	Vector3 array[10];
	
	Vector3* array_of_vectors = new Vector3[10];
	for(int i = 0; i < 10; i++){
		array_of_vectors[i].y = 5;
	}
	for(int i = 0; i < 10; i++){
		cout << array_of_vectors[i] << endl;
	}
}