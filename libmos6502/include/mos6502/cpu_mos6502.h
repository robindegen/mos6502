//============================================================================
// Name        : mos6502
// Author      : Gianluca Ghettini
// Version     : 1.0
// Copyright   :
// Description : A MOS 6502 CPU emulator written in C++
//============================================================================

#pragma once

#include <cstdint>
#include <array>

namespace mos6502
{

class cpu_mos6502
{
public:
    // read/write callbacks
    using bus_write_func = void (*)(const std::uint16_t, const std::uint8_t) noexcept;
    using bus_read_func = auto (*)(const std::uint16_t) noexcept -> std::uint8_t;

    cpu_mos6502(const bus_read_func read_func, const bus_write_func write_func);
    ~cpu_mos6502() = default;

    cpu_mos6502(cpu_mos6502 &&) noexcept = delete;
    auto operator=(cpu_mos6502 &&) noexcept -> cpu_mos6502 & = delete;

    cpu_mos6502(const cpu_mos6502 &) noexcept = delete;
    auto operator=(const cpu_mos6502 &) noexcept -> cpu_mos6502 & = delete;

    void nmi() noexcept;
    void irq() noexcept;
    void reset() noexcept;
    void run(const std::uint32_t n) noexcept;

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

    bool illegal_opcode_{};

    std::uint8_t A{};
    std::uint8_t X{};
    std::uint8_t Y{};
    std::uint8_t sp{};
    std::uint16_t pc{};
    std::uint8_t status{};

    // consumed clock cycles
    uint32_t cycles_{};

    bus_read_func bus_read_func_;
    bus_write_func bus_write_func_;
};

} // namespace mos6502
