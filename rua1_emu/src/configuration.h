#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <memory>

namespace rua1::config
{

enum class device_type
{
    rom,
    ram,
    acia_6551,
    via_6522
};

class device_config
{
public:
    ~device_config() = default;

    device_config(device_config &&) noexcept = delete;
    auto operator=(device_config &&) noexcept -> device_config & = delete;

    device_config(const device_config &) noexcept = delete;
    auto operator=(const device_config &) noexcept -> device_config & = delete;

    auto type() const noexcept
    {
        return type_;
    }

    const auto &name() const noexcept
    {
        return name_;
    }

    auto enabled() const noexcept
    {
        return enabled_;
    }

    template <typename T>
    const auto &as() const
    {
        return static_cast<const T &>(*this);
    }

protected:
    explicit device_config(const device_type type, std::string name, const bool enabled)
        : type_{type}
        , name_{std::move(name)}
        , enabled_{enabled}
    {
    }

private:
    device_type type_;
    std::string name_;
    bool enabled_;
};

class rom_device_config final : public device_config
{
public:
    explicit rom_device_config(std::string name, const bool enabled, const int offset, const int size,
                               std::filesystem::path file)
        : device_config{device_type::rom, std::move(name), enabled}
        , offset_{offset}
        , size_{size}
        , file_{std::move(file)}
    {
    }

    ~rom_device_config() = default;

    rom_device_config(rom_device_config &&) noexcept = delete;
    auto operator=(rom_device_config &&) noexcept -> rom_device_config & = delete;

    rom_device_config(const rom_device_config &) noexcept = delete;
    auto operator=(const rom_device_config &) noexcept -> rom_device_config & = delete;

    auto offset() const noexcept
    {
        return offset_;
    }

    auto size() const noexcept
    {
        return size_;
    }

    const auto &file() const noexcept
    {
        return file_;
    }

private:
    int offset_;
    int size_;
    std::filesystem::path file_;
};

class ram_device_config final : public device_config
{
public:
    explicit ram_device_config(std::string name, const bool enabled, const int offset, const int size)
        : device_config{device_type::ram, std::move(name), enabled}
        , offset_{offset}
        , size_{size}
    {
    }

    ~ram_device_config() = default;

    ram_device_config(ram_device_config &&) noexcept = delete;
    auto operator=(ram_device_config &&) noexcept -> ram_device_config & = delete;

    ram_device_config(const ram_device_config &) noexcept = delete;
    auto operator=(const ram_device_config &) noexcept -> ram_device_config & = delete;

    auto offset() const noexcept
    {
        return offset_;
    }

    auto size() const noexcept
    {
        return size_;
    }

private:
    int offset_;
    int size_;
};

class configuration final
{
public:
    explicit configuration(std::filesystem::path path);
    ~configuration();

    configuration(configuration &&) noexcept = delete;
    auto operator=(configuration &&) noexcept -> configuration & = delete;

    configuration(const configuration &) noexcept = delete;
    auto operator=(const configuration &) noexcept -> configuration & = delete;

    void set_device_config(std::vector<std::unique_ptr<device_config>> &&devices)
    {
        devices_ = std::move(devices);
    }

    const auto &get_device_config() const noexcept
    {
        return devices_;
    }

private:
    std::vector<std::unique_ptr<device_config>> devices_;
    std::filesystem::path path_;
};

} // namespace rua1::config
