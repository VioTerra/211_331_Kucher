#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

QByteArray loadCorrectPinHash()
{
    const QByteArray defaultHash("81dc9bdb52d04dc20036dbd8313ed055");

    const QString filePath =
        QCoreApplication::applicationDirPath() + "/pin.txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return defaultHash;
    }

    QTextStream in(&file);

    QString line = in.readLine().trimmed();
    if (line.isEmpty()) {
        return defaultHash;
    }

    line = line.toLower();

    if (line.size() != 32) {
        return defaultHash;
    }
    for (QChar ch : line) {
        if (!(ch.isDigit() ||
              (ch >= 'a' && ch <= 'f'))) {
            return defaultHash;
        }
    }

    return line.toUtf8();
}

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

    if (!ok) {
        return 0;
    }

    QByteArray hash = QCryptographicHash::hash(pin.toUtf8(), QCryptographicHash::Md5).toHex();

    QByteArray correctHash = loadCorrectPinHash();

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
