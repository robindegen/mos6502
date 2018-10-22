#include <emu6502/acia_6551.h>
#include <cassert>
#include <utility>

namespace emu6502
{

static constexpr std::uint8_t status_parity_error_bit = 0x01;
static constexpr std::uint8_t status_framing_error_bit = 0x02;
static constexpr std::uint8_t status_overrun_bit = 0x04;
static constexpr std::uint8_t status_receiver_data_register_full_bit = 0x08;
static constexpr std::uint8_t status_transmitter_data_empty_bit = 0x10;
static constexpr std::uint8_t status_interrupt_bit = 0x80;

static constexpr std::uint8_t control_receiver_clock_source_bit = 0x10;
static constexpr std::uint8_t control_stop_bit_number_bit = 0x80;

acia_6551::acia_6551(const acia_6551_settings settings) noexcept
    : send_recv_data_register_{settings.send_recv_address}
    , status_register_{settings.status_reg_address}
    , command_register_{settings.command_reg_address}
    , control_register_{settings.control_reg_address}
    , simulate_wdc_bugs_{settings.simulate_wdc_bugs}
    , receive_func_{settings.receive_func}
{
    hard_reset();
}

void acia_6551::recv_data(const std::uint8_t data, const recv_data_error error) noexcept
{
    if (aeon::common::check_bit_flag(status_register_.get(), status_receiver_data_register_full_bit))
        status_register_.set_bit_flags(status_overrun_bit);

    if (error == recv_data_error::parity)
        status_register_.set_bit_flags(status_parity_error_bit);

    if (error == recv_data_error::frame)
        status_register_.set_bit_flags(status_framing_error_bit);

    status_register_.set_bit_flags(status_receiver_data_register_full_bit);
    send_recv_data_register_ = data;
}

auto acia_6551::baud_rate() const noexcept -> float
{
    // If an external clock source is used, the baud rate is not known.
    if (aeon::common::check_bit_flag(control_register_.get(), control_receiver_clock_source_bit))
        return 0;

    const auto baud_index = control_register_.get_low_nibble();

    switch (baud_index)
    {
        case 0:
            return 16.0f; // TODO: Figure out exactly how this RxC external clock rate works.
        case 1:
            return 50.0f;
        case 2:
            return 75.0f;
        case 3:
            return 109.92f;
        case 4:
            return 134.58f;
        case 5:
            return 150.0f;
        case 6:
            return 300.0f;
        case 7:
            return 600.0f;
        case 8:
            return 1200.0f;
        case 9:
            return 1800.0f;
        case 10:
            return 2400.0f;
        case 11:
            return 3600.0f;
        case 12:
            return 4800.0f;
        case 13:
            return 7200.0f;
        case 14:
            return 9600.0f;
        case 15:
            return 19200.0f;
        default:
            assert(false);
            return 0;
    }
}

auto acia_6551::word_length() const noexcept -> int
{
    // Bit 5 and 6 are word length bits.
    const auto word_length_idx = control_register_.get_bit_range(5, 2);

    switch (word_length_idx)
    {
        case 0:
            return 8;
        case 1:
            return 7;
        case 2:
            return 6;
        case 3:
            return 5;
        default:
            assert(false);
            return 0;
    }
}

auto acia_6551::stop_bits() const noexcept -> stop_bit_count
{
    if (control_register_.check_bit_flags(control_stop_bit_number_bit))
        return stop_bit_count::one;
    else
    {
        if (word_length() == 5)
            return stop_bit_count::one_and_a_half;

        return stop_bit_count::two;
    }
}

auto acia_6551::receiver_clock_source() const noexcept -> clock_source
{
    if (control_register_.check_bit_flags(control_receiver_clock_source_bit))
        return clock_source::external_receiver_clock;

    return clock_source::baud_rate;
}

void acia_6551::hard_reset() noexcept
{
    status_register_ = 0;
    control_register_ = 0;
    command_register_ = 0;

    if (simulate_wdc_bugs_)
        status_register_.set_bit_flags(status_transmitter_data_empty_bit);
}

void acia_6551::soft_reset() noexcept
{
    status_register_.clear_bit_flags(status_overrun_bit);
    command_register_.mask_bit_flags(0xE0); // All but PMC1, PMC0 and PME are reset to 0.

    if (simulate_wdc_bugs_)
        status_register_.set_bit_flags(status_transmitter_data_empty_bit);
}

void acia_6551::send_data(const std::uint8_t data) noexcept
{
    receive_func_(data);

    if (simulate_wdc_bugs_)
        status_register_.set_bit_flags(status_transmitter_data_empty_bit);
    else
        status_register_.clear_bit_flags(status_transmitter_data_empty_bit);
}

void acia_6551::write(const std::uint16_t address, const std::uint8_t value) noexcept
{
    if (address == send_recv_data_register_.address())
    {
        send_data(value);
    }
    else if (address == status_register_.address())
    {
        soft_reset();
    }
    else if (address == command_register_.address())
    {
        command_register_ = value;
    }
    else if (address == control_register_.address())
    {
        control_register_ = value;
    }
}

auto acia_6551::read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t>
{
    if (address == send_recv_data_register_.address())
    {
        // On read, all error flags are cleared, and the buffer is empty.
        status_register_.mask_bit_flags(0xF0);
        return {true, send_recv_data_register_.get()};
    }
    else if (address == status_register_.address())
    {
        const auto value = status_register_.get();
        status_register_.clear_bit_flags(status_interrupt_bit);
        return {true, value};
    }
    else if (address == command_register_.address())
    {
        return {true, command_register_.get()};
    }
    else if (address == control_register_.address())
    {
        return {true, control_register_.get()};
    }

    return {false, static_cast<std::uint8_t>(0)};
}

} // namespace emu6502
