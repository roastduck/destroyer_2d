#include <cassert>
#include "world.h"
#include "loader.h"
#include "matter.h"
#include "mousehandler.h"

MouseHandler::MouseHandler(World *_world)
    : mWorld(_world),
      leftClicked(false), rightClicked(false), lastClickedClock(clock()),
      status(MOUSE_FREE), mPuttingCallback(NULL)
{}

void MouseHandler::addButton(Rigid *_rigid, MouseCallback *_callback)
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
    case MOUSE_PUTTING:
        processPutting();
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

    clock_t now = clock();
    if (now - lastClickedClock > CLICK_INTERVAL * CLOCKS_PER_SEC)
    {
        leftClicked = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        rightClicked = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
        if (leftClicked || rightClicked)
            lastClickedClock = now;
    } else
        leftClicked = rightClicked = false;
}

void MouseHandler::processFree()
{
    for (const auto &button : buttons)
        if (button.first->testPoint(worldX, worldY))
        {
            button.first->setAlert(ALERT_HOVER);
            if (leftClicked)
                button.second->leftClick(worldX, worldY);
        } else
            button.first->setAlert(ALERT_NORMAL);
}

void MouseHandler::processPutting()
{
    if (mPuttingCallback)
    {
        if (leftClicked)
            mPuttingCallback->leftClick(worldX, worldY);
        else if (rightClicked)
            mPuttingCallback->rightClick(worldX, worldY);
        else
            mPuttingCallback->move(worldX, worldY);
    }
}

void MouseHandler::reset()
{
    if (mPuttingCallback)
    {
        mPuttingCallback = 0;
        delete mPuttingCallback;
    }
    for (const auto &button : buttons)
        button.first->setAlert(ALERT_NORMAL);
}

void MouseHandler::setStatus(MouseStatus _status, MouseCallback *puttingCallback)
{
    if (_status == status) return;
    reset();

    status = _status;
    mPuttingCallback = puttingCallback;
}

