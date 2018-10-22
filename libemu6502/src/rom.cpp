#include <emu6502/rom.h>
#include <cassert>

namespace emu6502
{

rom::rom(const std::uint16_t offset, const std::uint16_t size)
    : memory{offset, size}
{
}

void rom::write(const std::uint16_t address, const std::uint8_t value) noexcept
{
    // ROM can not be written to.
}

} // namespace emu6502
