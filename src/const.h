#ifndef CONST_H_
#define CONST_H_

// About window
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char WINDOW_TITLE[] = "Destroyer2D";

// About UI
enum AlertType
{
    ALERT_NONE = 0,
    ALERT_NORMAL = 1,
    ALERT_HOVER = 2,
    ALERT_WARNING = 3
};

// Colors
const float BG_COLOR_R = 0.0f;
const float BG_COLOR_G = 0.0f;
const float BG_COLOR_B = 0.0f;

const float WOOD_COLOR_R = (float)0x85/0xFF;
const float WOOD_COLOR_G = (float)0x5E/0xFF;
const float WOOD_COLOR_B = (float)0x42/0xFF;
const float WOOD_COLOR_A = 1.0f;

const float FRAME_COLOR_R = 1.0f;
const float FRAME_COLOR_G = 1.0f;
const float FRAME_COLOR_B = 1.0f;
const float FRAME_COLOR_A = 1.0f;

const unsigned char WATER_COLOR_R_256 = 0xB2;
const unsigned char WATER_COLOR_G_256 = 0xB2;
const unsigned char WATER_COLOR_B_256 = 0xFF;
const unsigned char WATER_COLOR_A_256 = 0x60;
// The alpha for particles actually not influencing the
// color, but the particle size

const float ALERT_NORMAL_COLOR_R = (float)0x05/0xFF;
const float ALERT_NORMAL_COLOR_G = (float)0x3E/0xFF;
const float ALERT_NORMAL_COLOR_B = (float)0xFF/0xFF;
const float ALERT_NORMAL_COLOR_A = 0.5f;

const float ALERT_HOVER_COLOR_R = (float)0xFF/0xFF;
const float ALERT_HOVER_COLOR_G = (float)0x77/0xFF;
const float ALERT_HOVER_COLOR_B = (float)0x00/0xFF;
const float ALERT_HOVER_COLOR_A = 0.5f;

const float ALERT_WARNING_COLOR_R = (float)0xFF/0xFF;
const float ALERT_WARNING_COLOR_G = (float)0x00/0xFF;
const float ALERT_WARNING_COLOR_B = (float)0x00/0xFF;
const float ALERT_WARNING_COLOR_A = 0.5f;

// Render
const float PARTICLE_SIZE_SCALE = 200.0f;

// Physics
const float GRAVITY = 2.5f; // >= 0
const float PARTICLE_RADIUS = 0.25f; // maybe too small

// About Simulation
const float TIME_STEP = 1.0f / 60.0f;
const int VELOCITY_ITERATIONS = 6;
const int POSITION_ITERATIONS = 3;

#endif // CONST_H_
