#include "Bubble.h"

Bubble::Bubble(float x, float y, float radius, int scoreValue, Color color, float lifetime)
    : x(x), y(y), radius(radius), scoreValue(scoreValue),
      active(true), color(color), age(0.0f), lifetime(lifetime)
{
}

void Bubble::Update(float dt)
{
    age += dt;
}

void Bubble::Draw() const
{
    if (!active)
    {
        return;
    }

    float remaining = lifetime - age;
    Color c         = color;

    if (remaining < 1.5f)
    {
        c.a = (unsigned char)(remaining / 1.5f * 255.0f);
    }

    DrawCircleV({ x, y }, radius, c);
    DrawCircleLinesV({ x, y }, radius, BLACK);

    const char* text      = TextFormat("%d", scoreValue);
    int         fontSize  = (int)(radius * 0.7f);
    int         textWidth = MeasureText(text, fontSize);

    Color textColor = { 0, 0, 0, c.a };
    DrawText(
        text,
        (int)(x - textWidth / 2),
        (int)(y - fontSize / 2),
        fontSize,
        textColor
    );
}

bool Bubble::IsClicked(Vector2 mousePos) const
{
    if (!active)
    {
        return false;
    }

    float dx = mousePos.x - x;
    float dy = mousePos.y - y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

bool Bubble::IsActive() const
{
    return active;
}

bool Bubble::IsExpired() const
{
    return age >= lifetime;
}

void Bubble::SetActive(bool state)
{
    active = state;
}

int Bubble::GetScoreValue() const
{
    return scoreValue;
}