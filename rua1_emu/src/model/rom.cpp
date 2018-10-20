#include <model/rom.h>
#include <aeon/streams/file_stream.h>

namespace rua1::model
{

rom::rom(view::imain_window &main_window, const config::rom_device_config &config)
    : rom_{static_cast<std::uint16_t>(config.offset()), static_cast<std::uint16_t>(config.size())}
{
    aeon::streams::file_stream file{config.file()};
    rom_.load(file, 0);
}

rom::~rom() = default;

auto rom::get_device() noexcept -> mos6502::ibus_device &
{
    return rom_;
}

} // namespace rua1::model
