#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace PepperMint {

class OrthographicCameraController {
public:
	OrthographicCameraController(float iAspectRatio, bool iRotation = false) :
		_aspectRatio(iAspectRatio),
		_camera(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel),
		_rotation(iRotation) {}

	~OrthographicCameraController() = default;

	void onUpdate(Timestep iTimestep);
	void onEvent(Event& iEvent);

	inline OrthographicCamera& camera() { return _camera; }
	inline const OrthographicCamera& camera() const { return _camera; }

private:
	bool onMouseScrolled(MouseScrolledEvent& iEvent);
	bool onWindowResize(WindowResizeEvent& iEvent);

private:
	OrthographicCamera _camera;
	float _aspectRatio;
	float _zoomLevel = 1.0f;

	bool _rotation;

	glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
	float _cameraRotation = 0.0f;
	float _cameraTranslationSpeed = 5.0f;
	float _cameraRotationSpeed = 180.0f;
};

}