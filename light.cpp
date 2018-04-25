#include "light.h"
Light::Light(Vec3 position, Vec3 colors) {
    this->colors = colors * 255;
	this->position = position;
}
