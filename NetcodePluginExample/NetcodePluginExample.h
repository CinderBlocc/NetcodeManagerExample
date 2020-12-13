#pragma once
#pragma comment(lib, "pluginsdk.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "NetcodeManager.h"

class NetcodePluginExample : public BakkesMod::Plugin::BakkesModPlugin
{
private:
    std::shared_ptr<NetcodeManager> Netcode;

public:
	void onLoad() override;
	void onUnload() override;

    ServerWrapper GetCurrentGameState();

    void RequestBall();
    void OnMessageReceived(const std::string& Message, PriWrapper Sender);
};
