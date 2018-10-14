#include <mos6502/cpu_mos6502.h>
#include <status_registers.h>
#include <functional>

namespace mos6502
{

cpu_mos6502::cpu_mos6502(const bus_read_func read_func, const bus_write_func write_func)
    : instruction_{}
    , bus_read_func_{read_func}
    , bus_write_func_{write_func}
{
    initialize_illegal_opcodes();
    initialize_opcodes();
    reset();
}

uint16_t cpu_mos6502::Addr_ACC()
{
    return 0; // not used
}

uint16_t cpu_mos6502::Addr_IMM()
{
    return pc++;
}

uint16_t cpu_mos6502::Addr_ABS()
{
    const std::uint16_t addr_l = bus_read_func_(pc++);
    const std::uint16_t addr_h = bus_read_func_(pc++);
    return addr_l + (addr_h << 8);
}

uint16_t cpu_mos6502::Addr_ZER()
{
    return bus_read_func_(pc++);
}

uint16_t cpu_mos6502::Addr_IMP()
{
    return 0; // not used
}

uint16_t cpu_mos6502::Addr_REL()
{
    auto offset = static_cast<uint16_t>(bus_read_func_(pc++));

    if (offset & 0x80)
        offset |= 0xFF00;

    return pc + static_cast<int16_t>(offset);
}

uint16_t cpu_mos6502::Addr_ABI()
{
    const std::uint16_t addr_l = bus_read_func_(pc++);
    const std::uint16_t addr_h = bus_read_func_(pc++);

    const std::uint16_t abs = (addr_h << 8) | addr_l;

    const std::uint16_t eff_l = bus_read_func_(abs);
    const std::uint16_t eff_h = bus_read_func_((abs & 0xFF00) + ((abs + 1) & 0x00FF));

    return eff_l + 0x100 * eff_h;
}

uint16_t cpu_mos6502::Addr_ZEX()
{
    return (bus_read_func_(pc++) + X) % 256;
}

uint16_t cpu_mos6502::Addr_ZEY()
{
    return (bus_read_func_(pc++) + Y) % 256;
}

uint16_t cpu_mos6502::Addr_ABX()
{
    const std::uint16_t addr_l = bus_read_func_(pc++);
    const std::uint16_t addr_h = bus_read_func_(pc++);
    return addr_l + (addr_h << 8) + X;
}

uint16_t cpu_mos6502::Addr_ABY()
{
    const std::uint16_t addr_l = bus_read_func_(pc++);
    const std::uint16_t addr_h = bus_read_func_(pc++);
    return addr_l + (addr_h << 8) + Y;
}

uint16_t cpu_mos6502::Addr_INX()
{
    const std::uint16_t zero_l = (bus_read_func_(pc++) + X) % 256;
    const std::uint16_t zero_h = (zero_l + 1) % 256;
    return bus_read_func_(zero_l) + (bus_read_func_(zero_h) << 8);
}

uint16_t cpu_mos6502::Addr_INY()
{
    const std::uint16_t zero_l = bus_read_func_(pc++);
    const std::uint16_t zero_h = (zero_l + 1) % 256;
    return bus_read_func_(zero_l) + (bus_read_func_(zero_h) << 8) + Y;
}

void cpu_mos6502::reset()
{
    A = 0x00;
    Y = 0x00;
    X = 0x00;

    pc = (bus_read_func_(rstVectorH) << 8) + bus_read_func_(rstVectorL); // load PC from reset vector

    sp = 0xFD;

    status |= status::constant_flag;

    cycles = 6; // according to the datasheet, the reset routine takes 6 clock cycles

    illegal_opcode_ = false;
}

void cpu_mos6502::stack_push(std::uint8_t byte) noexcept
{
    bus_write_func_(0x0100 + sp, byte);

    if (sp == 0x00)
        sp = 0xFF;
    else
        sp--;
}

auto cpu_mos6502::stack_pop() noexcept -> std::uint8_t
{
    if (sp == 0xFF)
        sp = 0x00;
    else
        sp++;

    return bus_read_func_(0x0100 + sp);
}

void cpu_mos6502::irq()
{
    if (!status::is_interrupt_flag_set(status))
    {
        status::set_break(status, 0);
        stack_push((pc >> 8) & 0xFF);
        stack_push(pc & 0xFF);
        stack_push(status);
        status::set_interrupt(status, 1);
        pc = (bus_read_func_(irqVectorH) << 8) + bus_read_func_(irqVectorL);
    }
}

void cpu_mos6502::nmi()
{
    status::set_break(status, 0);
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    stack_push(status);
    status::set_interrupt(status, 1);
    pc = (bus_read_func_(nmiVectorH) << 8) + bus_read_func_(nmiVectorL);
}

void cpu_mos6502::run(const std::uint32_t n)
{
    const auto start = cycles;

    while (start + n > cycles && !illegal_opcode_)
    {
        // fetch
        const auto opcode = bus_read_func_(pc++);

        // decode
        const auto instr = instruction_[opcode];

        // execute
        exec(instr);

        cycles++;
    }
}

auto cpu_mos6502::is_illegal_opcode_set() const noexcept -> bool
{
    return illegal_opcode_;
}

void cpu_mos6502::exec(const instruction i)
{
    const auto src = std::invoke(i.addr, *this);
    std::invoke(i.code, *this, src);
}

void cpu_mos6502::Op_ILLEGAL(uint16_t src)
{
    illegal_opcode_ = true;
}

void cpu_mos6502::initialize_illegal_opcodes()
{
    for (auto &i : instruction_)
    {
        i = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_ILLEGAL};
    }
}

