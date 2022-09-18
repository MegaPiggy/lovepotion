#include <objects/joystick_ext.hpp>
#include <utilities/bidirectionalmap.hpp>

#include <algorithm>
#include <limits>

using namespace love;

template<>
Type Joystick<>::type("Joystick", &Object::type);

// clang-format off
constexpr auto buttons = BidirectionalMap<>::Create(
    Joystick<>::GAMEPAD_BUTTON_A,             KEY_A,
    Joystick<>::GAMEPAD_BUTTON_B,             KEY_B,
    Joystick<>::GAMEPAD_BUTTON_X,             KEY_X,
    Joystick<>::GAMEPAD_BUTTON_Y,             KEY_Y,
    Joystick<>::GAMEPAD_BUTTON_BACK,          KEY_SELECT,
    Joystick<>::GAMEPAD_BUTTON_GUIDE,         -1,
    Joystick<>::GAMEPAD_BUTTON_START,         KEY_START,
    Joystick<>::GAMEPAD_BUTTON_LEFTSHOULDER,  KEY_L,
    Joystick<>::GAMEPAD_BUTTON_RIGHTSHOULDER, KEY_R,
    Joystick<>::GAMEPAD_BUTTON_LEFTSTICK,     -1,
    Joystick<>::GAMEPAD_BUTTON_RIGHTSTICK,    -1,
    Joystick<>::GAMEPAD_BUTTON_DPAD_UP,       KEY_DUP,
    Joystick<>::GAMEPAD_BUTTON_DPAD_DOWN,     KEY_DDOWN,
    Joystick<>::GAMEPAD_BUTTON_DPAD_RIGHT,    KEY_DRIGHT,
    Joystick<>::GAMEPAD_BUTTON_DPAD_LEFT,     KEY_LEFT
);
// clang-format on

bool Joystick<Console::CTR>::GetConstant(GamepadButton in, int& out)
{
    return buttons.Find(in, out);
}

bool Joystick<Console::CTR>::GetConstant(int in, GamepadButton& out)
{
    return buttons.ReverseFind(in, out);
}

// clang-format off
static constexpr auto axes = BidirectionalMap<>::Create(
    Joystick<>::GAMEPAD_AXIS_LEFTX,        KEY_CPAD_LEFT   | KEY_CPAD_RIGHT,
    Joystick<>::GAMEPAD_AXIS_LEFTY,        KEY_CPAD_UP     | KEY_CPAD_DOWN,
    Joystick<>::GAMEPAD_AXIS_RIGHTX,       KEY_CSTICK_LEFT | KEY_CSTICK_RIGHT,
    Joystick<>::GAMEPAD_AXIS_RIGHTY,       KEY_CSTICK_UP   | KEY_CSTICK_DOWN,
    Joystick<>::GAMEPAD_AXIS_TRIGGERLEFT,  KEY_ZL,
    Joystick<>::GAMEPAD_AXIS_TRIGGERRIGHT, KEY_ZR
);
// clang-format on

bool Joystick<Console::CTR>::GetConstant(GamepadAxis in, size_t& out)
{
    return axes.Find(in, out);
}

bool Joystick<Console::CTR>::GetConstant(size_t in, GamepadAxis& out)
{
    return axes.ReverseFind(in, out);
}

Joystick<Console::CTR>::Joystick(int id) : buttonStates {}
{
    this->instanceId = -1;
    this->id         = id;

    this->guid   = guid::GetDeviceGUID(this->GetGamepadType());
    this->handle = std::make_unique<uint8_t>(1);
}

Joystick<Console::CTR>::Joystick(int id, int index) : Joystick(id)
{
    this->Open(index);
}

Joystick<Console::CTR>::~Joystick()
{
    this->Close();
}

bool Joystick<Console::CTR>::Open(int index)
{
    this->Close();

    this->name = guid::GetDeviceName(this->GetGamepadType());

    this->instanceId = index;

    return this->IsConnected();
}

void Joystick<Console::CTR>::Close()
{
    this->instanceId = -1;
}

void Joystick<Console::CTR>::GetDeviceInfo(int& vendor, int& product, int& version)
{
    guid::DeviceInfo info {};

    if (!guid::GetDeviceInfo(this->GetGamepadType(), info))
        return;

    vendor  = info.vendorId;
    product = info.productId;
    version = info.productVersion;
}

int Joystick<Console::CTR>::GetAxisCount() const
{
    bool isN3DS = false;
    APT_CheckNew3DS(&isN3DS);

    int count = 6;
    return isN3DS ? count : count - 2;
}

int Joystick<Console::CTR>::GetButtonCount() const
{
    return 12;
}

