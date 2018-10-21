#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmcpu;
}

namespace rua1::view
{

class frmcpu final : public QFrame
{
    Q_OBJECT

public:
    explicit frmcpu(std::function<void()> close_signal, QWidget *parent = nullptr);
    ~frmcpu();

    frmcpu(frmcpu &&) noexcept = delete;
    auto operator=(frmcpu &&) noexcept -> frmcpu & = delete;

    frmcpu(const frmcpu &) noexcept = delete;
    auto operator=(const frmcpu &) noexcept -> frmcpu & = delete;

    void set_pc_value(const std::uint16_t val);
    void set_sp_value(const std::uint8_t val);
    void set_a_value(const std::uint8_t val);
    void set_x_value(const std::uint8_t val);
    void set_y_value(const std::uint8_t val);

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmcpu> ui_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
