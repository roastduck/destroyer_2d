#ifndef CONST_H_
#define CONST_H_

/*
 * About window
 */
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char WINDOW_TITLE[] = "Destroyer2D";

/*
 * About UI
 */
enum AlertType
{
    ALERT_NONE = 0,
    ALERT_NORMAL = 1,
    ALERT_HOVER = 2,
    ALERT_WARNING = 3,
    ALERT_SHADOW = 4
};

const float ALERT_LINE_WIDTH = 0.2f;

const int POPUP_PADDING = 20; // in pixels
const int POPUP_BOARDER = 5;

// minimum interval of two mouse clicks, in sec
const float CLICK_INTERVAL = 0.3f;

const float FOCUS_SPEED = 2.0f;

/*
 * Shapes
 */
const float STICK_THICKNESS = 0.3f;
const float STICK_END_THICKNESS = 0.5f;

const float WATER_PARTICLE_SIZE_SCALE = 6.0f;
const float DUST_PARTICLE_SIZE_SCALE = 1.0f;
const float FLAME_PARTICLE_SIZE_SCALE = DUST_PARTICLE_SIZE_SCALE;

/*
 * Colors
 */
const float BG_COLOR_R = 0.0f;
const float BG_COLOR_G = 0.0f;
const float BG_COLOR_B = 0.0f;

const float WOOD_COLOR_R = (float)0x85/0xFF;
const float WOOD_COLOR_G = (float)0x5E/0xFF;
const float WOOD_COLOR_B = (float)0x42/0xFF;
const float WOOD_COLOR_A = 1.0f;

const float STEEL_COLOR_R = (float)0xC0/0xFF;
const float STEEL_COLOR_G = (float)0xC0/0xFF;
const float STEEL_COLOR_B = (float)0xC0/0xFF;
const float STEEL_COLOR_A = 1.0f;

const float BOMB_COLOR_R = (float)0x7A/0xFF;
const float BOMB_COLOR_G = (float)0x00/0xFF;
const float BOMB_COLOR_B = (float)0x00/0xFF;
const float BOMB_COLOR_A = 1.0f;

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

const unsigned char FLAME_COLOR_R_256 = 0xFF;
const unsigned char FLAME_COLOR_G_256 = 0xFF;
const unsigned char FLAME_COLOR_B_256 = 0x00;
const unsigned char FLAME_COLOR_A_256 = 0xFF;

const float ALERT_NORMAL_COLOR_R = (float)0x05/0xFF;
const float ALERT_NORMAL_COLOR_G = (float)0x3E/0xFF;
const float ALERT_NORMAL_COLOR_B = (float)0xFF/0xFF;
const float ALERT_NORMAL_COLOR_A = 0.5f;

const float ALERT_HOVER_COLOR_R = (float)0xFF/0xFF;
const float ALERT_HOVER_COLOR_G = (float)0xFF/0xFF;
const float ALERT_HOVER_COLOR_B = (float)0x00/0xFF;
const float ALERT_HOVER_COLOR_A = 0.5f;

const float ALERT_WARNING_COLOR_R = (float)0xFF/0xFF;
const float ALERT_WARNING_COLOR_G = (float)0x00/0xFF;
const float ALERT_WARNING_COLOR_B = (float)0x00/0xFF;
const float ALERT_WARNING_COLOR_A = 0.8f;

const float ALERT_SHADOW_COLOR_R = (float)0x32/0xFF;
const float ALERT_SHADOW_COLOR_G = (float)0x27/0xFF;
const float ALERT_SHADOW_COLOR_B = (float)0x22/0xFF;
const float ALERT_SHADOW_COLOR_A = 0.8f;

const float TIP_LINE_COLOR_R = (float)0xFF/0xFF;
const float TIP_LINE_COLOR_G = (float)0x00/0xFF;
const float TIP_LINE_COLOR_B = (float)0x00/0xFF;
const float TIP_LINE_COLOR_A = 1.0f;

const float POPUP_BOARDER_R = (float)0x3D/0xFF;
const float POPUP_BOARDER_G = (float)0x2B/0xFF;
const float POPUP_BOARDER_B = (float)0x1F/0xFF;
const float POPUP_BOARDER_A = 1.0f;

/*
 * Physics
 */
const float GRAVITY = 3.0f; // >= 0
const float PARTICLE_RADIUS = 0.35f; // maybe too small

const float WOOD_DENSITY = 0.4f;
const float WOOD_FRICTION = 0.7f;
const float WOOD_RESTITUTION = 0.5f;
const float WOOD_STRENGTH = 60.0f;
// strength means how much impulse can it stand with. It's not physics but imaginary...

const float STEEL_DENSITY = 7.9f;
const float STEEL_FRICTION = 0.2f;
const float STEEL_RESTITUTION = 0.56f;
const float STEEL_STRENGTH = 250.0f;

const float BOMB_DENSITY = 4.0f;
const float BOMB_FRICTION = STEEL_FRICTION;
const float BOMB_RESTITUTION = STEEL_RESTITUTION;
const float BOMB_STRENGTH = 150.0f;

const float SMALL_ENGINE_FORCE = 50.0f;
const float LARGE_ENGINE_FORCE = 500.0f;

/*
 * About Simulation
 */
const float TIME_STEP = 1.0f / 45.0f;
const int VELOCITY_ITERATIONS = 6;
const int POSITION_ITERATIONS = 3;

#endif // CONST_H_
