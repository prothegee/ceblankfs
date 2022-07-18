// act as this project game config & may as pre compile header
#pragma once
#include <array>
#include <numeric>


#include "StdAfx.h"


#include <CryEntitySystem/IEntityComponent.h>


#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>


#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>


namespace ceblankfs
{

/**
 * @brief core game config class
 * 
 */
class GameConfig
{
public:
    const string projectName = "ceblankfs";
    const string projectVersion = "0.0.0";

public:
    GameConfig(/* args */);
    virtual ~GameConfig();
};


} // pace ceblankfs