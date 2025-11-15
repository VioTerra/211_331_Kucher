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
    QString cardNumber;
    QString route;
    qint64 timestamp;
    QString hash;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<TripRecord> m_records;

    void loadData();
    void loadFromCsv(const QString &path);
    void showData();
};

#endif // MAINWINDOW_H
