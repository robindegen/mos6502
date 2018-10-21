#pragma once

#include <model/component.h>
#include <model/sidebar_toggleable.h>
#include <view/imain_window.h>
#include <view/frmmemory.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <mos6502/ram.h>

namespace rua1::model
{

class ram final : public component, public sidebar_toggleable<view::frmmemory>
{
public:
    explicit ram(view::imain_window &main_window, const config::ram_device_config &config);
    ~ram();

    ram(ram &&) noexcept = delete;
    auto operator=(ram &&) noexcept -> ram & = delete;

    ram(const ram &) noexcept = delete;
    auto operator=(const ram &) noexcept -> ram & = delete;

    auto get_device() noexcept -> mos6502::ibus_device & override;

private:
    void on_view_created() override;
    void on_view_destroyed() override;

    mos6502::ram ram_;
};

} // namespace rua1::model
