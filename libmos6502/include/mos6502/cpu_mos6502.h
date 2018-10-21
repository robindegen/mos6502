//============================================================================
// Name        : mos6502
// Author      : Gianluca Ghettini
// Version     : 1.0
// Copyright   :
// Description : A MOS 6502 CPU emulator written in C++
//============================================================================

#pragma once

#include <mos6502/ibus_interface.h>
#include <cstdint>
#include <array>

namespace mos6502
{

class icpu_debug_interface;
class bus;

class cpu_mos6502 final : public ibus_interface
{
public:
    explicit cpu_mos6502(bus &bus, icpu_debug_interface *debug_interface = nullptr);
    ~cpu_mos6502() = default;

    cpu_mos6502(cpu_mos6502 &&) noexcept = delete;
    auto operator=(cpu_mos6502 &&) noexcept -> cpu_mos6502 & = delete;

    cpu_mos6502(const cpu_mos6502 &) noexcept = delete;
    auto operator=(const cpu_mos6502 &) noexcept -> cpu_mos6502 & = delete;

    void trigger_nmi() noexcept;
    void trigger_irq() noexcept;
    void reset() noexcept;

    void run() noexcept;
    void stop() noexcept;

    void step(const std::uint32_t n = 1) noexcept;

    auto a() const noexcept
    {
        return register_a_;
    }

    auto x() const noexcept
    {
        return register_x_;
    }

    auto y() const noexcept
    {
        return register_y_;
    }

    auto sp() const noexcept
    {
        return register_sp_;
    }

    auto pc() const noexcept
    {
        return register_pc_;
    }

    auto status() const noexcept
    {
        return register_status_;
    }

    auto num_executed_instructions() const noexcept
    {
        return num_executed_instructions_;
    }

    auto is_illegal_opcode_set() const noexcept -> bool;

private:
    using opcode_exec_func = void (cpu_mos6502::*)(std::uint16_t) noexcept;
    using addr_exec_func = auto (cpu_mos6502::*)() noexcept -> std::uint16_t;

    struct instruction
    {
        addr_exec_func addr;
        opcode_exec_func code;
    };

    void exec(const instruction i) noexcept;

    void stack_push(std::uint8_t byte) noexcept;
    auto stack_pop() noexcept -> std::uint8_t;

    void bus_write(const std::uint16_t address, const std::uint8_t value) const noexcept;
    auto bus_read(const std::uint16_t address) const noexcept -> std::uint8_t;

    void on_irq() noexcept override;

    // addressing modes
    auto addr_acc() noexcept -> std::uint16_t; // ACCUMULATOR
    auto addr_imm() noexcept -> std::uint16_t; // IMMEDIATE
    auto addr_abs() noexcept -> std::uint16_t; // ABSOLUTE
    auto addr_zer() noexcept -> std::uint16_t; // ZERO PAGE
    auto addr_zex() noexcept -> std::uint16_t; // INDEXED-X ZERO PAGE
    auto addr_zey() noexcept -> std::uint16_t; // INDEXED-Y ZERO PAGE
    auto addr_abx() noexcept -> std::uint16_t; // INDEXED-X ABSOLUTE
    auto addr_aby() noexcept -> std::uint16_t; // INDEXED-Y ABSOLUTE
    auto addr_imp() noexcept -> std::uint16_t; // IMPLIED
    auto addr_rel() noexcept -> std::uint16_t; // RELATIVE
    auto addr_inx() noexcept -> std::uint16_t; // INDEXED-X INDIRECT
    auto addr_iny() noexcept -> std::uint16_t; // INDEXED-Y INDIRECT
    auto addr_abi() noexcept -> std::uint16_t; // ABSOLUTE INDIRECT

