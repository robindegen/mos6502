#include <model/acia_6551.h>
#include <aeon/streams/file_stream.h>

namespace rua1::model
{

acia_6551::acia_6551(view::imain_window &main_window, const config::acia_6551_device_config &config)
    : sidebar_toggleable<view::frmacia, view::frmacia_model_interface>{*this, config.name(), main_window}
    , acia_{
          {static_cast<std::uint16_t>(config.send_recv_register()),
           static_cast<std::uint16_t>(config.status_register()), static_cast<std::uint16_t>(config.command_register()),
           static_cast<std::uint16_t>(config.control_register()), config.simulate_wdc_bugs(), [](const auto value) {}}}
{
}

acia_6551::~acia_6551() = default;

auto acia_6551::get_device() noexcept -> emu6502::ibus_device &
{
    return acia_;
}

void acia_6551::on_view_created()
{
}

void acia_6551::on_view_destroyed()
{
}

} // namespace rua1::model
