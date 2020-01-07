#include "pmpch.h"

#include "OrthographicCameraController.h"

#include "Input.h"
#include "KeyCodes.h"

namespace PepperMint {

void OrthographicCameraController::onUpdate(Timestep iTimestep) {
	if (Input::IsKeyPressed(PM_KEY_A)) {
		_cameraPosition.x -= _cameraTranslationSpeed * iTimestep;
	} else if (Input::IsKeyPressed(PM_KEY_D)) {
		_cameraPosition.x += _cameraTranslationSpeed * iTimestep;
	}

	if (Input::IsKeyPressed(PM_KEY_W)) {
		_cameraPosition.y += _cameraTranslationSpeed * iTimestep;
	} else if (Input::IsKeyPressed(PM_KEY_S)) {
		_cameraPosition.y -= _cameraTranslationSpeed * iTimestep;
	}

	_camera.setPosition(_cameraPosition);
	_cameraTranslationSpeed = _zoomLevel;

	if (_rotation) {
		if (Input::IsKeyPressed(PM_KEY_Q)) {
			_cameraRotation += _cameraRotationSpeed * iTimestep;
		}
		if (Input::IsKeyPressed(PM_KEY_E)) {
			_cameraRotation -= _cameraRotationSpeed * iTimestep;
		}

		_camera.setRotation(_cameraRotation);
	}
}

void OrthographicCameraController::onEvent(Event& iEvent) {
	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<MouseScrolledEvent>(PM_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
	dispatcher.dispatch<WindowResizeEvent>(PM_BIND_EVENT_FN(OrthographicCameraController::onWindowResize));
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& iEvent) {
	_zoomLevel -= iEvent.yOffset() * 0.25f;
	_zoomLevel = std::max(_zoomLevel, 0.25f);
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	return false;
}

bool OrthographicCameraController::onWindowResize(WindowResizeEvent& iEvent) {
	_aspectRatio = (float)iEvent.width() / (float)iEvent.height();
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	return false;
}
}