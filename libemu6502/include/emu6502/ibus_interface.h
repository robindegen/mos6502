#pragma once

namespace emu6502
{

class ibus_interface
{
public:
    ibus_interface(ibus_interface &&) noexcept = delete;
    auto operator=(ibus_interface &&) noexcept -> ibus_interface & = delete;

    ibus_interface(const ibus_interface &) noexcept = delete;
    auto operator=(const ibus_interface &) noexcept -> ibus_interface & = delete;

    virtual void on_irq() noexcept = 0;

protected:
    ibus_interface() = default;
    ~ibus_interface() = default;
};

} // namespace emu6502
