#include <frmmain.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application{argc, argv};

    rua1::widgets_tests::frmmain frmmain;
    frmmain.show();

    return QApplication::exec();
}
