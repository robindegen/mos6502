#pragma once

#include <view/imain_window.h>
#include <view/sidebar_toggle_button.h>
#include <mos6502/ibus_device.h>

namespace rua1::model
{

template <typename view_t>
class sidebar_toggleable
{
public:
    explicit sidebar_toggleable(std::string text, const bool default_shown, view::imain_window &main_window)
        : main_window_{main_window}
        , view_{}
        , sidebar_button_{main_window.register_toggle_button(
              text, default_shown, [this](auto checked) { on_sidebar_button_toggled(checked); })}
        , text_{std::move(text)}
    {
        if (default_shown)
            create_view();
    }

    virtual ~sidebar_toggleable() = default;

    sidebar_toggleable(sidebar_toggleable &&) noexcept = delete;
    auto operator=(sidebar_toggleable &&) noexcept -> sidebar_toggleable & = delete;

    sidebar_toggleable(const sidebar_toggleable &) noexcept = delete;
    auto operator=(const sidebar_toggleable &) noexcept -> sidebar_toggleable & = delete;

protected:
    void create_view()
    {
        assert(!view_);
        view_ = new view_t{[this]() { on_view_closed(); }};
        main_window_.add_mdi_child(view_);
        view_->setWindowTitle(QString::fromStdString(text_));
        view_->show();
    }

    void destroy_view()
    {
        assert(view_);
        view_->parentWidget()->close();
        view_ = nullptr;
    }

    auto view() const noexcept
    {
        return view_;
    }

private:
    void on_sidebar_button_toggled(const bool checked)
    {
        if (checked)
        {
            create_view();
        }
        else
        {
            destroy_view();
        }
    }

    void on_view_closed() const
    {
        sidebar_button_->set_checked(false);
    }

    view::imain_window &main_window_;
    view_t *view_;
    view::sidebar_toggle_button *sidebar_button_;
    std::string text_;
};

} // namespace rua1::model
