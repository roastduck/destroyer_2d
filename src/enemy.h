#ifndef ENEMY_H_
#define ENEMY_H_

#include <ctime>
#include <vector>
#include <string>

class World;
class Rigid;

/**
 * This class loads and controls enemy
 */
class Enemy
{
public:
    Enemy(World *world, std::vector<std::string> _path, float _offsetX);

    /**
     * set objects to the world
     */
    void start();

    /**
     * if key down now?
     */
    bool getKeyDown(int key);

    int getId() const { return id; }

private:
    void loadActions();

    World *mWorld;

    std::vector<std::string> path;

    int id;
    static int maxId;

    float offsetX;

    clock_t startClock;

    struct action_t
    {
        char key;
        float sec;
        enum { PRESS = 0, RELEASE = 1};
        int flag; // PRESS or RELEASE. defined as int for inputting
    };
    std::vector<action_t> actions;

    bool keyDown[26];
};

#endif // ENEMY_H_