void cpu_mos6502::initialize_opcodes()
{
    instruction_[0x69] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_ADC};
    instruction_[0x6D] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_ADC};
    instruction_[0x65] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_ADC};
    instruction_[0x61] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_ADC};
    instruction_[0x71] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_ADC};
    instruction_[0x75] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_ADC};
    instruction_[0x7D] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_ADC};
    instruction_[0x79] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_ADC};

    instruction_[0x29] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_AND};
    instruction_[0x2D] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_AND};
    instruction_[0x25] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_AND};
    instruction_[0x21] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_AND};
    instruction_[0x31] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_AND};
    instruction_[0x35] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_AND};
    instruction_[0x3D] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_AND};
    instruction_[0x39] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_AND};

    instruction_[0x0E] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_ASL};
    instruction_[0x06] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_ASL};
    instruction_[0x0A] = {&cpu_mos6502::Addr_ACC, &cpu_mos6502::Op_ASL_ACC};
    instruction_[0x16] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_ASL};
    instruction_[0x1E] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_ASL};

    instruction_[0x90] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BCC};

    instruction_[0xB0] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BCS};

    instruction_[0xF0] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BEQ};

    instruction_[0x2C] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_BIT};
    instruction_[0x24] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_BIT};

    instruction_[0x30] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BMI};

    instruction_[0xD0] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BNE};

    instruction_[0x10] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BPL};

    instruction_[0x00] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_BRK};

    instruction_[0x50] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BVC};

    instruction_[0x70] = {&cpu_mos6502::Addr_REL, &cpu_mos6502::Op_BVS};

    instruction_[0x18] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_CLC};

    instruction_[0xD8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_CLD};

    instruction_[0x58] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_CLI};

    instruction_[0xB8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_CLV};

    instruction_[0xC9] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_CMP};
    instruction_[0xCD] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_CMP};
    instruction_[0xC5] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_CMP};
    instruction_[0xC1] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_CMP};
    instruction_[0xD1] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_CMP};
    instruction_[0xD5] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_CMP};
    instruction_[0xDD] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_CMP};
    instruction_[0xD9] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_CMP};

    instruction_[0xE0] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_CPX};
    instruction_[0xEC] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_CPX};
    instruction_[0xE4] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_CPX};

    instruction_[0xC0] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_CPY};
    instruction_[0xCC] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_CPY};
    instruction_[0xC4] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_CPY};

    instruction_[0xCE] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_DEC};
    instruction_[0xC6] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_DEC};
    instruction_[0xD6] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_DEC};
    instruction_[0xDE] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_DEC};

    instruction_[0xCA] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_DEX};

    instruction_[0x88] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_DEY};

    instruction_[0x49] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_EOR};
    instruction_[0x4D] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_EOR};
    instruction_[0x45] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_EOR};
    instruction_[0x41] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_EOR};
    instruction_[0x51] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_EOR};
    instruction_[0x55] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_EOR};
    instruction_[0x5D] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_EOR};
    instruction_[0x59] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_EOR};

    instruction_[0xEE] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_INC};
    instruction_[0xE6] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_INC};
    instruction_[0xF6] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_INC};
    instruction_[0xFE] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_INC};

    instruction_[0xE8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_INX};

    instruction_[0xC8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_INY};

    instruction_[0x4C] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_JMP};
    instruction_[0x6C] = {&cpu_mos6502::Addr_ABI, &cpu_mos6502::Op_JMP};

    instruction_[0x20] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_JSR};

    instruction_[0xA9] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_LDA};
    instruction_[0xAD] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_LDA};
    instruction_[0xA5] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_LDA};
    instruction_[0xA1] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_LDA};
    instruction_[0xB1] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_LDA};
    instruction_[0xB5] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_LDA};
    instruction_[0xBD] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_LDA};
    instruction_[0xB9] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_LDA};

    instruction_[0xA2] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_LDX};
    instruction_[0xAE] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_LDX};
    instruction_[0xA6] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_LDX};
    instruction_[0xBE] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_LDX};
    instruction_[0xB6] = {&cpu_mos6502::Addr_ZEY, &cpu_mos6502::Op_LDX};

    instruction_[0xA0] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_LDY};
    instruction_[0xAC] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_LDY};
    instruction_[0xA4] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_LDY};
    instruction_[0xB4] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_LDY};
    instruction_[0xBC] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_LDY};

    instruction_[0x4E] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_LSR};
    instruction_[0x46] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_LSR};
    instruction_[0x4A] = {&cpu_mos6502::Addr_ACC, &cpu_mos6502::Op_LSR_ACC};
    instruction_[0x56] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_LSR};
    instruction_[0x5E] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_LSR};

    instruction_[0xEA] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_NOP};

    instruction_[0x09] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_ORA};
    instruction_[0x0D] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_ORA};
    instruction_[0x05] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_ORA};
    instruction_[0x01] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_ORA};
    instruction_[0x11] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_ORA};
    instruction_[0x15] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_ORA};
    instruction_[0x1D] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_ORA};
    instruction_[0x19] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_ORA};

    instruction_[0x48] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_PHA};

    instruction_[0x08] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_PHP};

    instruction_[0x68] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_PLA};

    instruction_[0x28] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_PLP};

    instruction_[0x2E] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_ROL};
    instruction_[0x26] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_ROL};
    instruction_[0x2A] = {&cpu_mos6502::Addr_ACC, &cpu_mos6502::Op_ROL_ACC};
    instruction_[0x36] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_ROL};
    instruction_[0x3E] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_ROL};

    instruction_[0x6E] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_ROR};
    instruction_[0x66] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_ROR};
    instruction_[0x6A] = {&cpu_mos6502::Addr_ACC, &cpu_mos6502::Op_ROR_ACC};
    instruction_[0x76] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_ROR};
    instruction_[0x7E] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_ROR};

    instruction_[0x40] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_RTI};

    instruction_[0x60] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_RTS};

    instruction_[0xE9] = {&cpu_mos6502::Addr_IMM, &cpu_mos6502::Op_SBC};
    instruction_[0xED] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_SBC};
    instruction_[0xE5] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_SBC};
    instruction_[0xE1] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_SBC};
    instruction_[0xF1] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_SBC};
    instruction_[0xF5] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_SBC};
    instruction_[0xFD] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_SBC};
    instruction_[0xF9] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_SBC};

    instruction_[0x38] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_SEC};

    instruction_[0xF8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_SED};

    instruction_[0x78] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_SEI};

    instruction_[0x8D] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_STA};
    instruction_[0x85] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_STA};
    instruction_[0x81] = {&cpu_mos6502::Addr_INX, &cpu_mos6502::Op_STA};
    instruction_[0x91] = {&cpu_mos6502::Addr_INY, &cpu_mos6502::Op_STA};
    instruction_[0x95] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_STA};
    instruction_[0x9D] = {&cpu_mos6502::Addr_ABX, &cpu_mos6502::Op_STA};
    instruction_[0x99] = {&cpu_mos6502::Addr_ABY, &cpu_mos6502::Op_STA};

    instruction_[0x8E] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_STX};
    instruction_[0x86] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_STX};
    instruction_[0x96] = {&cpu_mos6502::Addr_ZEY, &cpu_mos6502::Op_STX};

    instruction_[0x8C] = {&cpu_mos6502::Addr_ABS, &cpu_mos6502::Op_STY};
    instruction_[0x84] = {&cpu_mos6502::Addr_ZER, &cpu_mos6502::Op_STY};
    instruction_[0x94] = {&cpu_mos6502::Addr_ZEX, &cpu_mos6502::Op_STY};

    instruction_[0xAA] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TAX};

    instruction_[0xA8] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TAY};

    instruction_[0xBA] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TSX};

    instruction_[0x8A] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TXA};

    instruction_[0x9A] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TXS};

    instruction_[0x98] = {&cpu_mos6502::Addr_IMP, &cpu_mos6502::Op_TYA};
}

