#include <cctype>
#include <cstring>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "enemy.h"
#include "saver.h"
#include "matter.h"

int Enemy::maxId = 0;

Enemy::Enemy(World *world, std::vector<std::string> _path, float _offsetX)
    : mWorld(world), path(_path), id(++maxId), offsetX(_offsetX)
{
    memset(keyDown, 0, sizeof keyDown);
    loadActions();
}

void Enemy::start()
{
    auto _path = path;
    _path.back() += ".data";
    Saver::getInstance().loadFrom(mWorld, _path, offsetX, id);
    startClock = clock();
}

bool Enemy::getKeyDown(int key)
{
    clock_t now = clock();
    while (! actions.empty() && startClock + actions.back().sec * CLOCKS_PER_SEC < now)
    {
        assert(isalpha(actions.back().key));
        keyDown[actions.back().key - 'A'] = actions.back().flag == action_t::PRESS ? true : false;
        actions.pop_back();
    }
    assert(isalpha(key));
    return keyDown[key - 'A'];
}

void Enemy::loadActions()
{
    auto _path = path;
    _path.back() += ".conf";
    std::ifstream conf(Saver::getInstance().makePath(_path));
    if (! conf.good())
    {
        std::cout << "Warning : config file " 
                  << Saver::getInstance().makePath(_path) << " not found" << std::endl;
        return;
    }

    action_t action;
    while (conf >> action.key >> action.sec >> action.flag)
    {
        assert(isupper(action.key));
        assert(action.flag == action_t::PRESS || action.flag == action_t::RELEASE);
        actions.push_back(action);
    }
    std::sort(actions.begin(), actions.end(), [](const action_t &x, const action_t &y){return x.sec > y.sec;});
}

