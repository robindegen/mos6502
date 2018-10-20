#include <model/computer.h>
#include <model/rom.h>

namespace rua1::model
{

computer::computer(const view::imain_window &main_window, std::filesystem::path path)
    : main_window_{main_window}
    , config_{std::move(path)}
    , bus_{}
    , cpu_{main_window_, bus_}
    , components_{}
{
    const auto &device_config = config_.get_device_config();
    for (const auto &device : device_config)
    {
        switch (device->type())
        {
            case config::device_type::rom:
            {
                auto component = std::make_unique<rom>(main_window_, device->as<config::rom_device_config>());
                bus_.add(component->get_device());
                components_.emplace_back(std::move(component));
                break;
            }
            case config::device_type::ram:
                break;
            case config::device_type::acia_6551:
                break;
            case config::device_type::via_6522:
                break;
            default:;
        }
    }
}

computer::~computer() = default;

} // namespace rua1::model
