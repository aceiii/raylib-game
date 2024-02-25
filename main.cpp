
/*

Powerful Trap Beat | Strong by Alex-Productions | https://onsound.eu/
Music promoted by https://www.chosic.com/free-music/all/
Creative Commons CC BY 3.0
https://creativecommons.org/licenses/by/3.0/

*/

#include <raylib.h>
#include <fmt/core.h>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

int window_width = 1200;
int window_height = 600;

int paddle_width = 300;
int paddle_height = 20;
float paddle_x = 0;
float paddle_y = 0;
float paddle_speed = 500;

int ball_radius = 25;
float ball_x = 0;
float ball_y = 0;
float ball_speed_x = 0;
float ball_speed_y = 0;

enum powerup_ability {
    POWER_BALL_SMALLER,
    POWER_BALL_BIGGER,
    POWER_BALL_SLOWER,
    POWER_BALL_FASTER,
    POWER_PADDLE_LONGER,
    POWER_PADDLE_SHORTER,
    POWER_COUNT,
};

struct powerup {
    int size;
    int x;
    int y;
    powerup_ability ability;
};

std::vector<powerup> powerups;
int powerup_count = 20;

int points = 0;

Sound boing;
Music music;

bool start_game = false;

int window_center_x() {
    return (window_width / 2);
}

int window_center_y() {
    return (window_height / 2);
}

void init() {
    InitWindow(window_width, window_height, "Bryan's First Game");
    InitAudioDevice();

    boing = LoadSound("assets/boing2.wav");
    music = LoadMusicStream("assets/Powerful-Trap.mp3");

    int gap = 20;
    paddle_x = window_center_x() - (paddle_width / 2);
    paddle_y = window_height - paddle_height - gap;

    ball_x = window_center_x();
    ball_y = window_center_y();

    PlayMusicStream(music);
}

void remove_powerup(std::vector<powerup>::iterator it) {
    powerups.erase(it);
}

void add_random_powerup() {
    powerup p;
    p.size = 20;
    p.x = GetRandomValue(50, window_width - 50);
    p.y = GetRandomValue(50, window_height - paddle_height - 100);
    p.ability = static_cast<powerup_ability>(GetRandomValue(0, POWER_COUNT));
    powerups.push_back(p);
}

bool check_powerup_collisions(const powerup& p) {
    Rectangle rect;
    rect.width = p.size;
    rect.height = p.size;
    rect.x = p.x - (rect.width / 2);
    rect.y = p.y - (rect.height /2);

    Vector2 circle_position;
    circle_position.x = ball_x;
    circle_position.y = ball_y;

    if (CheckCollisionCircleRec(circle_position, ball_radius, rect)) {
        // do all the powerups
        switch (p.ability) {
        case POWER_BALL_BIGGER:
            fmt::print("powerup: ball bigger\n");
            ball_radius += 20;
            if (ball_radius > 100) {
                ball_radius = 100;
            }

            break;

        case POWER_BALL_SMALLER:
            fmt::print("powerup: ball smaller\n");
            ball_radius -= 20;
            if (ball_radius < 20) {
                ball_radius = 20;
            }

            break;

        case POWER_BALL_SLOWER:
        {
            fmt::print("powerup: ball slower\n");
            ball_speed_x = ball_speed_x / 2;
            ball_speed_y = ball_speed_y / 2;

            int min_ball_sleep = 50;
            if (ball_speed_x < min_ball_sleep) {
                ball_speed_x = min_ball_sleep;
            }
            if (ball_speed_y < min_ball_sleep) {
                ball_speed_y = min_ball_sleep;
            }
        }
        break;

        case POWER_BALL_FASTER:
        {
            fmt::print("powerup: ball faster\n");
            ball_speed_x = ball_speed_x * 2;
            ball_speed_y = ball_speed_y * 2;

            int max_ball_speed = 2000;
            if (ball_speed_x > max_ball_speed) {
                ball_speed_x = max_ball_speed;
            }
            if (ball_speed_y > max_ball_speed) {
                ball_speed_y = max_ball_speed;
            }
        }
            break;

        case POWER_PADDLE_LONGER:
            fmt::print("powerup: ball longer\n");
            paddle_width = paddle_width + 20;
            if (paddle_width > 500) {
                paddle_width = 500;
            }
            break;

        case POWER_PADDLE_SHORTER:
            fmt::print("powerup: ball shorter\n");
            paddle_width = paddle_width - 20;
            if (paddle_width < 50) {
                paddle_width = 50;
            }
            break;

        default:
            break;
        }

        return true;
    }

    return false;
}

void clear_powerups() {
    powerups.clear();
}

