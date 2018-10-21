#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include <vector>
#include <functional>

namespace Ui
{
class frmmain;
}

namespace rua1::view
{

class sidebar_toggle_button;

class frmmain final : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmmain();
    ~frmmain();

    frmmain(frmmain &&) noexcept = delete;
    auto operator=(frmmain &&) noexcept -> frmmain & = delete;

    frmmain(const frmmain &) noexcept = delete;
    auto operator=(const frmmain &) noexcept -> frmmain & = delete;

    void add_mdi_child(QWidget *widget, Qt::WindowFlags flags = Qt::WindowFlags{}) const noexcept;

    auto register_toggle_button(const std::string &text, std::function<void(const bool)> on_toggle)
        -> sidebar_toggle_button *;
    void remove_toggle_button(const sidebar_toggle_button *button);
    void remove_all_toggle_buttons();

private:
    std::unique_ptr<Ui::frmmain> ui_;
    std::vector<std::unique_ptr<sidebar_toggle_button>> sidebar_toggle_buttons_;
};

} // namespace rua1::view
