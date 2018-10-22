#pragma once

#include <emu6502/memory.h>

namespace emu6502
{

class ram final : public memory
{
public:
    explicit ram(const std::uint16_t offset, const std::uint16_t size);
    virtual ~ram() = default;

    ram(ram &&) noexcept = delete;
    auto operator=(ram &&) noexcept -> ram & = delete;

    ram(const ram &) noexcept = delete;
    auto operator=(const ram &) noexcept -> ram & = delete;
};

} // namespace emu6502
