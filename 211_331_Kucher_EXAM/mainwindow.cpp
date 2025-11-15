#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>
#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadData();
}

void MainWindow::loadData()
{
    const QString filePath = QCoreApplication::applicationDirPath() + "/data.csv";

    loadFromCsv(filePath);
    showData();
}

void MainWindow::loadFromCsv(const QString &path)
{
    m_records.clear();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось открыть файл: %1").arg(path));
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        const QStringList parts = line.split(',');
        if (parts.size() != 4) {
            continue;
        }

        TripRecord rec;
        rec.cardNumber = parts[0].trimmed();
        rec.route      = parts[1].trimmed();

        bool ok = false;
        rec.timestamp = parts[2].trimmed().toLongLong(&ok);
        if (!ok) {
            continue;
        }

        rec.hash = parts[3].trimmed();

        m_records.append(rec);
    }
}

QString MainWindow::calculateRecordHash(const TripRecord &rec, const QString &prevHash) const
{
    QString data = rec.cardNumber
                   + "|" + rec.route
                   + "|" + QString::number(rec.timestamp)
                   + "|" + prevHash;

    QByteArray md5 = QCryptographicHash::hash(data.toUtf8(),
                                              QCryptographicHash::Md5);
    return QString::fromLatin1(md5.toHex());
}

void MainWindow::showData()
{
    if (m_records.isEmpty()) {
        ui->textEditData->clear();
        return;
    }

    QString html;

    QString prevHash = "00000000000000000000000000000000";

    int firstBadIndex = -1;

    for (int i = 0; i < m_records.size(); ++i) {
        const TripRecord &rec = m_records[i];

        QString expected = calculateRecordHash(rec, prevHash);

        bool ok = rec.hash.compare(expected, Qt::CaseInsensitive) == 0;

        if (!ok && firstBadIndex == -1) {
            firstBadIndex = i;
        }

        prevHash = rec.hash;
    }


    prevHash = "00000000000000000000000000000000";

    for (int i = 0; i < m_records.size(); ++i) {
        const TripRecord &rec = m_records[i];

        bool markRed = (firstBadIndex != -1 && i >= firstBadIndex);

        QString block;
        block += "<b>Карта:</b> " + rec.cardNumber + "<br>";
        block += "<b>Маршрут:</b> " + rec.route + "<br>";
        block += "<b>Время (time_t):</b> " + QString::number(rec.timestamp) + "<br>";
        block += "<b>Хеш (из файла):</b> " + rec.hash + "<br>";

        QString expected = calculateRecordHash(rec, prevHash);
        block += "<b>Ожидаемый хеш:</b> " + expected + "<br>";

        bool ok = rec.hash.compare(expected, Qt::CaseInsensitive) == 0;
        block += QString("<b>Статус:</b> %1<br>")
                     .arg(ok ? "OK" : "ОШИБКА");

        QString wrapped;
        if (markRed) {
            wrapped = "<div style=\"color:red; margin-bottom:8px;\">"
                      + block +
                      "</div><hr>";
        } else {
            wrapped = "<div style=\"margin-bottom:8px;\">"
                      + block +
                      "</div><hr>";
        }

        html += wrapped;

        prevHash = rec.hash;
    }

    ui->textEditData->setHtml(html);
}

MainWindow::~MainWindow()
{
    delete ui;
}
