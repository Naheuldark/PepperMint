#include "pmpch.h"
#include "PepperMint/Renderer/OrthographicCameraController.h"

#include "PepperMint/Core/Input.h"
#include "PepperMint/Core/KeyCodes.h"

namespace PepperMint {

OrthographicCameraController::OrthographicCameraController(float iAspectRatio, bool iRotation) : _aspectRatio(iAspectRatio), _rotation(iRotation) {
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
}

void OrthographicCameraController::onUpdate(Timestep iTimestep) {
	PM_PROFILE_FUNCTION();

	if (Input::IsKeyPressed(PM_KEY_A)) {
		_cameraPosition.x -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
		_cameraPosition.y -= sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
	} else if (Input::IsKeyPressed(PM_KEY_D)) {
		_cameraPosition.x += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
		_cameraPosition.y += sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
	}

	if (Input::IsKeyPressed(PM_KEY_W)) {
		_cameraPosition.x += -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
		_cameraPosition.y += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
	} else if (Input::IsKeyPressed(PM_KEY_S)) {
		_cameraPosition.x -= -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
		_cameraPosition.y -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * iTimestep;
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

		if (_cameraRotation > 180.f) {
			_cameraRotation -= 360.0f;
		} else if (_cameraRotation <= -180.0f) {
			_cameraRotation += 360.0f;
		}

		_camera.setRotation(_cameraRotation);
	}
}

void OrthographicCameraController::onEvent(Event& iEvent) {
	PM_PROFILE_FUNCTION();

	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<MouseScrolledEvent>(PM_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
	dispatcher.dispatch<WindowResizeEvent>(PM_BIND_EVENT_FN(OrthographicCameraController::onWindowResize));
}

void OrthographicCameraController::onResize(float iWidth, float iHeight) {
	_aspectRatio = iWidth / iHeight;
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& iEvent) {
	PM_PROFILE_FUNCTION();

	_zoomLevel -= iEvent.yOffset() * 0.25f;
	_zoomLevel = std::max(_zoomLevel, 0.25f);
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	return false;
}

bool OrthographicCameraController::onWindowResize(WindowResizeEvent& iEvent) {
	PM_PROFILE_FUNCTION();

	_aspectRatio = (float)iEvent.width() / (float)iEvent.height();
	_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	return false;
}
}