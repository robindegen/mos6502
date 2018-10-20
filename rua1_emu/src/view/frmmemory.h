#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmmemory;
}

namespace rua1::view
{

class frmmemory final : public QFrame
{
    Q_OBJECT

public:
    explicit frmmemory(std::function<void()> close_signal, QWidget *parent = nullptr);
    ~frmmemory();

    frmmemory(frmmemory &&) noexcept = delete;
    auto operator=(frmmemory &&) noexcept -> frmmemory & = delete;

    frmmemory(const frmmemory &) noexcept = delete;
    auto operator=(const frmmemory &) noexcept -> frmmemory & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmmemory> ui_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
