#include <disasm6502/disasm.h>
#include <aeon/common/string.h>
#include <array>
#include <optional>

namespace disasm6502
{

static auto address_mode_accumulator(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "A";
}

static auto address_mode_immediate(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "#$" + aeon::common::string::int_to_hex_string(*(++itr));
}

static auto address_mode_absolute(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    const auto part1 = aeon::common::string::int_to_hex_string(*(++itr));
    const auto part2 = aeon::common::string::int_to_hex_string(*(++itr));
    return "$" + part1 + part2;
}

static auto address_mode_zero_page(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$" + aeon::common::string::int_to_hex_string(*(++itr));
}

static auto address_mode_index_x_zero_page(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$" + aeon::common::string::int_to_hex_string(*(++itr)) + ",X";
}

static auto address_mode_index_y_zero_page(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$" + aeon::common::string::int_to_hex_string(*(++itr)) + ",Y";
}

static auto address_mode_index_x_absolute(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    const auto part1 = aeon::common::string::int_to_hex_string(*(++itr));
    const auto part2 = aeon::common::string::int_to_hex_string(*(++itr));
    return "$" + part1 + part2 + ",X";
}

static auto address_mode_index_y_absolute(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    const auto part1 = aeon::common::string::int_to_hex_string(*(++itr));
    const auto part2 = aeon::common::string::int_to_hex_string(*(++itr));
    return "$" + part1 + part2 + ",Y";
}

static auto address_mode_implied(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "";
}

static auto address_mode_relative(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$" + aeon::common::string::int_to_hex_string(*(++itr));
}

static auto address_mode_indexed_x_indirect(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$(" + aeon::common::string::int_to_hex_string(*(++itr)) + ",X)";
}

static auto address_mode_indexed_y_indirect(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    return "$(" + aeon::common::string::int_to_hex_string(*(++itr)) + "),Y";
}

static auto address_mode_absolute_indirect(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string
{
    const auto part1 = aeon::common::string::int_to_hex_string(*(++itr));
    const auto part2 = aeon::common::string::int_to_hex_string(*(++itr));
    return "($" + part1 + part2 + ")";
}

using addressing_mode_decode_func = auto (*)(aeon::common::span<std::uint8_t>::iterator &itr) -> std::string;

struct instruction
{
    addressing_mode_decode_func decode_func{};
    std::string opcode{};
};

static auto get_instruction_length(const instruction &i) noexcept -> int
{
    if (i.decode_func == address_mode_accumulator)
        return 1;
    if (i.decode_func == address_mode_immediate)
        return 2;
    if (i.decode_func == address_mode_absolute)
        return 3;
    if (i.decode_func == address_mode_zero_page)
        return 2;
    if (i.decode_func == address_mode_index_x_zero_page)
        return 2;
    if (i.decode_func == address_mode_index_y_zero_page)
        return 2;
    if (i.decode_func == address_mode_index_x_absolute)
        return 3;
    if (i.decode_func == address_mode_index_y_absolute)
        return 3;
    if (i.decode_func == address_mode_implied)
        return 1;
    if (i.decode_func == address_mode_relative)
        return 2;
    if (i.decode_func == address_mode_indexed_x_indirect)
        return 2;
    if (i.decode_func == address_mode_indexed_y_indirect)
        return 2;
    if (i.decode_func == address_mode_absolute_indirect)
        return 3;

    return 1; // Invalid/unknown opcode?
}

static std::array<instruction, 256> instruction{{}};

void initialize()
{
    instruction[0x69] = {address_mode_immediate, "adc"};
    instruction[0x6D] = {address_mode_absolute, "adc"};
    instruction[0x65] = {address_mode_zero_page, "adc"};
    instruction[0x61] = {address_mode_indexed_x_indirect, "adc"};
    instruction[0x71] = {address_mode_indexed_y_indirect, "adc"};
    instruction[0x75] = {address_mode_index_x_zero_page, "adc"};
    instruction[0x7D] = {address_mode_index_x_absolute, "adc"};
    instruction[0x79] = {address_mode_index_y_absolute, "adc"};

    instruction[0x29] = {address_mode_immediate, "and"};
    instruction[0x2D] = {address_mode_absolute, "and"};
    instruction[0x25] = {address_mode_zero_page, "and"};
    instruction[0x21] = {address_mode_indexed_x_indirect, "and"};
    instruction[0x31] = {address_mode_indexed_y_indirect, "and"};
    instruction[0x35] = {address_mode_index_x_zero_page, "and"};
    instruction[0x3D] = {address_mode_index_x_absolute, "and"};
    instruction[0x39] = {address_mode_index_y_absolute, "and"};

    instruction[0x0E] = {address_mode_absolute, "asl"};
    instruction[0x06] = {address_mode_zero_page, "asl"};
    instruction[0x0A] = {address_mode_accumulator, "asl"};
    instruction[0x16] = {address_mode_index_x_zero_page, "asl"};
    instruction[0x1E] = {address_mode_index_x_absolute, "asl"};

    instruction[0x90] = {address_mode_relative, "bcc"};

    instruction[0xB0] = {address_mode_relative, "bcs"};

    instruction[0xF0] = {address_mode_relative, "beq"};

    instruction[0x2C] = {address_mode_absolute, "bit"};
    instruction[0x24] = {address_mode_zero_page, "bit"};

    instruction[0x30] = {address_mode_relative, "bmi"};

    instruction[0xD0] = {address_mode_relative, "bne"};

    instruction[0x10] = {address_mode_relative, "bpl"};

    instruction[0x00] = {address_mode_implied, "brk"};

    instruction[0x50] = {address_mode_relative, "bvc"};

    instruction[0x70] = {address_mode_relative, "bvs"};

    instruction[0x18] = {address_mode_implied, "clc"};

    instruction[0xD8] = {address_mode_implied, "cld"};

    instruction[0x58] = {address_mode_implied, "cli"};

    instruction[0xB8] = {address_mode_implied, "clv"};

    instruction[0xC9] = {address_mode_immediate, "cmp"};
    instruction[0xCD] = {address_mode_absolute, "cmp"};
    instruction[0xC5] = {address_mode_zero_page, "cmp"};
    instruction[0xC1] = {address_mode_indexed_x_indirect, "cmp"};
    instruction[0xD1] = {address_mode_indexed_y_indirect, "cmp"};
    instruction[0xD5] = {address_mode_index_x_zero_page, "cmp"};
    instruction[0xDD] = {address_mode_index_x_absolute, "cmp"};
    instruction[0xD9] = {address_mode_index_y_absolute, "cmp"};

    instruction[0xE0] = {address_mode_immediate, "cpx"};
    instruction[0xEC] = {address_mode_absolute, "cpx"};
    instruction[0xE4] = {address_mode_zero_page, "cpx"};

    instruction[0xC0] = {address_mode_immediate, "cpy"};
    instruction[0xCC] = {address_mode_absolute, "cpy"};
    instruction[0xC4] = {address_mode_zero_page, "cpy"};

    instruction[0xCE] = {address_mode_absolute, "dec"};
    instruction[0xC6] = {address_mode_zero_page, "dec"};
    instruction[0xD6] = {address_mode_index_x_zero_page, "dec"};
    instruction[0xDE] = {address_mode_index_x_absolute, "dec"};

    instruction[0xCA] = {address_mode_implied, "dex"};

    instruction[0x88] = {address_mode_implied, "dey"};

    instruction[0x49] = {address_mode_immediate, "eor"};
    instruction[0x4D] = {address_mode_absolute, "eor"};
    instruction[0x45] = {address_mode_zero_page, "eor"};
    instruction[0x41] = {address_mode_indexed_x_indirect, "eor"};
    instruction[0x51] = {address_mode_indexed_y_indirect, "eor"};
    instruction[0x55] = {address_mode_index_x_zero_page, "eor"};
    instruction[0x5D] = {address_mode_index_x_absolute, "eor"};
    instruction[0x59] = {address_mode_index_y_absolute, "eor"};

    instruction[0xEE] = {address_mode_absolute, "inc"};
    instruction[0xE6] = {address_mode_zero_page, "inc"};
    instruction[0xF6] = {address_mode_index_x_zero_page, "inc"};
    instruction[0xFE] = {address_mode_index_x_absolute, "inc"};

    instruction[0xE8] = {address_mode_implied, "inx"};

    instruction[0xC8] = {address_mode_implied, "iny"};

    instruction[0x4C] = {address_mode_absolute, "jmp"};
    instruction[0x6C] = {address_mode_absolute_indirect, "jmp"};

    instruction[0x20] = {address_mode_absolute, "jsr"};

    instruction[0xA9] = {address_mode_immediate, "lda"};
    instruction[0xAD] = {address_mode_absolute, "lda"};
    instruction[0xA5] = {address_mode_zero_page, "lda"};
    instruction[0xA1] = {address_mode_indexed_x_indirect, "lda"};
    instruction[0xB1] = {address_mode_indexed_y_indirect, "lda"};
    instruction[0xB5] = {address_mode_index_x_zero_page, "lda"};
    instruction[0xBD] = {address_mode_index_x_absolute, "lda"};
    instruction[0xB9] = {address_mode_index_y_absolute, "lda"};

    instruction[0xA2] = {address_mode_immediate, "ldx"};
    instruction[0xAE] = {address_mode_absolute, "ldx"};
    instruction[0xA6] = {address_mode_zero_page, "ldx"};
    instruction[0xBE] = {address_mode_index_y_absolute, "ldx"};
    instruction[0xB6] = {address_mode_index_y_zero_page, "ldx"};

    instruction[0xA0] = {address_mode_immediate, "ldy"};
    instruction[0xAC] = {address_mode_absolute, "ldy"};
    instruction[0xA4] = {address_mode_zero_page, "ldy"};
    instruction[0xB4] = {address_mode_index_x_zero_page, "ldy"};
    instruction[0xBC] = {address_mode_index_x_absolute, "ldy"};

    instruction[0x4E] = {address_mode_absolute, "lsr"};
    instruction[0x46] = {address_mode_zero_page, "lsr"};
    instruction[0x4A] = {address_mode_accumulator, "lsr"};
    instruction[0x56] = {address_mode_index_x_zero_page, "lsr"};
    instruction[0x5E] = {address_mode_index_x_absolute, "lsr"};

    instruction[0xEA] = {address_mode_implied, "nop"};

    instruction[0x09] = {address_mode_immediate, "ora"};
    instruction[0x0D] = {address_mode_absolute, "ora"};
    instruction[0x05] = {address_mode_zero_page, "ora"};
    instruction[0x01] = {address_mode_indexed_x_indirect, "ora"};
    instruction[0x11] = {address_mode_indexed_y_indirect, "ora"};
    instruction[0x15] = {address_mode_index_x_zero_page, "ora"};
    instruction[0x1D] = {address_mode_index_x_absolute, "ora"};
    instruction[0x19] = {address_mode_index_y_absolute, "ora"};

    instruction[0x48] = {address_mode_implied, "pha"};

    instruction[0x08] = {address_mode_implied, "php"};

    instruction[0x68] = {address_mode_implied, "pla"};

    instruction[0x28] = {address_mode_implied, "plp"};

    instruction[0x2E] = {address_mode_absolute, "rol"};
    instruction[0x26] = {address_mode_zero_page, "rol"};
    instruction[0x2A] = {address_mode_accumulator, "rol"};
    instruction[0x36] = {address_mode_index_x_zero_page, "rol"};
    instruction[0x3E] = {address_mode_index_x_absolute, "rol"};

    instruction[0x6E] = {address_mode_absolute, "ror"};
    instruction[0x66] = {address_mode_zero_page, "ror"};
    instruction[0x6A] = {address_mode_accumulator, "ror"};
    instruction[0x76] = {address_mode_index_x_zero_page, "ror"};
    instruction[0x7E] = {address_mode_index_x_absolute, "ror"};

    instruction[0x40] = {address_mode_implied, "rti"};

    instruction[0x60] = {address_mode_implied, "rts"};

    instruction[0xE9] = {address_mode_immediate, "sbc"};
    instruction[0xED] = {address_mode_absolute, "sbc"};
    instruction[0xE5] = {address_mode_zero_page, "sbc"};
    instruction[0xE1] = {address_mode_indexed_x_indirect, "sbc"};
    instruction[0xF1] = {address_mode_indexed_y_indirect, "sbc"};
    instruction[0xF5] = {address_mode_index_x_zero_page, "sbc"};
    instruction[0xFD] = {address_mode_index_x_absolute, "sbc"};
    instruction[0xF9] = {address_mode_index_y_absolute, "sbc"};

    instruction[0x38] = {address_mode_implied, "sec"};

    instruction[0xF8] = {address_mode_implied, "sed"};

    instruction[0x78] = {address_mode_implied, "sei"};

    instruction[0x8D] = {address_mode_absolute, "sta"};
    instruction[0x85] = {address_mode_zero_page, "sta"};
    instruction[0x81] = {address_mode_indexed_x_indirect, "sta"};
    instruction[0x91] = {address_mode_indexed_y_indirect, "sta"};
    instruction[0x95] = {address_mode_index_x_zero_page, "sta"};
    instruction[0x9D] = {address_mode_index_x_absolute, "sta"};
    instruction[0x99] = {address_mode_index_y_absolute, "sta"};

    instruction[0x8E] = {address_mode_absolute, "stx"};
    instruction[0x86] = {address_mode_zero_page, "stx"};
    instruction[0x96] = {address_mode_index_y_zero_page, "stx"};

    instruction[0x8C] = {address_mode_absolute, "sty"};
    instruction[0x84] = {address_mode_zero_page, "sty"};
    instruction[0x94] = {address_mode_index_x_zero_page, "sty"};

    instruction[0xAA] = {address_mode_implied, "tax"};

    instruction[0xA8] = {address_mode_implied, "tay"};

    instruction[0xBA] = {address_mode_implied, "tsx"};

    instruction[0x8A] = {address_mode_implied, "txa"};

    instruction[0x9A] = {address_mode_implied, "txs"};

    instruction[0x98] = {address_mode_implied, "tya"};
}

auto disassemble(const aeon::common::span<std::uint8_t> bytes) -> std::vector<disassembled_instruction>
{
    std::vector<disassembled_instruction> disassembly;

    for (auto itr = std::begin(bytes); itr != std::end(bytes); ++itr)
    {
        const auto instruction_info = instruction[*itr];

        if (instruction_info.decode_func == nullptr)
        {
            disassembly.push_back({".db " + aeon::common::string::int_to_hex_string(*itr),
                                   aeon::common::span<std::uint8_t>{itr, itr + 1}});
            continue;
        }

        const auto length_remaining = std::distance(itr, std::end(bytes));
        const auto instruction_length = get_instruction_length(instruction_info);

        if (length_remaining < instruction_length)
            break;

        const auto first_itr = itr;
        const auto disassembly_str =
            aeon::common::string::trimmed(instruction_info.opcode + " " + instruction_info.decode_func(itr));
        disassembly.push_back({disassembly_str, aeon::common::span<std::uint8_t>{first_itr, itr + 1}});
    }

    return disassembly;
}

} // namespace disasm6502
