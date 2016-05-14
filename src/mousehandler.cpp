#include <cassert>
#include "world.h"
#include "loader.h"
#include "matter.h"
#include "mousehandler.h"
#include "mousecallback.h"

MouseHandler::MouseHandler(World *_world)
    : mWorld(_world),
      leftClicked(false), rightClicked(false), leftHold(false), rightHold(false),
      leftRelease(false), rightRelease(false), lastClickedClock(clock()),
      status(MOUSE_FREE), mFreeCallback(NULL), mPuttingCallback(NULL)
{}

void MouseHandler::addButton(Rigid *_rigid, MouseCallback *_callback)
{
    assert(_rigid != NULL);
    assert(_callback != NULL);
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

    bool leftPressing = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    bool rightPressing = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

    leftRelease = (! leftPressing && leftHold);
    rightRelease = (! rightPressing && rightHold);

    clock_t now = clock();
    if (now - lastClickedClock > CLICK_INTERVAL * CLOCKS_PER_SEC)
    {
        leftClicked = leftPressing;
        rightClicked = rightPressing;
        if (leftClicked || rightClicked)
            lastClickedClock = now;
    } else
        leftClicked = rightClicked = false;

    leftHold = (! leftClicked && leftPressing);
    rightHold = (! rightClicked && rightPressing);
}

void MouseHandler::triggerCallback(MouseCallback *callback)
{
    if (callback)
    {
        if (leftClicked)
            callback->leftClick(worldX, worldY);
        if (rightClicked)
            callback->rightClick(worldX, worldY);
        if (leftRelease)
            callback->leftRelease(worldX, worldY);
        if (rightRelease)
            callback->rightRelease(worldX, worldY);
        if (!leftClicked && !rightClicked && !leftRelease && !rightRelease)
            callback->move(worldX, worldY);
    }
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
    triggerCallback(mFreeCallback);
}

void MouseHandler::processPutting()
{
    triggerCallback(mPuttingCallback);
}

void MouseHandler::setFreeCallback(MouseCallback *callback)
{
    if (mFreeCallback) delete mFreeCallback;
    mFreeCallback = callback;
}

void MouseHandler::setPuttingCallback(MouseCallback *callback)
{
    if (mPuttingCallback) delete mPuttingCallback;
    mPuttingCallback = callback;
}

void MouseHandler::setStatus(MouseStatus _status, MouseCallback *callback)
{
    if (_status == status) return;
    reset();

    status = _status;
    if (callback)
    {
        if (status == MOUSE_FREE)
            setFreeCallback(callback);
        if (status == MOUSE_PUTTING)
            setPuttingCallback(callback);
    }
}

void MouseHandler::reset()
{
    setPuttingCallback(NULL);
    for (const auto &button : buttons)
        button.first->setAlert(ALERT_NORMAL);
}

