#pragma once

#include <aeon/common/span.h>
#include <vector>

namespace disasm6502
{

class disassembled_instruction
{
public:
    explicit disassembled_instruction(const std::uint16_t address, std::string disassembly,
                                      const aeon::common::span<std::uint8_t> bytes)
        : address_{address}
        , disassembly_{std::move(disassembly)}
        , bytes_{bytes}
    {
    }

    ~disassembled_instruction() = default;

    disassembled_instruction(disassembled_instruction &&) noexcept = default;
    auto operator=(disassembled_instruction &&) noexcept -> disassembled_instruction & = default;

    disassembled_instruction(const disassembled_instruction &) noexcept = delete;
    auto operator=(const disassembled_instruction &) noexcept -> disassembled_instruction & = delete;

    auto address() const noexcept
    {
        return address_;
    }

    const auto &disassembly() const noexcept
    {
        return disassembly_;
    }

    auto bytes() const noexcept
    {
        return bytes_;
    }

private:
    std::uint16_t address_;
    std::string disassembly_;
    aeon::common::span<std::uint8_t> bytes_;
};

void initialize();

auto disassemble(const aeon::common::span<std::uint8_t> bytes, const std::uint16_t offset = 0)
    -> std::vector<disassembled_instruction>;

} // namespace disasm6502