void cpu_mos6502::Op_ADC(uint16_t src)
{
    const auto m = bus_read_func_(src);
    unsigned int tmp = m + A + (status::is_carry_flag_set(status) ? 1 : 0);
    status::set_zero(status, !(tmp & 0xFF));
    if (status::is_decimal_flag_set(status))
    {
        if (((A & 0xF) + (m & 0xF) + (status::is_carry_flag_set(status) ? 1 : 0)) > 9)
            tmp += 6;
        status::set_negative(status, tmp & 0x80);
        status::set_overflow(status, !((A ^ m) & 0x80) && ((A ^ tmp) & 0x80));
        if (tmp > 0x99)
        {
            tmp += 96;
        }
        status::set_carry(status, tmp > 0x99);
    }
    else
    {
        status::set_negative(status, tmp & 0x80);
        status::set_overflow(status, !((A ^ m) & 0x80) && ((A ^ tmp) & 0x80));
        status::set_carry(status, tmp > 0xFF);
    }

    A = tmp & 0xFF;
}

void cpu_mos6502::Op_AND(uint16_t src)
{
    const auto m = bus_read_func_(src);
    const std::uint8_t res = m & A;
    status::set_negative(status, res & 0x80);
    status::set_zero(status, !res);
    A = res;
}

void cpu_mos6502::Op_ASL(uint16_t src)
{
    auto m = bus_read_func_(src);
    status::set_carry(status, m & 0x80);
    m <<= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write_func_(src, m);
}

