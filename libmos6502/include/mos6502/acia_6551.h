#pragma once

#include <mos6502/ibus_device.h>
#include <mos6502/ic_register.h>
#include <functional>

namespace mos6502
{

using acia_6551_receive_func = std::function<void(std::uint8_t)>;

struct acia_6551_settings
{
    const std::uint16_t send_recv_address{};
    const std::uint16_t status_reg_address{};
    const std::uint16_t command_reg_address{};
    const std::uint16_t control_reg_address{};
    const bool simulate_wdc_bugs{};
    acia_6551_receive_func receive_func;
};

class acia_6551 : public ibus_device
{
public:
    enum class recv_data_error
    {
        none,
        frame,
        parity
    };

    enum class stop_bit_count
    {
        one,
        two,
        one_and_a_half
    };

    enum class clock_source
    {
        external_receiver_clock,
        baud_rate
    };

    explicit acia_6551(const acia_6551_settings settings) noexcept;
    virtual ~acia_6551() = default;

    acia_6551(acia_6551 &&) noexcept = delete;
    auto operator=(acia_6551 &&) noexcept -> acia_6551 & = delete;

    acia_6551(const acia_6551 &) noexcept = delete;
    auto operator=(const acia_6551 &) noexcept -> acia_6551 & = delete;

    void recv_data(const std::uint8_t data, const recv_data_error error = recv_data_error::none) noexcept;

    auto baud_rate() const noexcept -> float;
    auto word_length() const noexcept -> int;
    auto stop_bits() const noexcept -> stop_bit_count;
    auto receiver_clock_source() const noexcept -> clock_source;

private:
    void hard_reset() noexcept;
    void soft_reset() noexcept;

    void send_data(const std::uint8_t data) noexcept;

    void write(const std::uint16_t address, const std::uint8_t value) noexcept override;
    auto read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t> override;

    ic_register send_recv_data_register_;
    ic_register status_register_;
    ic_register command_register_;
    ic_register control_register_;

    /*!
     * On the WDC 65C51, bit4 of the status register is always 1. This means that
     * 1. It can't be used for detecting when data was actually sent
     * 2. Enabling interrupt on send will cause a storm of interrupts to the CPU.
     *
     * Since the WDC IC's are the only ones still in production, it's important to
     * simulate this inherently wrong behavior. It's very likely to be reality.
     */
    bool simulate_wdc_bugs_;

    acia_6551_receive_func receive_func_;
};

} // namespace mos6502
