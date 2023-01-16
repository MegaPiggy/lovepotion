#include "modules/system/systemc.h"
#include "common/bidirectionalmap.h"

using namespace love::common;

System::System() : systemInfo()
{ /* initialize sysInfo defaults */
}

std::string System::GetOS() const
{
    return OS_NAME;
}

// clang-format off
constexpr auto powerStates = BidirectionalMap<>::Create(
    "unknown",  System::PowerState::POWER_UNKNOWN,
    "battery",  System::PowerState::POWER_BATTERY,
    "charged",  System::PowerState::POWER_CHARGED,
    "charging", System::PowerState::POWER_CHARGING
);

constexpr auto networkStates = BidirectionalMap<>::Create(
    "unknown",      System::NetworkState::NETWORK_UNKNOWN,
    "connected",    System::NetworkState::NETWORK_CONNECTED,
    "disconnected", System::NetworkState::NETWORK_DISCONNECTED
);

constexpr auto mediaTypes = BidirectionalMap<>::Create(
    "nand",     System::MediaType::MEDIATYPE_NAND,
    "sd",       System::MediaType::MEDIATYPE_SD,
    "gamecard", System::MediaType::MEDIATYPE_GAME_CARD
);
// clang-format on

bool System::GetConstant(const char* in, System::PowerState& out)
{
    return powerStates.Find(in, out);
}

bool System::GetConstant(System::PowerState in, const char*& out)
{
    return powerStates.ReverseFind(in, out);
}

bool System::GetConstant(const char* in, System::NetworkState& out)
{
    return networkStates.Find(in, out);
}

bool System::GetConstant(System::NetworkState in, const char*& out)
{
    return networkStates.ReverseFind(in, out);
}

bool System::GetConstant(const char* in, System::MediaType& out)
{
    return mediaTypes.Find(in, out);
}

bool System::GetConstant(System::MediaType in, const char*& out)
{
    return mediaTypes.ReverseFind(in, out);
}

std::vector<const char*> System::GetConstants(System::MediaType)
{
    return mediaTypes.GetNames();
}
