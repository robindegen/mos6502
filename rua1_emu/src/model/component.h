#pragma once

#include <emu6502/ibus_device.h>

namespace rua1::model
{

class component
{
public:
    component() = default;
    virtual ~component() = default;

    component(component &&) noexcept = delete;
    auto operator=(component &&) noexcept -> component & = delete;

    component(const component &) noexcept = delete;
    auto operator=(const component &) noexcept -> component & = delete;

    virtual auto get_device() noexcept -> emu6502::ibus_device & = 0;
};

} // namespace rua1::model