void cpu_mos6502::Op_ASL_ACC(uint16_t src)
{
    auto m = A;
    status::set_carry(status, m & 0x80);
    m <<= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_BCC(uint16_t src)
{
    if (!status::is_carry_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BCS(uint16_t src)
{
    if (status::is_carry_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BEQ(uint16_t src)
{
    if (status::is_zero_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BIT(uint16_t src)
{
    const auto m = bus_read_func_(src);
    const std::uint8_t res = m & A;
    status::set_negative(status, res & 0x80);
    status = (status & 0x3F) | static_cast<uint8_t>(m & 0xC0);
    status::set_zero(status, !res);
}

void cpu_mos6502::Op_BMI(uint16_t src)
{
    if (status::is_negative_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BNE(uint16_t src)
{
    if (!status::is_zero_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BPL(uint16_t src)
{
    if (!status::is_negative_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BRK(uint16_t src)
{
    pc++;
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    stack_push(status | status::break_flag);
    status::set_interrupt(status, 1);
    pc = (bus_read_func_(irqVectorH) << 8) + bus_read_func_(irqVectorL);
}

void cpu_mos6502::Op_BVC(uint16_t src)
{
    if (!status::is_overflow_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_BVS(uint16_t src)
{
    if (status::is_overflow_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::Op_CLC(uint16_t src)
{
    status::set_carry(status, 0);
}

void cpu_mos6502::Op_CLD(uint16_t src)
{
    status::set_decimal(status, 0);
}

void cpu_mos6502::Op_CLI(uint16_t src)
{
    status::set_interrupt(status, 0);
}

void cpu_mos6502::Op_CLV(uint16_t src)
{
    status::set_overflow(status, 0);
}

void cpu_mos6502::Op_CMP(uint16_t src)
{
    const unsigned int tmp = A - bus_read_func_(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::Op_CPX(uint16_t src)
{
    const unsigned int tmp = X - bus_read_func_(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::Op_CPY(uint16_t src)
{
    const unsigned int tmp = Y - bus_read_func_(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::Op_DEC(uint16_t src)
{
    auto m = bus_read_func_(src);
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write_func_(src, m);
}

void cpu_mos6502::Op_DEX(uint16_t src)
{
    auto m = X;
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::Op_DEY(uint16_t src)
{
    auto m = Y;
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::Op_EOR(uint16_t src)
{
    auto m = bus_read_func_(src);
    m = A ^ m;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_INC(uint16_t src)
{
    auto m = bus_read_func_(src);
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write_func_(src, m);
}

void cpu_mos6502::Op_INX(uint16_t src)
{
    auto m = X;
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::Op_INY(uint16_t src)
{
    auto m = Y;
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::Op_JMP(uint16_t src)
{
    pc = src;
}

void cpu_mos6502::Op_JSR(uint16_t src)
{
    pc--;
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    pc = src;
}

void cpu_mos6502::Op_LDA(uint16_t src)
{
    const auto m = bus_read_func_(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_LDX(uint16_t src)
{
    const auto m = bus_read_func_(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::Op_LDY(uint16_t src)
{
    const auto m = bus_read_func_(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::Op_LSR(uint16_t src)
{
    auto m = bus_read_func_(src);
    status::set_carry(status, m & 0x01);
    m >>= 1;
    status::set_negative(status, 0);
    status::set_zero(status, !m);
    bus_write_func_(src, m);
}

void cpu_mos6502::Op_LSR_ACC(uint16_t src)
{
    auto m = A;
    status::set_carry(status, m & 0x01);
    m >>= 1;
    status::set_negative(status, 0);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_NOP(uint16_t src)
{
}

void cpu_mos6502::Op_ORA(uint16_t src)
{
    auto m = bus_read_func_(src);
    m = A | m;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_PHA(uint16_t src)
{
    stack_push(A);
}

void cpu_mos6502::Op_PHP(uint16_t src)
{
    stack_push(status | status::break_flag);
}

void cpu_mos6502::Op_PLA(uint16_t src)
{
    A = stack_pop();
    status::set_negative(status, A & 0x80);
    status::set_zero(status, !A);
}

void cpu_mos6502::Op_PLP(uint16_t src)
{
    status = stack_pop();
    status::set_constant(status, 1);
}

void cpu_mos6502::Op_ROL(uint16_t src)
{
    std::uint16_t m = bus_read_func_(src);
    m <<= 1;
    if (status::is_carry_flag_set(status))
        m |= 0x01;
    status::set_carry(status, m > 0xFF);
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write_func_(src, static_cast<std::uint8_t>(m));
}

void cpu_mos6502::Op_ROL_ACC(uint16_t src)
{
    std::uint16_t m = A;
    m <<= 1;
    if (status::is_carry_flag_set(status))
        m |= 0x01;
    status::set_carry(status, m > 0xFF);
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = static_cast<std::uint8_t>(m);
}

void cpu_mos6502::Op_ROR(uint16_t src)
{
    std::uint16_t m = bus_read_func_(src);
    if (status::is_carry_flag_set(status))
        m |= 0x100;
    status::set_carry(status, m & 0x01);
    m >>= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write_func_(src, static_cast<std::uint8_t>(m));
}

void cpu_mos6502::Op_ROR_ACC(uint16_t src)
{
    std::uint16_t m = A;
    if (status::is_carry_flag_set(status))
        m |= 0x100;
    status::set_carry(status, m & 0x01);
    m >>= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = static_cast<std::uint8_t>(m);
}

void cpu_mos6502::Op_RTI(uint16_t src)
{
    status = stack_pop();
    const auto lo = stack_pop();
    const auto hi = stack_pop();
    pc = (hi << 8) | lo;
}

void cpu_mos6502::Op_RTS(uint16_t src)
{
    const auto lo = stack_pop();
    const auto hi = stack_pop();
    pc = ((hi << 8) | lo) + 1;
}

void cpu_mos6502::Op_SBC(uint16_t src)
{
    const auto m = bus_read_func_(src);
    unsigned int tmp = A - m - (status::is_carry_flag_set(status) ? 0 : 1);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
    status::set_overflow(status, ((A ^ tmp) & 0x80) && ((A ^ m) & 0x80));

    if (status::is_decimal_flag_set(status))
    {
        if (((A & 0x0F) - (status::is_carry_flag_set(status) ? 0 : 1)) < (m & 0x0F))
            tmp -= 6;
        if (tmp > 0x99)
        {
            tmp -= 0x60;
        }
    }
    status::set_carry(status, tmp < 0x100);
    A = (tmp & 0xFF);
}

void cpu_mos6502::Op_SEC(uint16_t src)
{
    status::set_carry(status, 1);
}

void cpu_mos6502::Op_SED(uint16_t src)
{
    status::set_decimal(status, 1);
}

void cpu_mos6502::Op_SEI(uint16_t src)
{
    status::set_interrupt(status, 1);
}

void cpu_mos6502::Op_STA(uint16_t src)
{
    bus_write_func_(src, A);
}

void cpu_mos6502::Op_STX(uint16_t src)
{
    bus_write_func_(src, X);
}

void cpu_mos6502::Op_STY(uint16_t src)
{
    bus_write_func_(src, Y);
}

void cpu_mos6502::Op_TAX(uint16_t src)
{
    const auto m = A;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::Op_TAY(uint16_t src)
{
    const auto m = A;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::Op_TSX(uint16_t src)
{
    const auto m = sp;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::Op_TXA(uint16_t src)
{
    const auto m = X;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::Op_TXS(uint16_t src)
{
    sp = X;
}

void cpu_mos6502::Op_TYA(uint16_t src)
{
    const auto m = Y;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

} // namespace mos6502
