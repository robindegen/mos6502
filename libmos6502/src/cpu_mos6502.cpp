#include <mos6502/cpu_mos6502.h>
#include <status_registers.h>
#include <functional>

namespace mos6502
{

// IRQ, reset, NMI vectors
static constexpr std::uint16_t irq_vector_h = 0xFFFF;
static constexpr std::uint16_t irq_vector_l = 0xFFFE;
static constexpr std::uint16_t rst_vector_h = 0xFFFD;
static constexpr std::uint16_t rst_vector_l = 0xFFFC;
static constexpr std::uint16_t nmi_vector_h = 0xFFFB;
static constexpr std::uint16_t nmi_vector_l = 0xFFFA;

cpu_mos6502::cpu_mos6502(const bus_read_func read_func, const bus_write_func write_func)
    : instruction_{}
    , bus_read_func_{read_func}
    , bus_write_func_{write_func}
{
    initialize_illegal_opcodes();
    initialize_opcodes();
    reset();
}

void cpu_mos6502::nmi() noexcept
{
    status::set_break(status, 0);
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    stack_push(status);
    status::set_interrupt(status, 1);
    pc = (bus_read(nmi_vector_h) << 8) + bus_read(nmi_vector_l);
}

void cpu_mos6502::irq() noexcept
{
    if (!status::is_interrupt_flag_set(status))
    {
        status::set_break(status, 0);
        stack_push((pc >> 8) & 0xFF);
        stack_push(pc & 0xFF);
        stack_push(status);
        status::set_interrupt(status, 1);
        pc = (bus_read(irq_vector_h) << 8) + bus_read(irq_vector_l);
    }
}

void cpu_mos6502::reset() noexcept
{
    A = 0x00;
    Y = 0x00;
    X = 0x00;

    pc = (bus_read(rst_vector_h) << 8) + bus_read(rst_vector_l); // load PC from reset vector

    sp = 0xFD;

    status |= status::constant_flag;

    cycles_ = 6; // according to the datasheet, the reset routine takes 6 clock cycles

    illegal_opcode_ = false;
}

void cpu_mos6502::run(const std::uint32_t n) noexcept
{
    const auto start = cycles_;

    while (start + n > cycles_ && !illegal_opcode_)
    {
        // fetch
        const auto opcode = bus_read(pc++);

        // decode
        const auto instr = instruction_[opcode];

        // execute
        exec(instr);

        cycles_++;
    }
}

auto cpu_mos6502::is_illegal_opcode_set() const noexcept -> bool
{
    return illegal_opcode_;
}

void cpu_mos6502::exec(const instruction i) noexcept
{
    const auto src = std::invoke(i.addr, *this);
    std::invoke(i.code, *this, src);
}

void cpu_mos6502::stack_push(std::uint8_t byte) noexcept
{
    bus_write(0x0100 + sp, byte);

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

    return bus_read(0x0100 + sp);
}

void cpu_mos6502::bus_write(const std::uint16_t address, const std::uint8_t value) const noexcept
{
    bus_write_func_(address, value);
}

auto cpu_mos6502::bus_read(const std::uint16_t address) const noexcept -> std::uint8_t
{
    return bus_read_func_(address);
}

auto cpu_mos6502::addr_acc() noexcept -> std::uint16_t
{
    return 0; // not used
}

auto cpu_mos6502::addr_imm() noexcept -> std::uint16_t
{
    return pc++;
}

auto cpu_mos6502::addr_abs() noexcept -> std::uint16_t
{
    const std::uint16_t addr_l = bus_read(pc++);
    const std::uint16_t addr_h = bus_read(pc++);
    return addr_l + (addr_h << 8);
}

auto cpu_mos6502::addr_zer() noexcept -> std::uint16_t
{
    return bus_read(pc++);
}

auto cpu_mos6502::addr_imp() noexcept -> std::uint16_t
{
    return 0; // not used
}

auto cpu_mos6502::addr_rel() noexcept -> std::uint16_t
{
    auto offset = static_cast<uint16_t>(bus_read(pc++));

    if (offset & 0x80)
        offset |= 0xFF00;

    return pc + static_cast<int16_t>(offset);
}

auto cpu_mos6502::addr_abi() noexcept -> std::uint16_t
{
    const std::uint16_t addr_l = bus_read(pc++);
    const std::uint16_t addr_h = bus_read(pc++);

    const std::uint16_t abs = (addr_h << 8) | addr_l;

    const std::uint16_t eff_l = bus_read(abs);
    const std::uint16_t eff_h = bus_read((abs & 0xFF00) + ((abs + 1) & 0x00FF));

    return eff_l + 0x100 * eff_h;
}

auto cpu_mos6502::addr_zex() noexcept -> std::uint16_t
{
    return (bus_read(pc++) + X) % 256;
}

auto cpu_mos6502::addr_zey() noexcept -> std::uint16_t
{
    return (bus_read(pc++) + Y) % 256;
}

auto cpu_mos6502::addr_abx() noexcept -> std::uint16_t
{
    const std::uint16_t addr_l = bus_read(pc++);
    const std::uint16_t addr_h = bus_read(pc++);
    return addr_l + (addr_h << 8) + X;
}

auto cpu_mos6502::addr_aby() noexcept -> std::uint16_t
{
    const std::uint16_t addr_l = bus_read(pc++);
    const std::uint16_t addr_h = bus_read(pc++);
    return addr_l + (addr_h << 8) + Y;
}

auto cpu_mos6502::addr_inx() noexcept -> std::uint16_t
{
    const std::uint16_t zero_l = (bus_read(pc++) + X) % 256;
    const std::uint16_t zero_h = (zero_l + 1) % 256;
    return bus_read(zero_l) + (bus_read(zero_h) << 8);
}

auto cpu_mos6502::addr_iny() noexcept -> std::uint16_t
{
    const std::uint16_t zero_l = bus_read(pc++);
    const std::uint16_t zero_h = (zero_l + 1) % 256;
    return bus_read(zero_l) + (bus_read(zero_h) << 8) + Y;
}

void cpu_mos6502::op_adc(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
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

void cpu_mos6502::op_and(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
    const std::uint8_t res = m & A;
    status::set_negative(status, res & 0x80);
    status::set_zero(status, !res);
    A = res;
}

void cpu_mos6502::op_asl(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    status::set_carry(status, m & 0x80);
    m <<= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write(src, m);
}

void cpu_mos6502::op_asl_acc(std::uint16_t src) noexcept
{
    auto m = A;
    status::set_carry(status, m & 0x80);
    m <<= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_bcc(std::uint16_t src) noexcept
{
    if (!status::is_carry_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_bcs(std::uint16_t src) noexcept
{
    if (status::is_carry_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_beq(std::uint16_t src) noexcept
{
    if (status::is_zero_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_bit(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
    const std::uint8_t res = m & A;
    status::set_negative(status, res & 0x80);
    status = (status & 0x3F) | static_cast<uint8_t>(m & 0xC0);
    status::set_zero(status, !res);
}

void cpu_mos6502::op_bmi(std::uint16_t src) noexcept
{
    if (status::is_negative_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_bne(std::uint16_t src) noexcept
{
    if (!status::is_zero_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_bpl(std::uint16_t src) noexcept
{
    if (!status::is_negative_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_brk(std::uint16_t src) noexcept
{
    pc++;
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    stack_push(status | status::break_flag);
    status::set_interrupt(status, 1);
    pc = (bus_read(irq_vector_h) << 8) + bus_read(irq_vector_l);
}

void cpu_mos6502::op_bvc(std::uint16_t src) noexcept
{
    if (!status::is_overflow_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_bvs(std::uint16_t src) noexcept
{
    if (status::is_overflow_flag_set(status))
    {
        pc = src;
    }
}

void cpu_mos6502::op_clc(std::uint16_t src) noexcept
{
    status::set_carry(status, 0);
}

void cpu_mos6502::op_cld(std::uint16_t src) noexcept
{
    status::set_decimal(status, 0);
}

void cpu_mos6502::op_cli(std::uint16_t src) noexcept
{
    status::set_interrupt(status, 0);
}

void cpu_mos6502::op_clv(std::uint16_t src) noexcept
{
    status::set_overflow(status, 0);
}

void cpu_mos6502::op_cmp(std::uint16_t src) noexcept
{
    const unsigned int tmp = A - bus_read(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::op_cpx(std::uint16_t src) noexcept
{
    const unsigned int tmp = X - bus_read(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::op_cpy(std::uint16_t src) noexcept
{
    const unsigned int tmp = Y - bus_read(src);
    status::set_carry(status, tmp < 0x100);
    status::set_negative(status, tmp & 0x80);
    status::set_zero(status, !(tmp & 0xFF));
}

void cpu_mos6502::op_dec(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write(src, m);
}

void cpu_mos6502::op_dex(std::uint16_t src) noexcept
{
    auto m = X;
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::op_dey(std::uint16_t src) noexcept
{
    auto m = Y;
    m = (m - 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::op_eor(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    m = A ^ m;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_inc(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write(src, m);
}

void cpu_mos6502::op_inx(std::uint16_t src) noexcept
{
    auto m = X;
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::op_iny(std::uint16_t src) noexcept
{
    auto m = Y;
    m = (m + 1) % 256;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::op_jmp(std::uint16_t src) noexcept
{
    pc = src;
}

void cpu_mos6502::op_jsr(std::uint16_t src) noexcept
{
    pc--;
    stack_push((pc >> 8) & 0xFF);
    stack_push(pc & 0xFF);
    pc = src;
}

void cpu_mos6502::op_lda(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_ldx(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::op_ldy(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::op_lsr(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    status::set_carry(status, m & 0x01);
    m >>= 1;
    status::set_negative(status, 0);
    status::set_zero(status, !m);
    bus_write(src, m);
}

void cpu_mos6502::op_lsr_acc(std::uint16_t src) noexcept
{
    auto m = A;
    status::set_carry(status, m & 0x01);
    m >>= 1;
    status::set_negative(status, 0);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_nop(std::uint16_t src) noexcept
{
}

void cpu_mos6502::op_ora(std::uint16_t src) noexcept
{
    auto m = bus_read(src);
    m = A | m;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_pha(std::uint16_t src) noexcept
{
    stack_push(A);
}

void cpu_mos6502::op_php(std::uint16_t src) noexcept
{
    stack_push(status | status::break_flag);
}

void cpu_mos6502::op_pla(std::uint16_t src) noexcept
{
    A = stack_pop();
    status::set_negative(status, A & 0x80);
    status::set_zero(status, !A);
}

void cpu_mos6502::op_plp(std::uint16_t src) noexcept
{
    status = stack_pop();
    status::set_constant(status, 1);
}

void cpu_mos6502::op_rol(std::uint16_t src) noexcept
{
    std::uint16_t m = bus_read(src);
    m <<= 1;
    if (status::is_carry_flag_set(status))
        m |= 0x01;
    status::set_carry(status, m > 0xFF);
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write(src, static_cast<std::uint8_t>(m));
}

void cpu_mos6502::op_rol_acc(std::uint16_t src) noexcept
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

void cpu_mos6502::op_ror(std::uint16_t src) noexcept
{
    std::uint16_t m = bus_read(src);
    if (status::is_carry_flag_set(status))
        m |= 0x100;
    status::set_carry(status, m & 0x01);
    m >>= 1;
    m &= 0xFF;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    bus_write(src, static_cast<std::uint8_t>(m));
}

void cpu_mos6502::op_ror_acc(std::uint16_t src) noexcept
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

void cpu_mos6502::op_rti(std::uint16_t src) noexcept
{
    status = stack_pop();
    const auto lo = stack_pop();
    const auto hi = stack_pop();
    pc = (hi << 8) | lo;
}

void cpu_mos6502::op_rts(std::uint16_t src) noexcept
{
    const auto lo = stack_pop();
    const auto hi = stack_pop();
    pc = ((hi << 8) | lo) + 1;
}

void cpu_mos6502::op_sbc(std::uint16_t src) noexcept
{
    const auto m = bus_read(src);
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

void cpu_mos6502::op_sec(std::uint16_t src) noexcept
{
    status::set_carry(status, 1);
}

void cpu_mos6502::op_sed(std::uint16_t src) noexcept
{
    status::set_decimal(status, 1);
}

void cpu_mos6502::op_sei(std::uint16_t src) noexcept
{
    status::set_interrupt(status, 1);
}

void cpu_mos6502::op_sta(std::uint16_t src) noexcept
{
    bus_write(src, A);
}

void cpu_mos6502::op_stx(std::uint16_t src) noexcept
{
    bus_write(src, X);
}

void cpu_mos6502::op_sty(std::uint16_t src) noexcept
{
    bus_write(src, Y);
}

void cpu_mos6502::op_tax(std::uint16_t src) noexcept
{
    const auto m = A;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::op_tay(std::uint16_t src) noexcept
{
    const auto m = A;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    Y = m;
}

void cpu_mos6502::op_tsx(std::uint16_t src) noexcept
{
    const auto m = sp;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    X = m;
}

void cpu_mos6502::op_txa(std::uint16_t src) noexcept
{
    const auto m = X;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_txs(std::uint16_t src) noexcept
{
    sp = X;
}

void cpu_mos6502::op_tya(std::uint16_t src) noexcept
{
    const auto m = Y;
    status::set_negative(status, m & 0x80);
    status::set_zero(status, !m);
    A = m;
}

void cpu_mos6502::op_illegal(std::uint16_t src) noexcept
{
    illegal_opcode_ = true;
}

void cpu_mos6502::initialize_illegal_opcodes() noexcept
{
    for (auto &i : instruction_)
    {
        i = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_illegal};
    }
}

void cpu_mos6502::initialize_opcodes() noexcept
{
    instruction_[0x69] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_adc};
    instruction_[0x6D] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_adc};
    instruction_[0x65] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_adc};
    instruction_[0x61] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_adc};
    instruction_[0x71] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_adc};
    instruction_[0x75] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_adc};
    instruction_[0x7D] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_adc};
    instruction_[0x79] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_adc};

    instruction_[0x29] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_and};
    instruction_[0x2D] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_and};
    instruction_[0x25] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_and};
    instruction_[0x21] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_and};
    instruction_[0x31] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_and};
    instruction_[0x35] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_and};
    instruction_[0x3D] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_and};
    instruction_[0x39] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_and};

    instruction_[0x0E] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_asl};
    instruction_[0x06] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_asl};
    instruction_[0x0A] = {&cpu_mos6502::addr_acc, &cpu_mos6502::op_asl_acc};
    instruction_[0x16] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_asl};
    instruction_[0x1E] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_asl};

    instruction_[0x90] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bcc};

    instruction_[0xB0] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bcs};

    instruction_[0xF0] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_beq};

    instruction_[0x2C] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_bit};
    instruction_[0x24] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_bit};

    instruction_[0x30] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bmi};

    instruction_[0xD0] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bne};

    instruction_[0x10] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bpl};

    instruction_[0x00] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_brk};

    instruction_[0x50] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bvc};

    instruction_[0x70] = {&cpu_mos6502::addr_rel, &cpu_mos6502::op_bvs};

    instruction_[0x18] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_clc};

    instruction_[0xD8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_cld};

    instruction_[0x58] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_cli};

    instruction_[0xB8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_clv};

    instruction_[0xC9] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_cmp};
    instruction_[0xCD] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_cmp};
    instruction_[0xC5] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_cmp};
    instruction_[0xC1] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_cmp};
    instruction_[0xD1] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_cmp};
    instruction_[0xD5] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_cmp};
    instruction_[0xDD] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_cmp};
    instruction_[0xD9] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_cmp};

    instruction_[0xE0] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_cpx};
    instruction_[0xEC] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_cpx};
    instruction_[0xE4] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_cpx};

    instruction_[0xC0] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_cpy};
    instruction_[0xCC] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_cpy};
    instruction_[0xC4] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_cpy};

    instruction_[0xCE] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_dec};
    instruction_[0xC6] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_dec};
    instruction_[0xD6] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_dec};
    instruction_[0xDE] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_dec};

    instruction_[0xCA] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_dex};

    instruction_[0x88] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_dey};

    instruction_[0x49] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_eor};
    instruction_[0x4D] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_eor};
    instruction_[0x45] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_eor};
    instruction_[0x41] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_eor};
    instruction_[0x51] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_eor};
    instruction_[0x55] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_eor};
    instruction_[0x5D] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_eor};
    instruction_[0x59] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_eor};

    instruction_[0xEE] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_inc};
    instruction_[0xE6] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_inc};
    instruction_[0xF6] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_inc};
    instruction_[0xFE] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_inc};

    instruction_[0xE8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_inx};

    instruction_[0xC8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_iny};

    instruction_[0x4C] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_jmp};
    instruction_[0x6C] = {&cpu_mos6502::addr_abi, &cpu_mos6502::op_jmp};

    instruction_[0x20] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_jsr};

    instruction_[0xA9] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_lda};
    instruction_[0xAD] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_lda};
    instruction_[0xA5] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_lda};
    instruction_[0xA1] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_lda};
    instruction_[0xB1] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_lda};
    instruction_[0xB5] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_lda};
    instruction_[0xBD] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_lda};
    instruction_[0xB9] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_lda};

    instruction_[0xA2] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_ldx};
    instruction_[0xAE] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_ldx};
    instruction_[0xA6] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_ldx};
    instruction_[0xBE] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_ldx};
    instruction_[0xB6] = {&cpu_mos6502::addr_zey, &cpu_mos6502::op_ldx};

    instruction_[0xA0] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_ldy};
    instruction_[0xAC] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_ldy};
    instruction_[0xA4] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_ldy};
    instruction_[0xB4] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_ldy};
    instruction_[0xBC] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_ldy};

    instruction_[0x4E] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_lsr};
    instruction_[0x46] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_lsr};
    instruction_[0x4A] = {&cpu_mos6502::addr_acc, &cpu_mos6502::op_lsr_acc};
    instruction_[0x56] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_lsr};
    instruction_[0x5E] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_lsr};

    instruction_[0xEA] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_nop};

    instruction_[0x09] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_ora};
    instruction_[0x0D] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_ora};
    instruction_[0x05] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_ora};
    instruction_[0x01] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_ora};
    instruction_[0x11] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_ora};
    instruction_[0x15] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_ora};
    instruction_[0x1D] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_ora};
    instruction_[0x19] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_ora};

    instruction_[0x48] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_pha};

    instruction_[0x08] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_php};

    instruction_[0x68] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_pla};

    instruction_[0x28] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_plp};

    instruction_[0x2E] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_rol};
    instruction_[0x26] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_rol};
    instruction_[0x2A] = {&cpu_mos6502::addr_acc, &cpu_mos6502::op_rol_acc};
    instruction_[0x36] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_rol};
    instruction_[0x3E] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_rol};

    instruction_[0x6E] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_ror};
    instruction_[0x66] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_ror};
    instruction_[0x6A] = {&cpu_mos6502::addr_acc, &cpu_mos6502::op_ror_acc};
    instruction_[0x76] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_ror};
    instruction_[0x7E] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_ror};

    instruction_[0x40] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_rti};

    instruction_[0x60] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_rts};

    instruction_[0xE9] = {&cpu_mos6502::addr_imm, &cpu_mos6502::op_sbc};
    instruction_[0xED] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_sbc};
    instruction_[0xE5] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_sbc};
    instruction_[0xE1] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_sbc};
    instruction_[0xF1] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_sbc};
    instruction_[0xF5] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_sbc};
    instruction_[0xFD] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_sbc};
    instruction_[0xF9] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_sbc};

    instruction_[0x38] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_sec};

    instruction_[0xF8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_sed};

    instruction_[0x78] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_sei};

    instruction_[0x8D] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_sta};
    instruction_[0x85] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_sta};
    instruction_[0x81] = {&cpu_mos6502::addr_inx, &cpu_mos6502::op_sta};
    instruction_[0x91] = {&cpu_mos6502::addr_iny, &cpu_mos6502::op_sta};
    instruction_[0x95] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_sta};
    instruction_[0x9D] = {&cpu_mos6502::addr_abx, &cpu_mos6502::op_sta};
    instruction_[0x99] = {&cpu_mos6502::addr_aby, &cpu_mos6502::op_sta};

    instruction_[0x8E] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_stx};
    instruction_[0x86] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_stx};
    instruction_[0x96] = {&cpu_mos6502::addr_zey, &cpu_mos6502::op_stx};

    instruction_[0x8C] = {&cpu_mos6502::addr_abs, &cpu_mos6502::op_sty};
    instruction_[0x84] = {&cpu_mos6502::addr_zer, &cpu_mos6502::op_sty};
    instruction_[0x94] = {&cpu_mos6502::addr_zex, &cpu_mos6502::op_sty};

    instruction_[0xAA] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_tax};

    instruction_[0xA8] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_tay};

    instruction_[0xBA] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_tsx};

    instruction_[0x8A] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_txa};

    instruction_[0x9A] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_txs};

    instruction_[0x98] = {&cpu_mos6502::addr_imp, &cpu_mos6502::op_tya};
}

} // namespace mos6502
