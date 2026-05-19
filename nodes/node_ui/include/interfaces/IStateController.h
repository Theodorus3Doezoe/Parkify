#pragma once

class IStateController {
public:
    virtual ~IStateController() {}
    virtual void setState(int state) = 0;
    virtual int getState() = 0;
};
