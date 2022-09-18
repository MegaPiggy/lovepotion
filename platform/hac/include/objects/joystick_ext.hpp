#pragma once

#include <objects/joystick/joystick.tcc>
#include <switch.h>

namespace love
{
    template<>
    class Joystick<Console::HAC> : public Joystick<Console::ALL>
    {
      public:
        Joystick(int id);

        Joystick(int id, int index);

        virtual ~Joystick();

        bool Open(int index);

        void Close();

        bool IsConnected() const;

        bool IsDown(JoystickInput& result);

        bool IsUp(JoystickInput& result);

        void GetDeviceInfo(int& vendor, int& product, int& version);

        int GetAxisCount() const;

        int GetButtonCount() const;

        void Update();

        float GetAxis(int index) const;

        std::vector<float> GetAxes() const;

        bool IsDown(const std::vector<int>& buttons) const;

        void SetPlayerIndex(int index);

        int GetPlayerIndex() const
        {
            return (int)this->playerId;
        }

        bool IsGamepad() const
        {
            return true;
        }

        guid::GamepadType GetGamepadType() const;

        float GetGamepadAxis(GamepadAxis axis) const;

        bool IsGamepadDown(const std::vector<GamepadButton>& buttons) const;

        bool IsVibrationSupported()
        {
            return true;
        }

        bool SetVibration(float left, float right, float duration = -1.0f);

        bool SetVibration();

        void GetVibration(float& left, float& right);

        static bool GetConstant(int in, GamepadButton& out);
        static bool GetConstant(GamepadButton in, int& out);

        static bool GetConstant(uint64_t in, GamepadAxis& out);
        static bool GetConstant(GamepadAxis in, uint64_t& out);

        static bool GetConstant(const char* in, HidNpadJoyDeviceType& out);
        static bool GetConstant(HidNpadJoyDeviceType in, const char*& out);
        static std::vector<const char*> GetConstants(HidNpadJoyDeviceType);

      private:
        PadState state;
        ::Event event;

        HidNpadStyleTag style;
        HidNpadIdType playerId;

        struct
        {
            uint64_t pressed;
            uint64_t released;
        } buttonStates;
    };
} // namespace love