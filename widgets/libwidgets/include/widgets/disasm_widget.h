#pragma once

#include <QWidget>

namespace rua1::widgets
{

class disasm_widget final : public QWidget
{
    Q_OBJECT

public:
    explicit disasm_widget(QWidget *parent = nullptr);
    ~disasm_widget() = default;

    disasm_widget(disasm_widget &&) noexcept = delete;
    auto operator=(disasm_widget &&) noexcept -> disasm_widget & = delete;

    disasm_widget(const disasm_widget &) noexcept = delete;
    auto operator=(const disasm_widget &) noexcept -> disasm_widget & = delete;

private:
    void paintEvent(QPaintEvent *pe) override;

    QSize charsize_;
    QColor address_background_;
};

} // namespace rua1::widgets
