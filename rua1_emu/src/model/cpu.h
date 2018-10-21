#pragma once

#include <model/sidebar_toggleable.h>
#include <view/frmcpu.h>
#include <mos6502/bus.h>
#include <mos6502/cpu_mos6502.h>
#include <mos6502/icpu_debug_interface.h>

namespace rua1::model
{

class cpu final : public sidebar_toggleable<view::frmcpu>, public mos6502::icpu_debug_interface
{
public:
    explicit cpu(view::imain_window &main_window, mos6502::bus &bus);
    ~cpu();

    cpu(cpu &&) noexcept = delete;
    auto operator=(cpu &&) noexcept -> cpu & = delete;

    cpu(const cpu &) noexcept = delete;
    auto operator=(const cpu &) noexcept -> cpu & = delete;

private:
    void on_view_created() override;
    void on_view_destroyed() override;
    void update_ui();

    void on_cpu_instruction_executed() override;
    void on_cpu_breakpoint() override;
    void on_cpu_illegal_opcode() override;
    void on_cpu_reset() override;
    void on_cpu_nmi() override;
    void on_cpu_irq() override;
    void on_cpu_stack_push(const std::uint8_t byte) override;
    void on_cpu_stack_pop() override;

    mos6502::cpu_mos6502 cpu_;
};

} // namespace rua1::model
