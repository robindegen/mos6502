#include <model/cpu.h>
#include <QtWidgets>
#include <cassert>

namespace rua1::model
{

cpu::cpu(view::imain_window &main_window, mos6502::bus &bus)
    : sidebar_toggleable<view::frmcpu>{"CPU", true, main_window}
    , cpu_{bus}
{
}

cpu::~cpu() = default;

} // namespace rua1::model
