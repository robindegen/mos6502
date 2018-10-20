#pragma once

#include <mos6502/memory.h>

namespace mos6502
{

class rom final : public memory
{
public:
    explicit rom(const std::uint16_t offset, const std::uint16_t size);
    virtual ~rom() = default;

    rom(rom &&) noexcept = delete;
    auto operator=(rom &&) noexcept -> rom & = delete;

    rom(const rom &) noexcept = delete;
    auto operator=(const rom &) noexcept -> rom & = delete;

private:
    void write(const std::uint16_t address, const std::uint8_t value) noexcept override;
};

} // namespace mos6502
