#pragma once

#include <model/sidebar_toggleable.h>
#include <view/frmcpu.h>
#include <mos6502/bus.h>
#include <mos6502/cpu_mos6502.h>

namespace rua1::model
{

class cpu final : public sidebar_toggleable<view::frmcpu>
{
public:
    explicit cpu(view::imain_window &main_window, mos6502::bus &bus);
    ~cpu();

    cpu(cpu &&) noexcept = delete;
    auto operator=(cpu &&) noexcept -> cpu & = delete;

    cpu(const cpu &) noexcept = delete;
    auto operator=(const cpu &) noexcept -> cpu & = delete;

private:
    mos6502::cpu_mos6502 cpu_;
};

} // namespace rua1::model
