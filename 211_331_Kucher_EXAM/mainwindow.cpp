#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>

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

void MainWindow::showData()
{
    QString text;

    for (const TripRecord &rec : m_records) {
        text += tr("Карта: %1\n").arg(rec.cardNumber);
        text += tr("Маршрут: %1\n").arg(rec.route);
        text += tr("Время (time_t): %1\n").arg(rec.timestamp);
        text += tr("Хеш (MD5): %1\n").arg(rec.hash);
        text += "-------------------------\n";
    }

    ui->textEditData->setPlainText(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}
