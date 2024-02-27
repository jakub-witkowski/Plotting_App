#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* including Qt classes */
#include <QMainWindow>
#include <QAbstractTableModel>
#include <QtCore>
#include <QMessageBox>

/* including self-defined classes */
#include "tdata.h"
#include "tsegment.h"
#include "tpolynomial.h"
#include "tplot.h"

/* including ROOT Framework classes */
#include "TApplication.h"
#include "TRootCanvas.h"

/* STL header files */
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TestModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TestModel(QObject *parent = 0);

    void populateData(const QList<QString> &dataset_depths, const QList<QString> &dataset_ages);
    void populateData(const QList<QString> &dataset_depths, const QList<QString> &dataset_ages, const QList<QString> &lsr_values);
    void populateData(const QList<QString> &dataset_depths, const QList<QString> &dataset_ages, const QList<QString> &lsr_values, const QList<QString> &smoothed_lsr_values);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> tiepoint_depth;
    QList<QString> tiepoint_age;
    QList<QString> lsr;
    QList<QString> smoothed_lsr;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TData* dataset{nullptr};
    std::vector<TSegment> segments{};
    bool is_overfitted{false};
    TPlot* plot{nullptr};

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    // void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
