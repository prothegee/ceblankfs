// Copyright 2016-2019 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntityClass.h>
#include <CryNetwork/INetwork.h>

class PCore;

// The entry-point of the application
// An instance of CGamePlugin is automatically created when the library is loaded
// We then construct the local player entity and PCore instance when OnClientConnectionReceived is first called.
class CGamePlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
	, public INetworkedClientListener
{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "CGamePlugin", "f01244b0-a4e7-4dc6-91e1-0ed18906fe7c"_cry_guid)

	virtual ~CGamePlugin();
	
	// Cry::IEnginePlugin
	virtual const char* GetCategory() const override { return "CGamePlugin"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	// INetworkedClientListener
	// Sent to the local client on disconnect
	virtual void OnLocalClientDisconnected(EDisconnectionCause cause, const char* description) override {}

	// Sent to the server when a new client has started connecting
	// Return false to disallow the connection
	virtual bool OnClientConnectionReceived(int channelId, bool bIsReset) override;
	// Sent to the server when a new client has finished connecting and is ready for gameplay
	// Return false to disallow the connection and kick the player
	virtual bool OnClientReadyForGameplay(int channelId, bool bIsReset) override;
	// Sent to the server when a client is disconnected
	virtual void OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient) override;
	// Sent to the server when a client is timing out (no packets for X seconds)
	// Return true to allow disconnection, otherwise false to keep client.
	virtual bool OnClientTimingOut(int channelId, EDisconnectionCause cause, const char* description) override { return true; }
	// ~INetworkedClientListener

	// Helper function to call the specified callback for every player in the game
	void IterateOverPlayers(std::function<void(PCore& player)> func) const;

	// Helper function to get the CGamePlugin instance
	// Note that CGamePlugin is declared as a singleton, so the CreateClassInstance will always return the same pointer
	static CGamePlugin* GetInstance()
	{
		return cryinterface_cast<CGamePlugin>(CGamePlugin::s_factory.CreateClassInstance().get());
	}
	
protected:
	// Map containing player components, key is the channel id received in OnClientConnectionReceived
	std::unordered_map<int, EntityId> m_players;
};