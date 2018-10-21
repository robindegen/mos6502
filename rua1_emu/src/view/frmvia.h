#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmvia;
}

namespace rua1::view
{

class frmvia final : public QFrame
{
    Q_OBJECT

public:
    explicit frmvia(std::function<void()> close_signal, QWidget *parent = nullptr);
    ~frmvia();

    frmvia(frmvia &&) noexcept = delete;
    auto operator=(frmvia &&) noexcept -> frmvia & = delete;

    frmvia(const frmvia &) noexcept = delete;
    auto operator=(const frmvia &) noexcept -> frmvia & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmvia> ui_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
