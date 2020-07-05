#pragma once

#include "PepperMint/Renderer/OrthographicCamera.h"
#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/Events/MouseEvent.h"

namespace PepperMint {

class OrthographicCameraController {
public:
	OrthographicCameraController(float iAspectRatio, bool iRotation = false);

	~OrthographicCameraController() = default;

	void onUpdate(Timestep iTimestep);
	void onEvent(Event& iEvent);

	void onResize(float iWidth, float iHeight);

	inline OrthographicCamera& camera() { return _camera; }
	inline const OrthographicCamera& camera() const { return _camera; }

	inline float zoomLevel() const { return _zoomLevel; }
	inline void setZoomLevel(float iZoomLevel) { _zoomLevel = iZoomLevel; }

private:
	bool onMouseScrolled(MouseScrolledEvent& iEvent);
	bool onWindowResize(WindowResizeEvent& iEvent);

private:
	OrthographicCamera _camera;
	float _aspectRatio;
	float _zoomLevel = 1.0f;

	bool _rotation;

	glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
	float _cameraRotation = 0.0f;  // In degrees, in the anti-clockwise direction
	float _cameraTranslationSpeed = 5.0f;
	float _cameraRotationSpeed = 180.0f;
};

}