#include <model/computer.h>
#include <model/rom.h>
#include <model/ram.h>
#include <model/acia_6551.h>
#include <model/via_6522.h>

namespace rua1::model
{

computer::computer(view::imain_window &main_window, std::filesystem::path path)
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
            {
                auto component = std::make_unique<ram>(main_window_, device->as<config::ram_device_config>());
                bus_.add(component->get_device());
                components_.emplace_back(std::move(component));
                break;
            }
            case config::device_type::acia_6551:
            {
                auto component =
                    std::make_unique<acia_6551>(main_window_, device->as<config::acia_6551_device_config>());
                bus_.add(component->get_device());
                components_.emplace_back(std::move(component));
                break;
            }
            case config::device_type::via_6522:
            {
                auto component = std::make_unique<via_6522>(main_window_, device->as<config::via_6522_device_config>());
                bus_.add(component->get_device());
                components_.emplace_back(std::move(component));
                break;
            }
            default:;
        }
    }

    // TODO: The CPU view should be shown by default.
    // cpu_.create_view();
}

computer::~computer() = default;

} // namespace rua1::model
