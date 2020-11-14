#pragma once

#include "Player.h"

struct Pillar {
    glm::vec3 topPosition = {0.0f, 10.0f, 0.0f};
    glm::vec2 topScale    = {15.0f, 20.0f};

    glm::vec3 bottomPosition = {10.0f, 10.0f, 0.0f};
    glm::vec2 bottomScale    = {15.0f, 20.0f};
};

class Level {
  public:
    Level()  = default;
    ~Level() = default;

    void init();

    void onUpdate(PepperMint::Timestep iTimestep);
    void onRender();
    void onImGuiRender();

    void reset();

    inline Player& player() { return _player; }
    inline bool    gameOver() const { return _gameOver; }

  private:
    void resetPillars();
    void createPillar(int iIndex, float iOffset);
    void endLevel();

    bool checkCollision();

  private:
    Player _player;

    bool _gameOver = false;

    PepperMint::Ref<PepperMint::Texture2D> _pillarTexture;
    std::vector<Pillar>                    _pillars;
    int                                    _pillarIndex  = 0;
    float                                  _pillarTarget = 30.0f;
    glm::vec3                              _pillarHSV    = {0.0f, 0.8f, 0.8f};
};
