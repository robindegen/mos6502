#pragma once

#include <cstdint>

namespace emu6502
{

class cpu_mos6502;

class icpu_debug_interface
{
public:
    icpu_debug_interface(icpu_debug_interface &&) noexcept = delete;
    auto operator=(icpu_debug_interface &&) noexcept -> icpu_debug_interface & = delete;

    icpu_debug_interface(const icpu_debug_interface &) noexcept = delete;
    auto operator=(const icpu_debug_interface &) noexcept -> icpu_debug_interface & = delete;

    virtual void on_cpu_instruction_executed() = 0;
    virtual void on_cpu_breakpoint() = 0;
    virtual void on_cpu_illegal_opcode() = 0;

    virtual void on_cpu_reset() = 0;
    virtual void on_cpu_nmi() = 0;
    virtual void on_cpu_irq() = 0;

    virtual void on_cpu_stack_push(const std::uint8_t byte) = 0;
    virtual void on_cpu_stack_pop() = 0;

protected:
    icpu_debug_interface() = default;
    virtual ~icpu_debug_interface() = default;
};

} // namespace emu6502
