#pragma once

#include <cstdint>

class BootManager
{
public:
    static constexpr uint32_t ApplicationStartAddress = 0x08008000U;
    //static constexpr uint32_t ApplicationEndAddress = 0x080FFFFFU;

    void run();
private:
    bool isApplicationValid() const;
    bool jumpToApplication() const;


};

