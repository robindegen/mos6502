#pragma once

#include <aeon/common/span.h>
#include <vector>

namespace disasm6502
{

struct disassembled_instruction
{
    std::uint16_t address{};
    std::string disassembly{};
    aeon::common::span<std::uint8_t> bytes{};
};

void initialize();

auto disassemble(const aeon::common::span<std::uint8_t> bytes, const std::uint16_t offset = 0) -> std::vector<disassembled_instruction>;

} // namespace disasm6502
