#pragma once

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <functional>

namespace rua1::view
{

class sidebar_toggle_button final : QObject
{
    Q_OBJECT

public:
    explicit sidebar_toggle_button(QVBoxLayout *layout, const QString &text, const bool default_state,
                                   std::function<void(const bool)> on_toggle) noexcept;

    ~sidebar_toggle_button();

    sidebar_toggle_button(sidebar_toggle_button &&) noexcept = delete;
    auto operator=(sidebar_toggle_button &&) noexcept -> sidebar_toggle_button & = delete;

    sidebar_toggle_button(const sidebar_toggle_button &) noexcept = delete;
    auto operator=(const sidebar_toggle_button &) noexcept -> sidebar_toggle_button & = delete;

    void set_checked(const bool checked) const;

private:
    std::function<void(const bool)> on_toggle_;
    QVBoxLayout *layout_;
    QPushButton *button_;
    QMetaObject::Connection signal_;
};

} // namespace rua1::view
