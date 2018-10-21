#include <model/via_6522.h>
#include <aeon/streams/file_stream.h>

namespace rua1::model
{

via_6522::via_6522(view::imain_window &main_window, const config::via_6522_device_config &config)
    : sidebar_toggleable<view::frmvia, view::frmvia_model_interface>{*this, config.name(), main_window}
    , via_{{static_cast<std::uint16_t>(config.iorb_register()), static_cast<std::uint16_t>(config.iora_register()),
            static_cast<std::uint16_t>(config.ddrb_register()), static_cast<std::uint16_t>(config.ddra_register()),
            static_cast<std::uint16_t>(config.t1cl_register()), static_cast<std::uint16_t>(config.t1ch_register()),
            static_cast<std::uint16_t>(config.t1ll_register()), static_cast<std::uint16_t>(config.t1lh_register()),
            static_cast<std::uint16_t>(config.t2cl_register()), static_cast<std::uint16_t>(config.t2ch_register()),
            static_cast<std::uint16_t>(config.sr_register()), static_cast<std::uint16_t>(config.acr_register()),
            static_cast<std::uint16_t>(config.pcr_register()), static_cast<std::uint16_t>(config.ifr_register()),
            static_cast<std::uint16_t>(config.ier_register()),
            static_cast<std::uint16_t>(config.iora_no_hs_register())}}
{
}

via_6522::~via_6522() = default;

auto via_6522::get_device() noexcept -> mos6502::ibus_device &
{
    return via_;
}

void via_6522::on_view_created()
{
}

void via_6522::on_view_destroyed()
{
}

} // namespace rua1::model
