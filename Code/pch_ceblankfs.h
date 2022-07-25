/**
 * @file pch_ceblankfs.h
 * @brief precompiler header for ceblankfs project
 * 
 * @note lazy mode to include header/s if you may
 * 
 */
#pragma once
#include <iostream>
#include <string>

#include <array>
#include <numeric>


#include "StdAfx.h"


#include <CryEntitySystem/IEntityComponent.h>


#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>


#include <DefaultComponents/Audio/ListenerComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/AlembicComponent.h>
#include <DefaultComponents/Geometry/AnimatedMeshComponent.h>
#include <DefaultComponents/Geometry/BaseMeshComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
