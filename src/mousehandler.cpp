#include <cassert>
#include "world.h"
#include "loader.h"
#include "matter.h"
#include "mousehandler.h"

void MouseHandler::addButton(Rigid *_rigid, OnClickCallback *_callback)
{
    buttons.push_back(std::make_pair(_rigid, _callback));
}

void MouseHandler::process()
{
    updateMouse();
    switch (status)
    {
    case MOUSE_FREE:
        processFree();
        break;
    default:
        assert(false);
    }
}

void MouseHandler::updateMouse()
{
    GLFWwindow *window = mWorld->mWindow->getReferee();
    double _x, _y;
    glfwGetCursorPos(window, &_x, &_y);
    int _w, _h;
    mWorld->mWindow->getWidthHeight(&_w, &_h);
    _y = _h - _y;

    worldX = _x / _w * (mWorld->mCurRightMost - mWorld->mCurLeftMost) + mWorld->mCurLeftMost;
    worldY = _y / _h * (mWorld->mCurUpMost - mWorld->mCurDownMost) + mWorld->mCurDownMost;
}

void MouseHandler::processFree()
{
    for (const auto &button : buttons)
        if (button.first->testPoint(worldX, worldY))
            button.first->setAlert(ALERT_HOVER);
        else
            button.first->setAlert(ALERT_NORMAL);
}

