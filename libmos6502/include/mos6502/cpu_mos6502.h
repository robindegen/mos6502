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

    void nmi();
    void irq();
    void reset();
    void run(const std::uint32_t n);

private:
    using opcode_exec_func = void (cpu_mos6502::*)(std::uint16_t);
    using addr_exec_func = std::uint16_t (cpu_mos6502::*)();

    struct instruction
    {
        addr_exec_func addr;
        opcode_exec_func code;
    };

    void exec(const instruction i);

    // addressing modes
    uint16_t Addr_ACC(); // ACCUMULATOR
    uint16_t Addr_IMM(); // IMMEDIATE
    uint16_t Addr_ABS(); // ABSOLUTE
    uint16_t Addr_ZER(); // ZERO PAGE
    uint16_t Addr_ZEX(); // INDEXED-X ZERO PAGE
    uint16_t Addr_ZEY(); // INDEXED-Y ZERO PAGE
    uint16_t Addr_ABX(); // INDEXED-X ABSOLUTE
    uint16_t Addr_ABY(); // INDEXED-Y ABSOLUTE
    uint16_t Addr_IMP(); // IMPLIED
    uint16_t Addr_REL(); // RELATIVE
    uint16_t Addr_INX(); // INDEXED-X INDIRECT
    uint16_t Addr_INY(); // INDEXED-Y INDIRECT
    uint16_t Addr_ABI(); // ABSOLUTE INDIRECT

    // opcodes (grouped as per datasheet)
    void Op_ADC(uint16_t src);
    void Op_AND(uint16_t src);
    void Op_ASL(uint16_t src);
    void Op_ASL_ACC(uint16_t src);
    void Op_BCC(uint16_t src);
    void Op_BCS(uint16_t src);

    void Op_BEQ(uint16_t src);
    void Op_BIT(uint16_t src);
    void Op_BMI(uint16_t src);
    void Op_BNE(uint16_t src);
    void Op_BPL(uint16_t src);

    void Op_BRK(uint16_t src);
    void Op_BVC(uint16_t src);
    void Op_BVS(uint16_t src);
    void Op_CLC(uint16_t src);
    void Op_CLD(uint16_t src);

    void Op_CLI(uint16_t src);
    void Op_CLV(uint16_t src);
    void Op_CMP(uint16_t src);
    void Op_CPX(uint16_t src);
    void Op_CPY(uint16_t src);

    void Op_DEC(uint16_t src);
    void Op_DEX(uint16_t src);
    void Op_DEY(uint16_t src);
    void Op_EOR(uint16_t src);
    void Op_INC(uint16_t src);

    void Op_INX(uint16_t src);
    void Op_INY(uint16_t src);
    void Op_JMP(uint16_t src);
    void Op_JSR(uint16_t src);
    void Op_LDA(uint16_t src);

    void Op_LDX(uint16_t src);
    void Op_LDY(uint16_t src);
    void Op_LSR(uint16_t src);
    void Op_LSR_ACC(uint16_t src);
    void Op_NOP(uint16_t src);
    void Op_ORA(uint16_t src);

    void Op_PHA(uint16_t src);
    void Op_PHP(uint16_t src);
    void Op_PLA(uint16_t src);
    void Op_PLP(uint16_t src);
    void Op_ROL(uint16_t src);
    void Op_ROL_ACC(uint16_t src);

    void Op_ROR(uint16_t src);
    void Op_ROR_ACC(uint16_t src);
    void Op_RTI(uint16_t src);
    void Op_RTS(uint16_t src);
    void Op_SBC(uint16_t src);
    void Op_SEC(uint16_t src);
    void Op_SED(uint16_t src);

    void Op_SEI(uint16_t src);
    void Op_STA(uint16_t src);
    void Op_STX(uint16_t src);
    void Op_STY(uint16_t src);
    void Op_TAX(uint16_t src);

    void Op_TAY(uint16_t src);
    void Op_TSX(uint16_t src);
    void Op_TXA(uint16_t src);
    void Op_TXS(uint16_t src);
    void Op_TYA(uint16_t src);

    void Op_ILLEGAL(uint16_t src);

    void initialize_illegal_opcodes();
    void initialize_opcodes();

    // stack operations
    void stack_push(std::uint8_t byte) noexcept;
    auto stack_pop() noexcept -> std::uint8_t;

    // IRQ, reset, NMI vectors
    static const uint16_t irqVectorH = 0xFFFF;
    static const uint16_t irqVectorL = 0xFFFE;
    static const uint16_t rstVectorH = 0xFFFD;
    static const uint16_t rstVectorL = 0xFFFC;
    static const uint16_t nmiVectorH = 0xFFFB;
    static const uint16_t nmiVectorL = 0xFFFA;

    std::array<instruction, 256> instruction_;

    bool illegalOpcode{};

    std::uint8_t A{};
    std::uint8_t X{};
    std::uint8_t Y{};
    std::uint8_t sp{};
    std::uint16_t pc{};
    std::uint8_t status{};

    // consumed clock cycles
    uint32_t cycles{};

    bus_read_func bus_read_func_;
    bus_write_func bus_write_func_;
};

} // namespace mos6502
