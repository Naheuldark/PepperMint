#include "Level.h"

#include "Random.h"

#include <glm/gtc/matrix_transform.hpp>

namespace {

glm::vec4 HSVtoRGB(const glm::vec3& iHSV) {
	int H = (int)(iHSV.x * 360.0f);
	double S = iHSV.y;
	double V = iHSV.z;

	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	} else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	} else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	} else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	} else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	} else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
}

bool pointInTriangle(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}
}

void Level::init() {
	// Player
	_player.init();

	// Pillars
	_pillarTexture = PepperMint::Texture2D::Create("assets/textures/Triangle.png");
	resetPillars();
}

void Level::onUpdate(PepperMint::Timestep iTimestep) {
	// Player
	_player.onUpdate(iTimestep);

	// Collision
	if (checkCollision()) {
		endLevel();
		return;
	}

	// Colors
	_pillarHSV.x += 0.1f * iTimestep;
	if (_pillarHSV.x > 1.0f) {
		_pillarHSV.x = 0.0f;
	}

	// Pillars
	if (_player.position().x > _pillarTarget) {
		createPillar(_pillarIndex, _pillarTarget + 20.0f);
		_pillarIndex = ++_pillarIndex % _pillars.size();
		_pillarTarget += 10.0f;
	}
}

void Level::onRender() {
	auto&& playerPos = _player.position();
	auto&& color = HSVtoRGB(_pillarHSV);

	// Background
	PepperMint::Renderer2D::DrawQuad({ playerPos.x, 0.0f, -0.8f }, 0.0f, { 50.0f, 50.0f }, 1.0f, nullptr, { 0.3f, 0.3f, 0.3f, 1.0f });

	// Floors + Ceiling
	PepperMint::Renderer2D::DrawQuad({ playerPos.x,  34.0f }, 0.0f, { 50.0f, 50.0f }, 1.0f, nullptr, color);
	PepperMint::Renderer2D::DrawQuad({ playerPos.x, -34.0f }, 0.0f, { 50.0f, 50.0f }, 1.0f, nullptr, color);

	// Pillars
	for (auto&& pillar : _pillars) {
		PepperMint::Renderer2D::DrawQuad(pillar.topPosition, glm::radians(180.0f), pillar.topScale, 1.0f, _pillarTexture, color);
		PepperMint::Renderer2D::DrawQuad(pillar.bottomPosition, 0.0f, pillar.bottomScale, 1.0f, _pillarTexture, color);
	}

	// Player
	_player.onRender();
}

void Level::onImGuiRender() {
	_player.onImGuiRender();
}

void Level::reset() {
	_gameOver = false;

	_player.reset();

	_pillarTarget = 30.0f;
	_pillarIndex = 0;
	resetPillars();
}

void Level::resetPillars() {
	_pillars.resize(5);
	for (int i = 0; i < 5; ++i) {
		createPillar(i, i * 10.0f);
	}
}

void Level::createPillar(int iIndex, float iOffset) {
	Pillar& pillar = _pillars[iIndex];
	pillar.topPosition.x = iOffset;
	pillar.bottomPosition.x = iOffset;
	pillar.topPosition.z = iIndex * 0.1f - 0.5f;
	pillar.bottomPosition.z = iIndex * 0.1f - 0.5f + 0.05f;

	float center = Random::Float() * 35.0f - 17.5f;
	float gap = 2.0f + Random::Float() * 5.0f;

	pillar.topPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
	pillar.bottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

void Level::endLevel() {
	_gameOver = true;
}

/*******************
 * Collision tests *
 *******************/

bool Level::checkCollision() {
	auto&& pos = _player.position();
	
	// Floor + Ceiling
	if (glm::abs(pos.y) > 8.5f) {
		return true;
	}

	glm::vec4 playerVertices[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	glm::vec4 playerTransformedVertices[4];
	for (int i = 0; i < 4; i++) {
		playerTransformedVertices[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f }) * 
									   glm::rotate(glm::mat4(1.0f), glm::radians(_player.rotation()), { 0.0f, 0.0f, 1.0f }) * 
									   glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f }) *
									   playerVertices[i];
	}

	// To match Triangle.png (each corner is 10% from the texture edge)
	glm::vec4 pillarVertices[3] = {
		{ -0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f },
	};

	for (auto&& p : _pillars) {
		glm::vec2 triangle[3];

		// Top pillars
		for (int i = 0; i < 3; i++) {
			triangle[i] = glm::translate(glm::mat4(1.0f), { p.topPosition.x, p.topPosition.y, 0.0f }) * 
						  glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f }) * 
						  glm::scale(glm::mat4(1.0f), { p.topScale.x, p.topScale.y, 1.0f }) * 
						  pillarVertices[i];
		}

		for (auto&& vertex : playerTransformedVertices) {
			if (pointInTriangle({ vertex.x, vertex.y }, triangle[0], triangle[1], triangle[2]))
				return true;
		}

		// Bottom pillars
		for (int i = 0; i < 3; i++) {
			triangle[i] = glm::translate(glm::mat4(1.0f), { p.bottomPosition.x, p.bottomPosition.y, 0.0f }) * 
						  glm::scale(glm::mat4(1.0f), { p.bottomScale.x, p.bottomScale.y, 1.0f }) * 
						  pillarVertices[i];
		}

		for (auto&& vertex : playerTransformedVertices) {
			if (pointInTriangle({ vertex.x, vertex.y }, triangle[0], triangle[1], triangle[2]))
				return true;
		}
	}

	return false;
}
