#pragma once

#include <aeon/common/bitflags.h>
#include <cstdint>

namespace mos6502
{

class ic_register
{
public:
    explicit ic_register(const std::uint16_t address, const std::uint8_t default_value = 0) noexcept
        : address_{address}
        , value_{default_value}
    {
    }

    ic_register(ic_register &&) noexcept = default;
    auto operator=(ic_register &&) noexcept -> ic_register & = default;

    ic_register(const ic_register &) noexcept = default;
    auto operator=(const ic_register &) noexcept -> ic_register & = default;

    ~ic_register() = default;

    auto address() const noexcept
    {
        return address_;
    }

    auto get() const noexcept
    {
        return value_;
    }

    auto get_low_nibble() const noexcept
    {
        return aeon::common::get_low_nibble(value_);
    }

    auto get_high_nibble() const noexcept
    {
        return aeon::common::get_high_nibble(value_);
    }

    auto get_bit_range(const int offset, const int length) const noexcept
    {
        return aeon::common::get_bit_range(value_, offset, length);
    }

    void set(const std::uint8_t value) noexcept
    {
        value_ = value;
    }

    void reset() noexcept
    {
        value_ = 0;
    }

    void set_bit_flags(const std::uint8_t flags) noexcept
    {
        value_ |= flags;
    }

    void clear_bit_flags(const std::uint8_t flags) noexcept
    {
        value_ &= ~flags;
    }

    auto check_bit_flags(const std::uint8_t flags) const noexcept
    {
        return aeon::common::check_bit_flag(value_, flags);
    }

    void mask_bit_flags(const std::uint8_t flags) noexcept
    {
        value_ &= flags;
    }

    auto &operator=(const std::uint8_t value) noexcept
    {
        set(value);
        return *this;
    }

private:
    std::uint16_t address_;
    std::uint8_t value_;
};

} // namespace mos6502
