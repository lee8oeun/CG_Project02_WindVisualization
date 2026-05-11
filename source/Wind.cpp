#include <iostream>
#include <GL/gl3w.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

//입자 여러 개 저장할 vector
std::vector<Particle> particles;

//입자 위치 저장
struct Particle {
	float x;
	float y;
};

//좌표[-1,1] 에 맞게 -1~1 사이의 랜덤값 생성
float randomFloat(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

const int particleCount = 500;




