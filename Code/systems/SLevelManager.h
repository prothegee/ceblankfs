#pragma once


class SLevelManager
{
public:
    SLevelManager(/* args */) {};
    virtual ~SLevelManager() {};

    // do something on level load start
    static void DSOLLS()
    {
        #ifndef NDEBUG
        CryLog("# SLevelManager DSOLLS");
        #else
        #endif
    }

    // do something on level load end
    static void DSOLLE()
    {
        #ifndef NDEBUG
        CryLog("# SLevelManager DSOLLE");
        #else
        #endif
    }
};
