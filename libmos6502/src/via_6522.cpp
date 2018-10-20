#include <mos6502/via_6522.h>

namespace mos6502
{

via_6522::via_6522(const via_6522_settings settings) noexcept
    : iorb_register_{settings.iorb_register_address_}
    , iora_register_{settings.iora_register_address_}
    , ddrb_register_{settings.ddrb_register_address_}
    , ddra_register_{settings.ddra_register_address_}
    , t1cl_register_{settings.t1cl_register_address_}
    , t1ch_register_{settings.t1ch_register_address_}
    , t1ll_register_{settings.t1ll_register_address_}
    , t1lh_register_{settings.t1lh_register_address_}
    , t2cl_register_{settings.t2cl_register_address_}
    , t2ch_register_{settings.t2ch_register_address_}
    , sr_register_{settings.sr_register_address_}
    , acr_register_{settings.acr_register_address_}
    , pcr_register_{settings.pcr_register_address_}
    , ifr_register_{settings.ifr_register_address_}
    , ier_register_{settings.ier_register_address_}
    , iora_no_handshake_register_{settings.iora_no_handshake_register_address_}
{
}

void via_6522::write(const std::uint16_t address, const std::uint8_t value) noexcept
{
}

auto via_6522::read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t>
{
    return {false, static_cast<std::uint8_t>(0)};
}

} // namespace mos6502
