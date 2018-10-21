#include <model/cpu.h>
#include <QtWidgets>
#include <cassert>

namespace rua1::model
{

cpu::cpu(view::imain_window &main_window, mos6502::bus &bus)
    : sidebar_toggleable<view::frmcpu>{"CPU", main_window}
    , cpu_{bus}
{
}

cpu::~cpu() = default;

void cpu::on_view_created()
{
    update_ui();
}

void cpu::on_view_destroyed()
{
}

void cpu::update_ui()
{
    if (!view())
        return;

    view()->set_pc_value(cpu_.pc());
    view()->set_sp_value(cpu_.sp());
    view()->set_a_value(cpu_.a());
    view()->set_x_value(cpu_.x());
    view()->set_y_value(cpu_.y());
}

void cpu::on_cpu_instruction_executed()
{
    update_ui();
}

void cpu::on_cpu_breakpoint()
{
}

void cpu::on_cpu_illegal_opcode()
{
}

void cpu::on_cpu_reset()
{
    update_ui();
}

void cpu::on_cpu_nmi()
{
}

void cpu::on_cpu_irq()
{
}

void cpu::on_cpu_stack_push(const std::uint8_t byte)
{
}

void cpu::on_cpu_stack_pop()
{
}

} // namespace rua1::model
