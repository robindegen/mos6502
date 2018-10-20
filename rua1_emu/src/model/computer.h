#pragma once

#include <model/cpu.h>
#include <model/component.h>
#include <view/imain_window.h>
#include <configuration.h>
#include <mos6502/bus.h>
#include <vector>
#include <memory>

namespace rua1::model
{

class computer final
{
public:
    explicit computer(view::imain_window &main_window, std::filesystem::path path);
    ~computer();

    computer(computer &&) noexcept = delete;
    auto operator=(computer &&) noexcept -> computer & = delete;

    computer(const computer &) noexcept = delete;
    auto operator=(const computer &) noexcept -> computer & = delete;

private:
    view::imain_window &main_window_;
    config::configuration config_;
    mos6502::bus bus_;
    cpu cpu_;

    std::vector<std::unique_ptr<component>> components_;
};

} // namespace rua1::model
