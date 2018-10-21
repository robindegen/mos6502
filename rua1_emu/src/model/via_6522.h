#pragma once

#include <model/component.h>
#include <model/sidebar_toggleable.h>
#include <view/imain_window.h>
#include <view/frmvia.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <mos6502/via_6522.h>

namespace rua1::model
{

class via_6522 final : public component, public sidebar_toggleable<view::frmvia>
{
public:
    explicit via_6522(view::imain_window &main_window, const config::via_6522_device_config &config);
    ~via_6522();

    via_6522(via_6522 &&) noexcept = delete;
    auto operator=(via_6522 &&) noexcept -> via_6522 & = delete;

    via_6522(const via_6522 &) noexcept = delete;
    auto operator=(const via_6522 &) noexcept -> via_6522 & = delete;

    auto get_device() noexcept -> mos6502::ibus_device & override;

private:
    mos6502::via_6522 via_;
};

} // namespace rua1::model
