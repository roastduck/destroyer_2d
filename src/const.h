#ifndef CONST_H_
#define CONST_H_

// About window
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char WINDOW_TITLE[] = "Destroyer2D";

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

// Physics
const float GRAVITY = 2.5f; // >= 0
const float PARTICLE_RADIUS = 0.3f; // maybe too small

// About Simulation
const float TIME_STEP = 1.0f / 60.0f;
const int VELOCITY_ITERATIONS = 6;
const int POSITION_ITERATIONS = 3;

#endif // CONST_H_
