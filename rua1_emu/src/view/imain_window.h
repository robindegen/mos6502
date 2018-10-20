#pragma once

#include <QWidget>

namespace rua1::view
{

class imain_window
{
public:
    imain_window(imain_window &&) noexcept = delete;
    auto operator=(imain_window &&) noexcept -> imain_window & = delete;

    imain_window(const imain_window &) noexcept = delete;
    auto operator=(const imain_window &) noexcept -> imain_window & = delete;

    virtual void add_mdi_child(QWidget *widget, Qt::WindowFlags flags = Qt::WindowFlags{}) const noexcept = 0;
    virtual void remove_mdi_child(QWidget *widget) const noexcept = 0;

protected:
    imain_window() = default;
    virtual ~imain_window() = default;
};

} // namespace rua1::view
