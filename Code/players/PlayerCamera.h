#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{

class PlayerCamera
    :   public IEntityComponent
{
public:
    PlayerCamera(/* args */);
    virtual ~PlayerCamera();


    static void ReflectType(Schematyc::CTypeDesc<PlayerCamera>& desc)
    {
        desc.SetGUID("{90085a54-7f05-46aa-b203-d614a938c0cc}"_cry_guid);
        desc.SetLabel("PlayerCamera");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Core player camera entity");
    }
};
 

} // namespace players
} // namespace ceblankfs