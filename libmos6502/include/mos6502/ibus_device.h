#pragma once

#include <cstdint>
#include <tuple>

namespace mos6502
{

class ibus_device
{
public:
    ibus_device(ibus_device &&) noexcept = delete;
    auto operator=(ibus_device &&) noexcept -> ibus_device & = delete;

    ibus_device(const ibus_device &) noexcept = delete;
    auto operator=(const ibus_device &) noexcept -> ibus_device & = delete;

    virtual void write(const std::uint16_t address, const std::uint8_t value) noexcept = 0;
    virtual auto read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t> = 0;

protected:
    ibus_device() = default;
    ~ibus_device() = default;
};

} // namespace mos6502
