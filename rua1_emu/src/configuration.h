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

class via_6522_device_config final : public device_config
{
public:
    explicit via_6522_device_config(std::string name, const bool enabled, const int iorb_register,
                                    const int iora_register, const int ddrb_register, const int ddra_register,
                                    const int t1cl_register, const int t1ch_register, const int t1ll_register,
                                    const int t1lh_register, const int t2cl_register, const int t2ch_register,
                                    const int sr_register, const int acr_register, const int pcr_register,
                                    const int ifr_register, const int ier_register, const int iora_no_hs_register)
        : device_config{device_type::via_6522, std::move(name), enabled}
        , iorb_register_{iorb_register}
        , iora_register_{iora_register}
        , ddrb_register_{ddrb_register}
        , ddra_register_{ddra_register}
        , t1cl_register_{t1cl_register}
        , t1ch_register_{t1ch_register}
        , t1ll_register_{t1ll_register}
        , t1lh_register_{t1lh_register}
        , t2cl_register_{t2cl_register}
        , t2ch_register_{t2ch_register}
        , sr_register_{sr_register}
        , acr_register_{acr_register}
        , pcr_register_{pcr_register}
        , ifr_register_{ifr_register}
        , ier_register_{ier_register}
        , iora_no_hs_register_{iora_no_hs_register}
    {
    }

    ~via_6522_device_config() = default;

    via_6522_device_config(via_6522_device_config &&) noexcept = delete;
    auto operator=(via_6522_device_config &&) noexcept -> via_6522_device_config & = delete;

    via_6522_device_config(const via_6522_device_config &) noexcept = delete;
    auto operator=(const via_6522_device_config &) noexcept -> via_6522_device_config & = delete;

    auto iorb_register() const noexcept
    {
        return iorb_register_;
    }

    auto iora_register() const noexcept
    {
        return iora_register_;
    }

    auto ddrb_register() const noexcept
    {
        return ddrb_register_;
    }

    auto ddra_register() const noexcept
    {
        return ddra_register_;
    }

    auto t1cl_register() const noexcept
    {
        return t1cl_register_;
    }

    auto t1ch_register() const noexcept
    {
        return t1ch_register_;
    }

    auto t1ll_register() const noexcept
    {
        return t1ll_register_;
    }

    auto t1lh_register() const noexcept
    {
        return t1lh_register_;
    }

    auto t2cl_register() const noexcept
    {
        return t2cl_register_;
    }

    auto t2ch_register() const noexcept
    {
        return t2ch_register_;
    }

    auto sr_register() const noexcept
    {
        return sr_register_;
    }

    auto acr_register() const noexcept
    {
        return acr_register_;
    }

    auto pcr_register() const noexcept
    {
        return pcr_register_;
    }

    auto ifr_register() const noexcept
    {
        return ifr_register_;
    }

    auto ier_register() const noexcept
    {
        return ier_register_;
    }

    auto iora_no_hs_register() const noexcept
    {
        return iora_no_hs_register_;
    }

private:
    int iorb_register_;
    int iora_register_;
    int ddrb_register_;
    int ddra_register_;
    int t1cl_register_;
    int t1ch_register_;
    int t1ll_register_;
    int t1lh_register_;
    int t2cl_register_;
    int t2ch_register_;
    int sr_register_;
    int acr_register_;
    int pcr_register_;
    int ifr_register_;
    int ier_register_;
    int iora_no_hs_register_;
};

class acia_6551_device_config final : public device_config
{
public:
    explicit acia_6551_device_config(std::string name, const bool enabled, const int send_recv_register,
                                     const int status_register, const int command_register, const int control_register,
                                     const bool simulate_wdc_bugs)
        : device_config{device_type::acia_6551, std::move(name), enabled}
        , send_recv_register_{send_recv_register}
        , status_register_{status_register}
        , command_register_{command_register}
        , control_register_{control_register}
        , simulate_wdc_bugs_{simulate_wdc_bugs}
    {
    }

    ~acia_6551_device_config() = default;

    acia_6551_device_config(acia_6551_device_config &&) noexcept = delete;
    auto operator=(acia_6551_device_config &&) noexcept -> acia_6551_device_config & = delete;

    acia_6551_device_config(const acia_6551_device_config &) noexcept = delete;
    auto operator=(const acia_6551_device_config &) noexcept -> acia_6551_device_config & = delete;

    auto send_recv_register() const noexcept
    {
        return send_recv_register_;
    }

    auto status_register() const noexcept
    {
        return status_register_;
    }

    auto command_register() const noexcept
    {
        return command_register_;
    }

    auto control_register() const noexcept
    {
        return control_register_;
    }

    auto simulate_wdc_bugs() const noexcept
    {
        return simulate_wdc_bugs_;
    }

private:
    int send_recv_register_;
    int status_register_;
    int command_register_;
    int control_register_;
    bool simulate_wdc_bugs_;
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
