#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/// Структура, описывающая одну запись о поездке в транспорте.
struct TripRecord
{
    QString cardNumber;
    QString route;
    qint64 timestamp;
    QString hash;
};

/// Главное окно приложения, отображающее список поездок и выполняющее проверку целостности данных.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<TripRecord> m_records;
    /// Вычисляет ожидаемый хеш записи по формуле (1) на основе полей записи и хеша предыдущей.
    QString calculateRecordHash(const TripRecord &rec, const QString &prevHash) const;
    /// Загружает данные из файла по умолчанию и обновляет интерфейс.
    void loadData();
    /// Считывает записи из CSV-файла по указанному пути и заполняет список m_records.
    void loadFromCsv(const QString &path);
    /// Отображает текущий список записей в QTextEdit и подсвечивает некорректные записи.
    void showData();
};

#endif
