//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_DROIDBLASTER_HPP
#define DROIDBLASTER_DROIDBLASTER_HPP

#include "ActivityHandler.hpp"
#include "EventLoop.hpp"
#include "GraphicsManager.hpp"
#include "PhysicsManager.hpp"
#include "TimeManager.hpp"
#include "Resource.hpp"
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "SpriteBatch.hpp"
#include "StarField.hpp"
#include "Types.hpp"


class DroidBlaster : public ActivityHandler {
public:
    DroidBlaster(android_app *pApplication);
    void run();

protected:
    status onActivate();
    void onDeactivate();
    status onStep();

    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();

    void onSaveInstanceState(void** pData, size_t *pSize);
    void onConfigurationChanged();
    void onLowMemory();

    void onCreateWindow();
    void onDestroyWindow();
    void onGainFocus();
    void onLostFocus();

private:
    DroidBlaster(const DroidBlaster&);
    void operator=(const DroidBlaster&);

    GraphicsManager mGraphicsManager;
    TimeManager mTimeManager;
    PhysicsManager mPhysicsManager;
    EventLoop mEventLoop;

    Resource mAsteroidTexture;
    Resource mShipTexture;
    Resource mStarTexture;

    Ship mShip;
    Asteroid mAsteroids;

    SpriteBatch mSpriteBatch;
    StarField mStarField;
};


#endif //DROIDBLASTER_DROIDBLASTER_HPP
