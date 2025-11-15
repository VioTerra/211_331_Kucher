#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct TripRecord
{
    QString cardNumber;  // номер транспортной карты (16 цифр)
    QString route;       // маршрут ("Москва-Зеленоград")
    qint64 timestamp;    // время в формате time_t
    QString hash;        // MD5 в hex
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<TripRecord> m_records;  // все загруженные записи

    void loadData();                        // загрузка данных при старте
    void loadFromCsv(const QString &path);  // чтение CSV-файла
    void showData();                        // отображение данных в QTextEdit
};

#endif // MAINWINDOW_H
