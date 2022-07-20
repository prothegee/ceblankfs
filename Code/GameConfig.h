#pragma once
#include "pch_ceblankfs.h"


namespace ceblankfs
{

/**
 * @brief core game config class
 * 
 */
class GameConfig
{
public:
    const string projectName = "ceblankfs"; // project name

    static constexpr int MAJOR = 0; // project major version
    static constexpr int MINOR = 0; // project minor version
    static constexpr int PATCH = 0; // project patch version

public:
    GameConfig(/* args */);
    virtual ~GameConfig();
};


} // pace ceblankfs