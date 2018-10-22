#pragma once

#include <emu6502/ibus_interface.h>
#include <cstdint>
#include <vector>

namespace emu6502
{

class ibus_device;

class bus final : public ibus_interface
{
    friend class cpu_mos6502;

public:
    bus() = default;
    explicit bus(std::vector<ibus_device *> devices);
    explicit bus(std::vector<ibus_device *> &&devices);
    ~bus() = default;

    bus(bus &&) noexcept = delete;
    auto operator=(bus &&) noexcept -> bus & = delete;

    bus(const bus &) noexcept = delete;
    auto operator=(const bus &) noexcept -> bus & = delete;

    void write(const std::uint16_t address, const std::uint8_t value) noexcept;
    auto read(const std::uint16_t) noexcept -> std::uint8_t;

    void add(ibus_device &device);

private:
    void set_cpu_bus_interface(ibus_interface *bus_interface) noexcept;
    void on_irq() noexcept override;

    std::vector<ibus_device *> devices_;
    ibus_interface *cpu_{};
};

} // namespace emu6502
