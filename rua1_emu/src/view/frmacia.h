#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmacia;
}

namespace rua1::view
{

class frmacia final : public QFrame
{
    Q_OBJECT

public:
    explicit frmacia(std::function<void()> close_signal, QWidget *parent = nullptr);
    ~frmacia();

    frmacia(frmacia &&) noexcept = delete;
    auto operator=(frmacia &&) noexcept -> frmacia & = delete;

    frmacia(const frmacia &) noexcept = delete;
    auto operator=(const frmacia &) noexcept -> frmacia & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmacia> ui_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
