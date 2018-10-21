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

class frmacia_model_interface
{
public:
    frmacia_model_interface(frmacia_model_interface &&) noexcept = delete;
    auto operator=(frmacia_model_interface &&) noexcept -> frmacia_model_interface & = delete;

    frmacia_model_interface(const frmacia_model_interface &) noexcept = delete;
    auto operator=(const frmacia_model_interface &) noexcept -> frmacia_model_interface & = delete;

protected:
    frmacia_model_interface() = default;
    virtual ~frmacia_model_interface() = default;
};

class frmacia final : public QFrame
{
    Q_OBJECT

public:
    explicit frmacia(frmacia_model_interface &model_interface, std::function<void()> close_signal,
                     QWidget *parent = nullptr);
    ~frmacia();

    frmacia(frmacia &&) noexcept = delete;
    auto operator=(frmacia &&) noexcept -> frmacia & = delete;

    frmacia(const frmacia &) noexcept = delete;
    auto operator=(const frmacia &) noexcept -> frmacia & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmacia> ui_;
    frmacia_model_interface &model_interface_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
