#include <model/ram.h>
#include <aeon/streams/file_stream.h>

namespace rua1::model
{

ram::ram(view::imain_window &main_window, const config::ram_device_config &config)
    : sidebar_toggleable<view::frmmemory>{config.name(), false, main_window}
    , ram_{static_cast<std::uint16_t>(config.offset()), static_cast<std::uint16_t>(config.size())}
{
}

ram::~ram() = default;

auto ram::get_device() noexcept -> mos6502::ibus_device &
{
    return ram_;
}

} // namespace rua1::model
