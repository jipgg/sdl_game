#pragma once
#include <iostream>
#include <windows.h>
enum class Console_clr {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    PURPLE = 0x5,
    YELLOW = 0x6,
    WHITE = 0x7,
    GRAY = 0x8,
    LIGHT_BLUE = 0x9,
    LIGHT_GREEN = 0xA,
    LIGHT_CYAN = 0xB,
    LIGHT_RED = 0xC,
    LIGHT_PURPLE = 0xD,
    LIGHT_YELLOW = 0xE,
    BRIGHT_WHITE = 0xF,
};
template<class ...Params>
void println(Params&&...arg) {
    ((std::cout << arg << " "), ...) << std::endl;
}
template <class...Params>
void print(Params&&...arg) {
    ((std::cout << arg << " "),...);
}
template <class ...Params>
void print(Console_clr color, Params&&...arg) {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, WORD(color));
    print(arg...);
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::WHITE));
}
template <class ...Params>
void println(Console_clr color, Params&&...arg) {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, WORD(color));
    println(arg...);
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::WHITE));
}
template <class...Params>
void prerr(Params&&...arg) {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::LIGHT_RED));
    ((std::cerr << arg << ""),...);
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::WHITE));
}
template <class...Params>
void prerrln(Params&&...arg) {
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::LIGHT_RED));
    ((std::cerr << arg << ""),...) << std::endl;
    SetConsoleTextAttribute(hconsole, WORD(Console_clr::WHITE));
}
