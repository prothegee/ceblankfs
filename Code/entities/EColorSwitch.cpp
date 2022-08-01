#include "EColorSwitch.h"


static void Register_EColorSwitch_Component(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(EColorSwitch));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&Register_EColorSwitch_Component);
EColorSwitch::EColorSwitch(/* args */)
{
}
EColorSwitch::~EColorSwitch()
{
}
