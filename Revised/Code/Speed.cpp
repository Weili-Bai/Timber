#include "Speed.h"
Speed::Speed(float xUpper, float yUpper) {
	int temp = 0 + rand() % 100;
	float ratio = temp * 1.0 / 101;
	x = xUpper * ratio;
	temp = 0 + rand() % 101;
	ratio = temp * 1.0 / 100;
	y = yUpper * ratio;
}