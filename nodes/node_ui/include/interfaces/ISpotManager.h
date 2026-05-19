#pragma once

class ISpotManager {
public:
    virtual ~ISpotManager() {}
    virtual void reserveSpot(int spotID) = 0;
    virtual void releaseSpot(int spotID) = 0;
};