void update() {
    float time = GetFrameTime();

    if (start_game == false && IsKeyPressed(KEY_ENTER)) {
        start_game = true;
        ball_speed_x = 250;
        ball_speed_y = 250;

        for (int i = 0; i < powerup_count; i += 1) {
            add_random_powerup();
        }
    }

    if (start_game == false) {
        return;
    }

    if(IsKeyPressed(KEY_UP)) {
        paddle_speed += 100;
    }

    if(IsKeyPressed(KEY_DOWN)) {
        paddle_speed -= 100;
    }

    // if (IsKeyDown(KEY_LEFT)) {
    //     // move padding to the left
    //     paddle_x = paddle_x - (time * paddle_speed);
    //     if (paddle_x < 0) {
    //         paddle_x = 0;
    //     }

    // } else if (IsKeyDown(KEY_RIGHT)) {
    //     // move padding to the right
    //     paddle_x = paddle_x + (time * paddle_speed);

    //     int max_x = window_width - paddle_width;
    //     if (paddle_x > max_x) {
    //         paddle_x = max_x;
    //     }
    // }

    auto mouse = GetMousePosition();
    auto move_diff = mouse.x - (paddle_x + (paddle_width / 2));
    if (move_diff < 0 ) {
        move_diff = -1;
    } else if (move_diff > 0) {
        move_diff = 1;
    } else {
        move_diff = 0;
    }

    if (paddle_x < 0) {
        paddle_x = 0;
    }

    int max_x = window_width - paddle_width;
    if (paddle_x > max_x) {
        paddle_x = max_x;
    }

    // paddle_x = mouse.x;
    // paddle_y = mouse.y;
    paddle_x += move_diff * paddle_speed * time;

    ball_x = ball_x + (time * ball_speed_x);
    ball_y = ball_y + (time * ball_speed_y);

    int min_ball_x = ball_radius;
    int min_ball_y = ball_radius;
    int max_ball_x = window_width - ball_radius;
    int max_ball_y = window_height - ball_radius;

    bool bounce = false;

    if (ball_x < min_ball_x) {
        ball_x = min_ball_x;
        ball_speed_x = -ball_speed_x;
        bounce = true;
    }

    if (ball_x > max_ball_x) {
        ball_x = max_ball_x;
        ball_speed_x = -ball_speed_x;
        bounce = true;
    }

    if (ball_y < min_ball_y) {
        ball_y = min_ball_y;
        ball_speed_y = -ball_speed_y;
        bounce = true;
    }

    // if (ball_y > max_ball_y) {
    //     ball_y = max_ball_y;
    //     ball_speed_y = -ball_speed_y;
    //     bounce = true;
    // }

    if (ball_y > window_height + ball_radius) {
        // LOSE, RESTART GAME
        start_game = false;
        ball_x = window_center_x();
        ball_y = window_center_y();
        ball_speed_x = 0;
        ball_speed_y = 0;

        clear_powerups();

        return;
    }

    Vector2 circle_position;
    circle_position.x = ball_x;
    circle_position.y = ball_y;

    Rectangle paddle;
    paddle.width = paddle_width;
    paddle.height = paddle_height;
    paddle.x = paddle_x;
    paddle.y = paddle_y;

    if (CheckCollisionCircleRec(circle_position, ball_radius, paddle)) {
        // ball_speed_x = -ball_speed_x;
        ball_speed_y = -ball_speed_y;
        bounce = true;
        points += 100;
    }

    if (bounce) {
        PlaySound(boing);
    }

    bool removed_powerup = false;
    for (auto it = powerups.begin(); it != powerups.end(); it++) {
        const auto& powerup = *it;
        if (check_powerup_collisions(powerup)) {
            points += 500;
            remove_powerup(it);
            removed_powerup = true;
            break;
        }
    }

    if (removed_powerup) {
        add_random_powerup();
    }
}

void draw_start_text() {
    int x = 0;
    int y = 200;

    Rectangle rect;
    rect.width = 360;
    rect.height = 200;
    rect.x = window_center_x() - (rect.width / 2) - x;
    rect.y = window_center_y() - (rect.height / 2) - y;

    DrawRectangleRounded(rect, 1, 3, MAROON);

    auto text = "START!";
    int font_size = 80;
    int width = MeasureText(text, font_size);
    DrawText(text, window_center_x() - (width / 2) - x, window_center_y() - 30 - y, font_size, WHITE);
}

void draw_ball() {
    DrawCircle(ball_x, ball_y, ball_radius, WHITE);
}

void draw_paddle() {
    DrawRectangle(paddle_x, paddle_y, paddle_width, paddle_height, RED);
    DrawText(fmt::format("({:.0f},{:.0f})", paddle_x, paddle_y).c_str(), paddle_x, paddle_y, 20, WHITE);
}

void draw_powerups() {
    for (auto &powerup: powerups) {
        Rectangle rect;
        rect.width = powerup.size;
        rect.height = powerup.size;
        rect.x = powerup.x - (rect.width / 2);
        rect.y = powerup.y - (rect.height / 2);

        Color a = RED;
        Color b = GREEN;
        Color c = ORANGE;
        Color d = YELLOW;

        if (powerup.ability == POWER_BALL_SLOWER ||
            powerup.ability == POWER_BALL_SMALLER ||
            powerup.ability == POWER_PADDLE_SHORTER)
        {
            b = MAROON;
            c = MAGENTA;
            d = VIOLET;
        }

        DrawRectangleGradientEx(rect, a, b, c, d);
    }
}

void draw_points() {
    std::string text = fmt::format("{} Points", points);
    int font_size = 40;
    int width = MeasureText(text.c_str(), font_size);
    DrawText(text.c_str(), window_width - width - 10, 10, font_size, WHITE);
}

void draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    DrawFPS(10, 10);

    draw_ball();
    draw_paddle();
    draw_powerups();
    draw_points();

    if (start_game == false) {
        draw_start_text();
    }

    EndDrawing();
}

int main() {
    fmt::print("Hello, world\n");

    init();

    SetExitKey(KEY_ESCAPE);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        update();
        draw();
    }

    fmt::print("Closing...\n");

    UnloadSound(boing);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

