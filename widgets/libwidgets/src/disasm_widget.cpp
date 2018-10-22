#include <widgets/disasm_widget.h>
#include <QPainter>
#include <QFontDatabase>

namespace rua1::widgets
{

disasm_widget::disasm_widget(QWidget *parent)
    : QWidget{parent}
    , address_background_{0xF0, 0xF0, 0xFE}
{
    auto f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    f.setPointSize(12);
    f.setStyleHint(QFont::TypeWriter);
    setFont(f);

    QFontMetrics metrics{f};
    charsize_ = QSize{metrics.width(" "), metrics.height()};
}

void disasm_widget::paintEvent(QPaintEvent *pe)
{
    QPainter painter{this};
    painter.drawLine(0, 0, width(), height());

    painter.drawText(0, charsize_.height(), "8d 02 44");
    painter.drawText(100, charsize_.height(), "sta $0244");
}

} // namespace rua1::widgets
