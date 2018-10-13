//============================================================================
// Name        : mos6502
// Author      : Gianluca Ghettini
// Version     : 1.0
// Copyright   :
// Description : A MOS 6502 CPU emulator written in C++
//============================================================================

#pragma once

#include <iostream>
#include <cstdint>

constexpr auto status_negative = 0x80;
constexpr auto status_overflow = 0x40;
constexpr auto status_constant = 0x20;
constexpr auto status_break = 0x10;
constexpr auto status_decimal = 0x08;
constexpr auto status_interrupt = 0x04;
constexpr auto status_zero = 0x02;
constexpr auto status_carry = 0x01;

#define SET_NEGATIVE(x) (x ? (status |= status_negative) : (status &= (~status_negative)))
#define SET_OVERFLOW(x) (x ? (status |= status_overflow) : (status &= (~status_overflow)))
#define SET_CONSTANT(x) (x ? (status |= status_constant) : (status &= (~status_constant)))
#define SET_BREAK(x) (x ? (status |= status_break) : (status &= (~status_break)))
#define SET_DECIMAL(x) (x ? (status |= status_decimal) : (status &= (~status_decimal)))
#define SET_INTERRUPT(x) (x ? (status |= status_interrupt) : (status &= (~status_interrupt)))
#define SET_ZERO(x) (x ? (status |= status_zero) : (status &= (~status_zero)))
#define SET_CARRY(x) (x ? (status |= status_carry) : (status &= (~status_carry)))

#define IF_NEGATIVE() ((status & status_negative) ? true : false)
#define IF_OVERFLOW() ((status & status_overflow) ? true : false)
#define IF_CONSTANT() ((status & status_constant) ? true : false)
#define IF_BREAK() ((status & status_break) ? true : false)
#define IF_DECIMAL() ((status & status_decimal) ? true : false)
#define IF_INTERRUPT() ((status & status_interrupt) ? true : false)
#define IF_ZERO() ((status & status_zero) ? true : false)
#define IF_CARRY() ((status & status_carry) ? true : false)

class mos6502
{
private:
    // registers
    uint8_t A; // accumulator
    uint8_t X; // X-index
    uint8_t Y; // Y-index

    // stack pointer
    uint8_t sp;

    // program counter
    uint16_t pc;

    // status register
    uint8_t status;

    // consumed clock cycles
    uint32_t cycles;

    typedef void (mos6502::*CodeExec)(uint16_t);
    typedef uint16_t (mos6502::*AddrExec)();

    struct Instr
    {
        AddrExec addr;
        CodeExec code;
    };

    Instr InstrTable[256];

    void Exec(Instr i);

    bool illegalOpcode;

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

    // IRQ, reset, NMI vectors
    static const uint16_t irqVectorH = 0xFFFF;
    static const uint16_t irqVectorL = 0xFFFE;
    static const uint16_t rstVectorH = 0xFFFD;
    static const uint16_t rstVectorL = 0xFFFC;
    static const uint16_t nmiVectorH = 0xFFFB;
    static const uint16_t nmiVectorL = 0xFFFA;

    // read/write callbacks
    typedef void (*BusWrite)(uint16_t, uint8_t);
    typedef uint8_t (*BusRead)(uint16_t);
    BusRead Read;
    BusWrite Write;

    // stack operations
    inline void StackPush(uint8_t byte);
    inline uint8_t StackPop();

public:
    mos6502(BusRead r, BusWrite w);
    void NMI();
    void IRQ();
    void Reset();
    void Run(uint32_t n);
};
