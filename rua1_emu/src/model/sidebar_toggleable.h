#pragma once

#include <view/imain_window.h>
#include <view/sidebar_toggle_button.h>
#include <mos6502/ibus_device.h>

namespace rua1::model
{

template <typename view_t, typename model_interface_t>
class sidebar_toggleable
{
public:
    explicit sidebar_toggleable(model_interface_t &model_interface, std::string text, view::imain_window &main_window)
        : main_window_{main_window}
        , view_{}
        , model_interface_{model_interface}
        , sidebar_button_{main_window.register_toggle_button(
              text, [this](auto checked) { internal_on_sidebar_button_toggled(checked); })}
        , text_{std::move(text)}
    {
    }

    virtual ~sidebar_toggleable() = default;

    sidebar_toggleable(sidebar_toggleable &&) noexcept = delete;
    auto operator=(sidebar_toggleable &&) noexcept -> sidebar_toggleable & = delete;

    sidebar_toggleable(const sidebar_toggleable &) noexcept = delete;
    auto operator=(const sidebar_toggleable &) noexcept -> sidebar_toggleable & = delete;

protected:
    auto view() const noexcept
    {
        return view_;
    }

    virtual void on_view_created() = 0;
    virtual void on_view_destroyed() = 0;

    void internal_create_view()
    {
        assert(!view_);
        view_ = new view_t{model_interface_, [this]() { internal_on_view_closed(); }};
        main_window_.add_mdi_child(view_);
        view_->setWindowTitle(QString::fromStdString(text_));
        view_->show();

        on_view_created();
    }

    void internal_destroy_view()
    {
        assert(view_);

        // Notify before actually closing the view to give a chance of cleaning up if needed.
        on_view_destroyed();

        view_->parentWidget()->close();
        view_ = nullptr;
    }

private:
    void internal_on_sidebar_button_toggled(const bool checked)
    {
        if (checked)
        {
            internal_create_view();
        }
        else
        {
            internal_destroy_view();
        }
    }

    void internal_on_view_closed() const
    {
        sidebar_button_->set_checked(false);
    }

    view::imain_window &main_window_;
    view_t *view_;
    model_interface_t &model_interface_;
    view::sidebar_toggle_button *sidebar_button_;
    std::string text_;
};

} // namespace rua1::model
