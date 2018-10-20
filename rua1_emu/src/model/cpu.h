#pragma once

#include <view/imain_window.h>
#include <view/frmcpu.h>
#include <view/sidebar_toggle_button.h>
#include <mos6502/bus.h>
#include <mos6502/cpu_mos6502.h>

namespace rua1::model
{

class cpu final
{
public:
    explicit cpu(view::imain_window &main_window, mos6502::bus &bus);
    ~cpu();

    cpu(cpu &&) noexcept = delete;
    auto operator=(cpu &&) noexcept -> cpu & = delete;

    cpu(const cpu &) noexcept = delete;
    auto operator=(const cpu &) noexcept -> cpu & = delete;

private:
    void create_view();
    void destroy_view();
    void on_sidebar_button_toggled(const bool checked);
    void on_view_closed() const;

    view::imain_window &main_window_;
    mos6502::cpu_mos6502 cpu_;
    view::frmcpu *view_;
    view::sidebar_toggle_button *sidebar_button_;
};

} // namespace rua1::model
