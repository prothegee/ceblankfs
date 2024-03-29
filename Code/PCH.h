#pragma once
#include <iostream>
#include <string>

#include <array>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <sstream>


#include "StdAfx.h"
#include "GamePlugin.h"


#include <CryEntitySystem/IEntityComponent.h>


#include <CryCore/StaticInstanceList.h>
#include <CryRenderer/IRenderAuxGeom.h>
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

#include <json/json.h>