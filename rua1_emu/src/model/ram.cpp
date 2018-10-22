#include <model/ram.h>
#include <aeon/streams/file_stream.h>

namespace rua1::model
{

ram::ram(view::imain_window &main_window, const config::ram_device_config &config)
    : sidebar_toggleable<view::frmmemory, view::frmmemory_model_interface>{*this, config.name(), main_window}
    , ram_{static_cast<std::uint16_t>(config.offset()), static_cast<std::uint16_t>(config.size())}
{
}

ram::~ram() = default;

auto ram::get_device() noexcept -> emu6502::ibus_device &
{
    return ram_;
}

void ram::on_view_created()
{
}

void ram::on_view_destroyed()
{
}

} // namespace rua1::model
