#pragma once

#include <model/component.h>
#include <view/imain_window.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <mos6502/rom.h>

namespace rua1::model
{

class rom final : public component
{
public:
    explicit rom(view::imain_window &main_window, const config::rom_device_config &config);
    ~rom();

    rom(rom &&) noexcept = delete;
    auto operator=(rom &&) noexcept -> rom & = delete;

    rom(const rom &) noexcept = delete;
    auto operator=(const rom &) noexcept -> rom & = delete;

    auto get_device() noexcept -> mos6502::ibus_device & override;

private:
    mos6502::rom rom_;
};

} // namespace rua1::model
