#pragma once

#include <QtWidgets/QDialog>
#include <memory>

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
    frmcpu(QWidget *parent = nullptr);
    ~frmcpu();

    frmcpu(frmcpu &&) noexcept = delete;
    auto operator=(frmcpu &&) noexcept -> frmcpu & = delete;

    frmcpu(const frmcpu &) noexcept = delete;
    auto operator=(const frmcpu &) noexcept -> frmcpu & = delete;

signals:
    void closing();

private:
    std::unique_ptr<Ui::frmcpu> ui_;
};

} // namespace rua1::view
