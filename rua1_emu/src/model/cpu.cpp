#include <model/cpu.h>
#include <QtWidgets>
#include <cassert>

namespace rua1::model
{

cpu::cpu(view::imain_window &main_window, emu6502::bus &bus)
    : sidebar_toggleable<view::frmcpu, view::frmcpu_model_interface>{*this, "CPU", main_window}
    , cpu_{bus, this}
    , hex_view_selected_{true}
{
}

cpu::~cpu() = default;

void cpu::on_ui_btn_reset_clicked()
{
    cpu_.reset();
}

void cpu::on_ui_btn_step_clicked()
{
    cpu_.step(1);
}

void cpu::on_ui_hex_selected()
{
    hex_view_selected_ = true;
    update_ui();
}

void cpu::on_ui_dec_selected()
{
    hex_view_selected_ = false;
    update_ui();
}

void cpu::on_view_created()
{
    view()->set_hex_view(hex_view_selected_);
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
