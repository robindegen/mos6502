#pragma once

#include <QApplication>
#include <view/frmmain.h>
#include <view/imain_window.h>
#include <model/computer.h>
#include <memory>

namespace rua1
{

class application final : public view::imain_window
{
public:
    application(int argc, char *argv[]);
    ~application();

    application(application &&) noexcept = delete;
    auto operator=(application &&) noexcept -> application & = delete;

    application(const application &) noexcept = delete;
    auto operator=(const application &) noexcept -> application & = delete;

    auto exec() -> int;

private:
    void add_mdi_child(QWidget *widget, Qt::WindowFlags flags) const noexcept override;

    auto register_toggle_button(const std::string &text, std::function<void(const bool)> on_toggle)
        -> view::sidebar_toggle_button * override;
    void remove_toggle_button(const view::sidebar_toggle_button *button) override;
    void remove_all_toggle_buttons() override;

    QApplication application_;
    view::frmmain frmmain_;
    std::unique_ptr<model::computer> computer_;
};

} // namespace rua1
