// act as this project game config & may as pre compile header
#pragma once


#include <CryEntitySystem/IEntityComponent.h>


#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>


namespace blankfs
{

/**
 * @brief core game config class
 * 
 */
class GameConfig
{
private:
    /* data */
public:
    GameConfig(/* args */);
    virtual ~GameConfig();
};


} // pace blankfs