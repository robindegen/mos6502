#pragma once

#include <QWidget>
#include <functional>

namespace rua1::view
{

class sidebar_toggle_button;

class imain_window
{
public:
    imain_window(imain_window &&) noexcept = delete;
    auto operator=(imain_window &&) noexcept -> imain_window & = delete;

    imain_window(const imain_window &) noexcept = delete;
    auto operator=(const imain_window &) noexcept -> imain_window & = delete;

    virtual void add_mdi_child(QWidget *widget, Qt::WindowFlags flags = Qt::WindowFlags{}) const noexcept = 0;
    virtual auto register_toggle_button(const QString &text, const bool default_state,
                                        std::function<void(const bool)> on_toggle) -> sidebar_toggle_button * = 0;
    virtual void remove_toggle_button(const sidebar_toggle_button *button) = 0;
    virtual void remove_all_toggle_buttons() = 0;

protected:
    imain_window() = default;
    virtual ~imain_window() = default;
};

} // namespace rua1::view
