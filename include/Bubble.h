#pragma once
#include "raylib.h"

class Bubble
{
private:
    float x;
    float y;
    float radius;
    int   scoreValue;
    bool  active;
    Color color;
    float age;
    float lifetime;

public:
    Bubble(float x, float y, float radius, int scoreValue, Color color, float lifetime);

    void Update(float dt);
    void Draw()                      const;
    bool IsClicked(Vector2 mousePos) const;

    bool  IsActive()                 const;
    bool  IsExpired()                const;
    void  SetActive(bool state);
    int   GetScoreValue()            const;
};