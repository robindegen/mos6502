#include <mos6502/ram.h>
#include <cassert>

namespace mos6502
{

ram::ram(const std::uint16_t offset, const std::uint16_t size)
    : memory{offset, size}
{
}

} // namespace mos6502
