#include "NetcodePluginExample.h"
#include "bakkesmod\wrappers\includes.h"

BAKKESMOD_PLUGIN(NetcodePluginExample, "Example of NetcodePlugin replication", "1.0", PLUGINTYPE_FREEPLAY)

void NetcodePluginExample::onLoad()
{
	using namespace std::placeholders;
    Netcode = std::make_shared<NetcodeManager>(cvarManager, gameWrapper, exports, std::bind(&NetcodePluginExample::OnMessageReceived, this, _1, _2));

	cvarManager->registerNotifier("NetcodeExample_RequestBall", [this](std::vector<std::string> params){RequestBall();}, "Give requester the ball", PERMISSION_ALL);
}
void NetcodePluginExample::onUnload() {}

ServerWrapper NetcodePluginExample::GetCurrentGameState()
{
    if(gameWrapper->IsInReplay())
        return gameWrapper->GetGameEventAsReplay().memory_address;
    else if(gameWrapper->IsInOnlineGame())
        return gameWrapper->GetOnlineGame();
    else
        return gameWrapper->GetGameEventAsServer();
}

// SEND REQUEST //
void NetcodePluginExample::RequestBall()
{
    CarWrapper MyCar = gameWrapper->GetLocalCar();
    if(MyCar.IsNull()) { return; }
    PriWrapper MyPRI = MyCar.GetPRI();
    if(MyPRI.IsNull()) { return; }

    Netcode->SendMessage("give me the ball");
}

// FULFILL REQUEST //
void NetcodePluginExample::OnMessageReceived(const std::string& Message, PriWrapper Sender)
{
    if(Sender.IsNull()) { return; }

    if(Message == "give me the ball")
    {
        CarWrapper SenderCar = Sender.GetCar();
        if(SenderCar.IsNull()) { return; }
        ServerWrapper Server = GetCurrentGameState();
        if(Server.IsNull()) { return; }
        BallWrapper Ball = Server.GetBall();
        if(Ball.IsNull()) { return; }

        //All clients should place the ball on the corresponding car
        //This makes replication much smoother and avoids odd interpolation from host
        Ball.SetLocation(SenderCar.GetLocation() + Vector(0, 0, Ball.GetRadius() + 100));
        Ball.SetVelocity(SenderCar.GetVelocity());
    }
}
