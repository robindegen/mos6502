#pragma once

#include <aeon/common/span.h>
#include <vector>

namespace disasm6502
{

struct disassembled_instruction
{
    std::string disassembly{};
    aeon::common::span<std::uint8_t> bytes{};
};

void initialize();

auto disassemble(const aeon::common::span<std::uint8_t> bytes) -> std::vector<disassembled_instruction>;

} // namespace disasm6502
