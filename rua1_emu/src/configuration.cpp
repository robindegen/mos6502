#include <configuration.h>
#include <json11.hpp>
#include <aeon/streams/file_stream.h>
#include <aeon/streams/stream_reader.h>
#include <aeon/common/string.h>
#include <stdexcept>

namespace rua1::config
{

namespace json11_helpers
{

static auto get_string(const std::map<std::string, json11::Json> &map, const std::string &key)
{
    const auto &str_obj = map.at(key);

    if (!str_obj.is_string())
        throw std::runtime_error{"Expected string."};

    return str_obj.string_value();
}

static auto get_hex_string(const std::map<std::string, json11::Json> &map, const std::string &key)
{
    const auto str = get_string(map, key);
    return aeon::common::string::hex_string_to_int<int>(str);
}

static auto get_bool(const std::map<std::string, json11::Json> &map, const std::string &key)
{
    const auto &bool_obj = map.at(key);

    if (!bool_obj.is_bool())
        throw std::runtime_error{"Expected bool."};

    return bool_obj.bool_value();
}

} // namespace json11_helpers

static auto load_config_file(const std::filesystem::path &path)
{
    aeon::streams::file_stream file{path, aeon::streams::access_mode::read, aeon::streams::file_mode::text};
    aeon::streams::stream_reader reader{file};
    const auto input = reader.read_as_string();

    auto error_string = std::string{};
    auto json = json11::Json::parse(input, error_string, json11::JsonParse::STANDARD);

    if (!error_string.empty())
        throw std::runtime_error{"Config file parse error."};

    return json;
}

auto load_rom_device_config(const std::map<std::string, json11::Json> &map)
{
    const auto name = json11_helpers::get_string(map, "name");
    const auto enabled = json11_helpers::get_bool(map, "enabled");
    const auto offset = json11_helpers::get_hex_string(map, "offset");
    const auto size = json11_helpers::get_hex_string(map, "size");
    const auto path = json11_helpers::get_string(map, "path");
    return std::make_unique<rom_device_config>(name, enabled, offset, size, path);
}

auto load_ram_device_config(const std::map<std::string, json11::Json> &map)
{
    const auto name = json11_helpers::get_string(map, "name");
    const auto enabled = json11_helpers::get_bool(map, "enabled");
    const auto offset = json11_helpers::get_hex_string(map, "offset");
    const auto size = json11_helpers::get_hex_string(map, "size");
    return std::make_unique<ram_device_config>(name, enabled, offset, size);
}

auto load_devices_config(json11::Json &json) -> std::vector<std::unique_ptr<device_config>>
{
    const auto &object_items = json.object_items();
    const auto result = object_items.find("devices");

    if (result == std::end(object_items))
        return {};

    if (!result->second.is_array())
        throw std::runtime_error{"Expected 'devices' to be an array."};

    const auto &json_array = result->second.array_items();

    std::vector<std::unique_ptr<device_config>> devices;

    for (const auto &json_device : json_array)
    {
        const auto &values = json_device.object_items();
        const auto type_result = values.find("type");

        if (type_result == std::end(values))
            throw std::runtime_error{"Missing device type in config."};

        if (!type_result->second.is_string())
            throw std::runtime_error{"Expected device type to be a string."};

        const auto &device_type_str = type_result->second.string_value();

        if (device_type_str == "rom")
        {
            devices.emplace_back(load_rom_device_config(values));
        }
        else if (device_type_str == "ram")
        {
            devices.emplace_back(load_ram_device_config(values));
        }
        else if (device_type_str == "acia_6551")
        {
        }
        else if (device_type_str == "via_6522")
        {
        }
    }

    return devices;
}

configuration::configuration(std::filesystem::path path)
    : path_{std::move(path)}
{
    auto json = load_config_file(path_);
    devices_ = load_devices_config(json);
}

configuration::~configuration() = default;

} // namespace rua1::config
