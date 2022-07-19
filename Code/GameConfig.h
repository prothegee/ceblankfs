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
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/AlembicComponent.h>
#include <DefaultComponents/Geometry/AnimatedMeshComponent.h>
#include <DefaultComponents/Geometry/BaseMeshComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>


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