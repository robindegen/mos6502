#pragma once

#include <model/component.h>
#include <model/sidebar_toggleable.h>
#include <view/imain_window.h>
#include <view/frmacia.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <mos6502/acia_6551.h>

namespace rua1::model
{

class acia_6551 final : public component,
                        public sidebar_toggleable<view::frmacia, view::frmacia_model_interface>,
                        public view::frmacia_model_interface
{
public:
    explicit acia_6551(view::imain_window &main_window, const config::acia_6551_device_config &config);
    ~acia_6551();

    acia_6551(acia_6551 &&) noexcept = delete;
    auto operator=(acia_6551 &&) noexcept -> acia_6551 & = delete;

    acia_6551(const acia_6551 &) noexcept = delete;
    auto operator=(const acia_6551 &) noexcept -> acia_6551 & = delete;

    auto get_device() noexcept -> mos6502::ibus_device & override;

private:
    void on_view_created() override;
    void on_view_destroyed() override;

    mos6502::acia_6551 acia_;
};

} // namespace rua1::model
