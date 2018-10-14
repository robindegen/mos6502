#include <mos6502/cpu_mos6502.h>

void bus_write(const std::uint16_t, const std::uint8_t) noexcept
{
}

auto bus_read(const std::uint16_t) noexcept -> std::uint8_t
{
    return 0;
}

int main(int argc, char *argv[])
{
    mos6502::cpu_mos6502 cpu{bus_read, bus_write};
    return 0;
}
