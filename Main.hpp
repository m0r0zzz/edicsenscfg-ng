#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdint>
#include <stdexcept>

enum class State{
    NOP,
    REG_WR,
    VAR_WR,
    REG_RD,
    VAR_RD
};

#endif // MAIN_HPP_INCLUDED
