#include <mos6502/bus.h>
#include <mos6502/ibus_device.h>
#include <cassert>

namespace mos6502
{

bus::bus(std::vector<ibus_device *> devices)
    : devices_{std::move(devices)}
{
}

bus::bus(std::vector<ibus_device *> &&devices)
    : devices_{std::move(devices)}
{
}

void bus::write(const std::uint16_t address, const std::uint8_t value) noexcept
{
    for (auto device : devices_)
    {
        device->write(address, value);
    }
}

auto bus::read(const std::uint16_t address) noexcept -> std::uint8_t
{
    for (auto device : devices_)
    {
        auto [valid, value] = device->read(address);

        if (valid)
            return value;
    }

    return 0;
}

void bus::add(ibus_device &device)
{
    devices_.emplace_back(&device);
}

void bus::set_cpu_bus_interface(ibus_interface *bus_interface) noexcept
{
    assert(bus_interface);
    cpu_ = bus_interface;
}

void bus::on_irq() noexcept
{
    assert(cpu_);
    cpu_->on_irq();
}

} // namespace mos6502
