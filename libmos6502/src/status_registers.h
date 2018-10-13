#pragma once

namespace mos6502::status
{

constexpr auto negative_flag = 0x80;
constexpr auto overflow_flag = 0x40;
constexpr auto constant_flag = 0x20;
constexpr auto break_flag = 0x10;
constexpr auto decimal_flag = 0x08;
constexpr auto interrupt_flag = 0x04;
constexpr auto zero_flag = 0x02;
constexpr auto carry_flag = 0x01;

template <typename T>
constexpr auto set_negative(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= negative_flag) : (status &= (~negative_flag)));
}

template <typename T>
constexpr auto set_overflow(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= overflow_flag) : (status &= (~overflow_flag)));
}

template <typename T>
constexpr auto set_constant(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= constant_flag) : (status &= (~constant_flag)));
}

template <typename T>
constexpr auto set_break(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= break_flag) : (status &= (~break_flag)));
}

template <typename T>
constexpr auto set_decimal(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= decimal_flag) : (status &= (~decimal_flag)));
}

template <typename T>
constexpr auto set_interrupt(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= interrupt_flag) : (status &= (~interrupt_flag)));
}

template <typename T>
constexpr auto set_zero(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= zero_flag) : (status &= (~zero_flag)));
}

template <typename T>
constexpr auto set_carry(std::uint8_t &status, const T x) noexcept
{
    return (x ? (status |= carry_flag) : (status &= (~carry_flag)));
}

constexpr auto is_negative_flag_set(const std::uint8_t status) noexcept
{
    return ((status & negative_flag) ? true : false);
}

constexpr auto is_overflow_flag_set(const std::uint8_t status) noexcept
{
    return ((status & overflow_flag) ? true : false);
}

constexpr auto is_constant_flag_set(const std::uint8_t status) noexcept
{
    return ((status & constant_flag) ? true : false);
}

constexpr auto is_break_flag_set(const std::uint8_t status) noexcept
{
    return ((status & break_flag) ? true : false);
}

constexpr auto is_decimal_flag_set(const std::uint8_t status) noexcept
{
    return ((status & decimal_flag) ? true : false);
}

constexpr auto is_interrupt_flag_set(const std::uint8_t status) noexcept
{
    return ((status & interrupt_flag) ? true : false);
}

constexpr auto is_zero_flag_set(const std::uint8_t status) noexcept
{
    return ((status & zero_flag) ? true : false);
}

constexpr auto is_carry_flag_set(const std::uint8_t status) noexcept
{
    return ((status & carry_flag) ? true : false);
}

} // namespace mos6502::status
