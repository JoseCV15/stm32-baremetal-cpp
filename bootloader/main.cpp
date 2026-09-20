#include "BootManager.hpp"

extern "C" int main()
{
    BootManager bootManager;

    bootManager.run();

    while (true)
    {
    }
    
}