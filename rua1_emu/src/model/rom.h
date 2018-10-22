#pragma once

#include <model/component.h>
#include <model/sidebar_toggleable.h>
#include <view/imain_window.h>
#include <view/frmmemory.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <mos6502/rom.h>

namespace rua1::model
{

class rom final : public component,
                  public sidebar_toggleable<view::frmmemory, view::frmmemory_model_interface>,
                  public view::frmmemory_model_interface
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
    void on_view_created() override;
    void on_view_destroyed() override;

    void on_ui_btn_hex_view_clicked() override;

    mos6502::rom rom_;
};

} // namespace rua1::model
