#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QCryptographicHash>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok = false;
    QString pin = QInputDialog::getText(
        nullptr,
        QObject::tr("Авторизация"),
        QObject::tr("Введите PIN-код:"),
        QLineEdit::Password,
        "",
        &ok
        );

    QByteArray hash = QCryptographicHash::hash(pin.toUtf8(),QCryptographicHash::Md5).toHex();

    const QByteArray correctHash = QByteArray("81dc9bdb52d04dc20036dbd8313ed055");

    if (hash != correctHash) {
        QMessageBox::warning(nullptr, QObject::tr("Ошибка"), QObject::tr("PIN-код неверный!"));
        return 0;
    }

    if (!ok) {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
