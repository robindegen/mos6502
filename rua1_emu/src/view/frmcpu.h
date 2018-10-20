#pragma once

#include <QtWidgets/QDialog>
#include <memory>
#include <functional>

namespace Ui
{
class frmcpu;
}

namespace rua1::view
{

class frmcpu final : public QDialog
{
    Q_OBJECT

public:
    explicit frmcpu(std::function<void()> close_signal, QWidget *parent = nullptr);
    ~frmcpu();

    frmcpu(frmcpu &&) noexcept = delete;
    auto operator=(frmcpu &&) noexcept -> frmcpu & = delete;

    frmcpu(const frmcpu &) noexcept = delete;
    auto operator=(const frmcpu &) noexcept -> frmcpu & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmcpu> ui_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
