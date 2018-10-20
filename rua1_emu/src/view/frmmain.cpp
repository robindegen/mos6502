#include <view/frmmain.h>
#include <view/sidebar_toggle_button.h>
#include <configuration.h>
#include <ui_frmmain.h>

namespace rua1::view
{

frmmain::frmmain()
    : ui_{std::make_unique<Ui::frmmain>()}
{
    ui_->setupUi(this);

    ui_->mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui_->mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

frmmain::~frmmain() = default;

void frmmain::add_mdi_child(QWidget *widget, Qt::WindowFlags flags) const noexcept
{
    ui_->mdi_area->addSubWindow(widget, flags);
}

auto frmmain::register_toggle_button(const QString &text, const bool default_state,
                                     std::function<void(const bool)> on_toggle) -> sidebar_toggle_button *
{
    auto toggle_button_reg =
        std::make_unique<sidebar_toggle_button>(ui_->verticalLayout, text, default_state, std::move(on_toggle));
    const auto toggle_button_reg_ptr = toggle_button_reg.get();
    sidebar_toggle_buttons_.emplace_back(std::move(toggle_button_reg));
    return toggle_button_reg_ptr;
}

void frmmain::remove_toggle_button(const sidebar_toggle_button *button)
{
    sidebar_toggle_buttons_.erase(std::remove_if(std::begin(sidebar_toggle_buttons_), std::end(sidebar_toggle_buttons_),
                                                 [button](const auto &btn) { return btn.get() == button; }),
                                  std::end(sidebar_toggle_buttons_));
}

void frmmain::remove_all_toggle_buttons()
{
    sidebar_toggle_buttons_.clear();
}

} // namespace rua1::view
