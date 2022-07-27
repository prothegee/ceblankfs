#pragma once
#include "GameConfig.h"


namespace ceblankfs
{
namespace players
{


/**
 * @brief core player projectile class
 * 
 */
class projectile
    :   public IEntityComponent
{
private:
    SEntityPhysicalizeParams physicsParams;




public:
    struct DVprojectile
    {
        
    };

    // projectile geometry
    // projectile speed
    // projectile material
    // projectile light
    // projectile type
    // projectile mass
    // projectile count

public:
    projectile(/* args */);
    virtual ~projectile();


    virtual void Initialize() override
    {
        // projectile geometry
        // projectile speed
        // projectile material
        // projectile light
        // projectile type
        // projectile mass
        // projectile count

        #ifndef NDEBUG
        CryLog("# players::projectile::Initialize");
        #else
        #endif
    }


    virtual Cry::Entity::EventFlags GetEventMask() const override
    {
        return
            Cry::Entity::EEvent::PhysicsCollision
            ;
    }
	virtual void ProcessEvent(const SEntityEvent& e) override
	{
        switch (e.event)
        {
            case Cry::Entity::EEvent::PhysicsCollision:
            {
                gEnv->pEntitySystem->RemoveEntity(GetEntityId());

                #ifndef NDEBUG
                CryLog("# players::projectile::ProcessEvent PhysicsCollision");
                #else
                #endif
            }
            break;
        }
	}


    static void ReflectType(Schematyc::CTypeDesc<projectile>& desc)
    {
        desc.SetGUID("{2f4bb0fb-a325-477f-bb3b-a652d7a5942d}"_cry_guid);
        desc.SetLabel("projectile");
        desc.SetEditorCategory("_players");
        desc.SetDescription("Player projectile component");
        #pragma region projectile data member
        #pragma endregion
    }




protected:
    // data

protected:
    // 
};


static void RegisterProjectileComponent(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(
            SCHEMATYC_MAKE_ENV_COMPONENT(ceblankfs::players::projectile));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterProjectileComponent);
ceblankfs::players::projectile::projectile(/* args */)
{
}
ceblankfs::players::projectile::projectile::~projectile()
{
}


} // namespace players
} // namespace ceblankfs