void Joystick<Console::CTR>::Update()
{
    this->buttonStates.pressed  = hidKeysDown();
    this->buttonStates.released = hidKeysUp();
}

bool Joystick<Console::CTR>::IsDown(JoystickInput& result)
{
    if (!this->IsConnected())
        return false;

    if (!this->buttonStates.pressed)
        return false;

    uint32_t button = 0;

    const auto entries = buttons.GetEntries();

    for (size_t index = 0; index < entries.second; index++)
    {
        button = (uint32_t)entries.first[index].second;

        if (entries.first[index].second == -1)
            continue;

        if (button & this->buttonStates.pressed)
        {
            this->buttonStates.pressed ^= button;
            result = { .type         = InputType::INPUT_TYPE_BUTTON,
                       .button       = entries.first[index].first,
                       .buttonNumber = (int)index };

            return true;
        }
    }

    return false;
}

bool Joystick<Console::CTR>::IsUp(JoystickInput& result)
{
    uint32_t button = 0;

    if (!this->buttonStates.released)
        return false;

    const auto entries = buttons.GetEntries();

    for (size_t index = 0; index < entries.second; index++)
    {
        button = (uint32_t)entries.first[index].second;

        if (entries.first[index].second == -1)
            continue;

        if (button & this->buttonStates.released)
        {
            this->buttonStates.released ^= button;
            result = { .type         = InputType::INPUT_TYPE_BUTTON,
                       .button       = entries.first[index].first,
                       .buttonNumber = (int)index };

            return true;
        }
    }

    return false;
}

float Joystick<Console::CTR>::GetAxis(int index) const
{
    if (!this->IsConnected() || index < 0 || index >= this->GetAxisCount())
        return 0.0f;

    if (index == 1 || index == 2)
    {
        circlePosition leftStick {};
        hidCircleRead(&leftStick);

        float value = (index == 2) ? leftStick.dx : leftStick.dy;
        return std::clamp<float>(value / Joystick::JoystickMax, 0, 1.0f);
    }
    else if (index == 3 || index == 4)
    {
        circlePosition rightStick {};
        irrstCstickRead(&rightStick);

        float value = (index == 4) ? rightStick.dx : rightStick.dy;
        return std::clamp<float>(value / Joystick::JoystickMax, 0, 1.0f);
    }
    else if (index == 5)
    {
        if (hidKeysHeld() & KEY_ZL)
            return 1.0f;

        return 0.0f;
    }
    else if (index == 6)
    {
        if (hidKeysHeld() & KEY_ZR)
            return 1.0f;

        return 0.0f;
    }
    else
    {
        if (index >= 7 && index < 10)
        {
            angularRate gyroScope {};
            hidGyroRead(&gyroScope);

            if (index == 7)
                return gyroScope.x;
            else if (index == 8)
                return gyroScope.y;

            return gyroScope.z;
        }
        else if (index >= 10 && index < 13)
        {
            accelVector accelerometer {};
            hidAccelRead(&accelerometer);

            if (index == 10)
                return accelerometer.x;
            else if (index == 11)
                return accelerometer.y;

            return accelerometer.z;
        }
    }

    return 0.0f;
}

std::vector<float> Joystick<Console::CTR>::GetAxes() const
{
    std::vector<float> axes;
    int count = this->GetAxisCount();

    if (!this->IsConnected() || count <= 0)
        return axes;

    axes.reserve(count);

    for (int index = 0; index < count; index++)
        axes.push_back(this->GetAxis(index));

    return axes;
}

bool Joystick<Console::CTR>::IsDown(const std::vector<int>& buttons) const
{
    if (!this->IsConnected())
        return false;

    int count    = this->GetButtonCount();
    auto records = ::buttons.GetEntries().first;

    for (int button : buttons)
    {
        if (button < 0 || button >= count)
            continue;

        if (hidKeysHeld() & records[button].second)
            return true;
    }

    return false;
}

float Joystick<Console::CTR>::GetGamepadAxis(GamepadAxis axis) const
{
    if (!this->IsConnected())
        return 0.0f;

    return this->GetAxis((int)axis);
}

bool Joystick<Console::CTR>::IsGamepadDown(const std::vector<GamepadButton>& buttons) const
{
    GamepadButton gamepadButton;
    const char* name = nullptr;

    uint32_t heldSet = hidKeysHeld();

    for (auto button : buttons)
    {
        if (!Joystick<>::GetConstant(button, name))
            continue;

        if (!Joystick<>::GetConstant(name, gamepadButton))
            continue;

        if (heldSet & (uint32_t)gamepadButton)
            return true;
    }

    return false;
}