#include "camera.h"

Camera* Camera::manager = nullptr;

Camera* Camera::instance() {
	if (!manager)
		manager = new Camera();
	return manager;
}