#include "raylib.h"
#include "Bubble.h"
#include <vector>
#include <algorithm>

const int   SCREEN_W        = 800;
const int   SCREEN_H        = 450;
const float SPAWN_INTERVAL  = 1.2f;
const float RADIUS_MIN      = 20.0f;
const float RADIUS_MAX      = 50.0f;
const float GAME_DURATION   = 60.0f;
const float BUBBLE_LIFETIME = 5.0f;
const float UI_HEIGHT = 60.0f;

Bubble SpawnBubble()
{
    float radius = RADIUS_MIN
                 + (float)GetRandomValue(0, 100) / 100.0f
                 * (RADIUS_MAX - RADIUS_MIN);

    float x = radius + (float)GetRandomValue(0, SCREEN_W - (int)(radius * 2));
    float y = UI_HEIGHT + radius
            + (float)GetRandomValue(0, SCREEN_H - (int)(UI_HEIGHT + radius * 2));

    int score = (int)(RADIUS_MAX - radius) / 5 + 1;

    Color colors[] =
    {
        RED, ORANGE, YELLOW, GREEN, BLUE,
        PURPLE, PINK, SKYBLUE, LIME, GOLD
    };
    Color color = colors[GetRandomValue(0, 9)];

    return Bubble(x, y, radius, score, color, BUBBLE_LIFETIME);
}

void ResetGame(std::vector<Bubble>& bubbles, int& score,
               float& spawnTimer, float& gameTime, bool& gameOver)
{
    bubbles.clear();
    score      = 0;
    spawnTimer = 0.0f;
    gameTime   = GAME_DURATION;
    gameOver   = false;

    for (int i = 0; i < 5; i++)
    {
        bubbles.push_back(SpawnBubble());
    }
}

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "EX06 - Click the Bubble");
    SetTargetFPS(60);

    std::vector<Bubble> bubbles;
    int   totalScore = 0;
    float spawnTimer = 0.0f;
    float gameTime   = GAME_DURATION;
    bool  gameOver   = false;

    for (int i = 0; i < 5; i++)
    {
        bubbles.push_back(SpawnBubble());
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (gameOver)
        {
            if (IsKeyPressed(KEY_R))
            {
                ResetGame(bubbles, totalScore, spawnTimer, gameTime, gameOver);
            }
        }
        else
        {
            gameTime -= dt;

            if (gameTime <= 0.0f)
            {
                gameTime = 0.0f;
                gameOver = true;
            }

            spawnTimer += dt;
            if (spawnTimer >= SPAWN_INTERVAL)
            {
                bubbles.push_back(SpawnBubble());
                spawnTimer = 0.0f;
            }

            for (Bubble& b : bubbles)
            {
                b.Update(dt);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mouse = GetMousePosition();

                for (Bubble& b : bubbles)
                {
                    if (b.IsClicked(mouse))
                    {
                        totalScore += b.GetScoreValue();
                        b.SetActive(false);
                        break;
                    }
                }
            }

            bubbles.erase(
                std::remove_if(
                    bubbles.begin(),
                    bubbles.end(),
                    [](const Bubble& b) { return !b.IsActive() || b.IsExpired(); }
                ),
                bubbles.end()
            );
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (const Bubble& b : bubbles)
            {
                b.Draw();
            }

            DrawText(TextFormat("Score: %d", totalScore), 10, 10, 30, DARKGRAY);
            DrawText(TextFormat("Time: %.0f", gameTime), SCREEN_W - 130, 10, 30, DARKGRAY);

            if (gameOver)
            {
                DrawRectangle(0, 0, SCREEN_W, SCREEN_H, { 0, 0, 0, 150 });

                const char* finalText = TextFormat("Final Score: %d", totalScore);
                int         finalW    = MeasureText(finalText, 50);
                DrawText(finalText, SCREEN_W / 2 - finalW / 2, SCREEN_H / 2 - 50, 50, WHITE);

                const char* restartText = "Press R to Restart";
                int         restartW    = MeasureText(restartText, 25);
                DrawText(restartText, SCREEN_W / 2 - restartW / 2, SCREEN_H / 2 + 20, 25, LIGHTGRAY);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}