    // opcodes (grouped as per datasheet)
    void op_adc(std::uint16_t src) noexcept;
    void op_and(std::uint16_t src) noexcept;
    void op_asl(std::uint16_t src) noexcept;
    void op_asl_acc(std::uint16_t src) noexcept;
    void op_bcc(std::uint16_t src) noexcept;
    void op_bcs(std::uint16_t src) noexcept;

    void op_beq(std::uint16_t src) noexcept;
    void op_bit(std::uint16_t src) noexcept;
    void op_bmi(std::uint16_t src) noexcept;
    void op_bne(std::uint16_t src) noexcept;
    void op_bpl(std::uint16_t src) noexcept;

    void op_brk(std::uint16_t src) noexcept;
    void op_bvc(std::uint16_t src) noexcept;
    void op_bvs(std::uint16_t src) noexcept;
    void op_clc(std::uint16_t src) noexcept;
    void op_cld(std::uint16_t src) noexcept;

    void op_cli(std::uint16_t src) noexcept;
    void op_clv(std::uint16_t src) noexcept;
    void op_cmp(std::uint16_t src) noexcept;
    void op_cpx(std::uint16_t src) noexcept;
    void op_cpy(std::uint16_t src) noexcept;

    void op_dec(std::uint16_t src) noexcept;
    void op_dex(std::uint16_t src) noexcept;
    void op_dey(std::uint16_t src) noexcept;
    void op_eor(std::uint16_t src) noexcept;
    void op_inc(std::uint16_t src) noexcept;

    void op_inx(std::uint16_t src) noexcept;
    void op_iny(std::uint16_t src) noexcept;
    void op_jmp(std::uint16_t src) noexcept;
    void op_jsr(std::uint16_t src) noexcept;
    void op_lda(std::uint16_t src) noexcept;

    void op_ldx(std::uint16_t src) noexcept;
    void op_ldy(std::uint16_t src) noexcept;
    void op_lsr(std::uint16_t src) noexcept;
    void op_lsr_acc(std::uint16_t src) noexcept;
    void op_nop(std::uint16_t src) noexcept;
    void op_ora(std::uint16_t src) noexcept;

    void op_pha(std::uint16_t src) noexcept;
    void op_php(std::uint16_t src) noexcept;
    void op_pla(std::uint16_t src) noexcept;
    void op_plp(std::uint16_t src) noexcept;
    void op_rol(std::uint16_t src) noexcept;
    void op_rol_acc(std::uint16_t src) noexcept;

    void op_ror(std::uint16_t src) noexcept;
    void op_ror_acc(std::uint16_t src) noexcept;
    void op_rti(std::uint16_t src) noexcept;
    void op_rts(std::uint16_t src) noexcept;
    void op_sbc(std::uint16_t src) noexcept;
    void op_sec(std::uint16_t src) noexcept;
    void op_sed(std::uint16_t src) noexcept;

    void op_sei(std::uint16_t src) noexcept;
    void op_sta(std::uint16_t src) noexcept;
    void op_stx(std::uint16_t src) noexcept;
    void op_sty(std::uint16_t src) noexcept;
    void op_tax(std::uint16_t src) noexcept;

    void op_tay(std::uint16_t src) noexcept;
    void op_tsx(std::uint16_t src) noexcept;
    void op_txa(std::uint16_t src) noexcept;
    void op_txs(std::uint16_t src) noexcept;
    void op_tya(std::uint16_t src) noexcept;

    void op_illegal(std::uint16_t src) noexcept;

    void initialize_illegal_opcodes() noexcept;
    void initialize_opcodes() noexcept;

    std::array<instruction, 256> instruction_;

    bool running_{};

    std::uint8_t register_a_{};
    std::uint8_t register_x_{};
    std::uint8_t register_y_{};
    std::uint8_t register_sp_{};
    std::uint16_t register_pc_{};
    std::uint8_t register_status_{};
    std::uint32_t num_executed_instructions_{};
    bool illegal_opcode_{};

    bus &bus_;
    icpu_debug_interface *debug_interface_;
};

} // namespace mos6502
