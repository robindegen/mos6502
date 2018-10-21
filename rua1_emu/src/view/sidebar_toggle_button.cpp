#include <view/sidebar_toggle_button.h>

namespace rua1::view
{

sidebar_toggle_button::sidebar_toggle_button(QVBoxLayout *layout, const QString &text,
                                             std::function<void(const bool)> on_toggle) noexcept
    : on_toggle_{std::move(on_toggle)}
    , layout_{layout}
    , button_{new QPushButton{text}}
{
    button_->setCheckable(true);
    button_->setChecked(false);

    const int offset = layout->count();
    assert(offset > 0);
    layout->insertWidget(offset - 1, button_);

    signal_ = connect(button_, &QPushButton::toggled, [this](auto checked) { on_toggle_(checked); });
}

sidebar_toggle_button::~sidebar_toggle_button()
{
    layout_->removeWidget(button_);
    disconnect(signal_);
    button_->deleteLater();
}

void sidebar_toggle_button::set_checked(const bool checked) const
{
    button_->setChecked(checked);
}

} // namespace rua1::view
