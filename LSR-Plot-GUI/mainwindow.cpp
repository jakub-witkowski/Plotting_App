/* QTableView handling and related aspects based on:
https://riptutorial.com/qt/example/13705/a-simple-read-only-table-to-view-data-from-a-model */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

/* including self-defined classes */
#include "tplot.h"

/* including Qt classes */
#include <QString>
#include <QFileDialog>

/* including STL header files */
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

TestModel::TestModel(QObject *parent) : QAbstractTableModel(parent)
{
}

/* Populate the model with data */
void TestModel::populateData(const QList<QString> &dataset_depths,const QList<QString> &dataset_ages)
{
    tiepoint_depth.clear();
    tiepoint_depth = dataset_depths;
    tiepoint_age.clear();
    tiepoint_age = dataset_ages;
    return;
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tiepoint_depth.length();
}

int TestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return tiepoint_depth[index.row()];
    } else if (index.column() == 1) {
        return tiepoint_age[index.row()];
    }
    return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Depth (mbsf)");
        } else if (section == 1) {
            return QString("Age (Ma)");
        }
    }
    return QVariant();
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "~/",
                                                    "Comma-Separated Value files (*.csv)");

    /* proceed if file name is not an empty string; more than four chars (".csv") is a minimum valid filename length */
    if (filename.length() > 4)
    {
        dataset = new TData(filename.toStdString());
        dataset->load_input();

        QList<QString> dataset_depths;
        QList<QString> dataset_ages;

        // load data from dataset to be displayed in widget
        for (int i = 0; i < dataset->get_depths_vector_size(); i++)
        {
            dataset_depths.append(QString::number(dataset->get_depths(i)));
            dataset_ages.append(QString::number(dataset->get_ages(i)));
        }

        // Create model:
        TestModel *AgeModel = new TestModel(this);

        // Populate model with data:
        AgeModel->populateData(dataset_depths,dataset_ages);

        // Connect model to table view:
        ui->tableView->setModel(AgeModel);

        // Make table header visible and display table:
        ui->tableView->horizontalHeader()->setVisible(true);
        ui->tableView->show();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!dataset)
    {
        QMessageBox message1;
        message1.setText("No input data loaded!");
        message1.exec();
    }

    /* is the input data sorted?
    if (dataset)
    {
        if (!dataset->test_input_order())
        {
            QMessageBox message2;
            message2.setText("The input data is not properly sorted! Inspect the input file and try again.");
            message2.exec();
        }
    }*/

    /* dataset is checked for whether gaps exist */
    if (dataset)
        dataset->find_hiatus();

    /* if n gaps are found, n+1 class TSegment objects are created, and data is copied to the respective segments */
    if ((dataset) && (dataset->get_segment_indexes_size() != 0))
    {
        for (int i = 0; i < dataset->get_segment_indexes_size(); i++)
        {
            segments.push_back(TSegment(dataset, dataset->get_index(i).first, dataset->get_index(i).second));
            segments[i].copy_ages_to_segment();
            segments[i].copy_depths_to_segment();
            segments[i].set_g1_ptr();
            segments[i].compute_lsr_values();
        }

        /* polynomial fitting is performed; test for overfitting is performed as well */
        for (size_t i = 0; i < segments.size(); i++)
        {
            for (int j = 0; j < 10; j++)
            {
                segments[i].add_to_fit_vector(j);
            }

            segments[i].perform_fitting();
            segments[i].get_fit_line_for_plot(segments[i].find_the_best_fit(0));
            segments[i].get_pretty_fit_line_for_plot(segments[i].find_the_best_fit(0));

            is_overfitted = segments[i].test_for_overfitting();
            int index{1};
            while (is_overfitted == true)
            {
                segments[i].clear_fit_line_vector();
                segments[i].get_fit_line_for_plot(segments[i].find_the_best_fit(index));
                segments[i].clear_pretty_fit_line_vector();
                segments[i].get_pretty_fit_line_for_plot(segments[i].find_the_best_fit(index));
                index++;
                is_overfitted = segments[i].test_for_overfitting();
            }

            /* linear sedimentation rates are smoothed using the selected polynomial fit */
            segments[i].set_g2_ptr_pretty();
            segments[i].lsr_smoothing();
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    exit(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!dataset)
    {
        QMessageBox message3;
        message3.setText("No input data loaded!");
        message3.exec();
    }

    if (dataset)
        dataset = nullptr;

    if (segments.size() > 0)
        segments.clear();

    if (plot)
        plot = nullptr;

    ui->tableView->hide();
}

void MainWindow::on_pushButton_5_clicked()
{
    if (!dataset)
    {
        QMessageBox message4;
        message4.setText("No input data loaded!");
        message4.exec();
    }

    /* ROOT widget TApplication is created */
    TApplication app("app", nullptr, nullptr);
    TRootCanvas *rc{nullptr};

    /* determine the number of segments and plot the results */
    if (segments.size() == 1)
    {
        // std::unique_ptr<TPlot> plot(new TPlot(segments[0]));
        plot = new TPlot(segments[0]);
        plot->plot();
        rc = (TRootCanvas *)plot->cnv->GetCanvasImp();
        rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    }
    else if (segments.size() > 1)
    {
        // std::unique_ptr<TPlot> plot(new TPlot((int)segments.size(), segments));
        plot = new TPlot((int)segments.size(), segments);

        for (size_t i = 0; i < segments.size(); i++)
        {
            plot->set_segm_ptr(&segments[i]);
            plot->copy_ages_to_plot();
            plot->copy_depths_to_plot();
            plot->set_g1_ptr();

            /* modifications to the data vectors to reflect hiatuses between segments */
            if (segments.size() > 1)
                if ((i > 0) && (i <= segments.size() - 1))
                    plot->set_lsr_plot_values(0);

            plot->copy_lsr_plot_values_to_plot();

            if (segments.size() > 1)
                if ((i >= 0) && (i < segments.size() - 1))
                    plot->set_lsr_plot_values(0);

            if (segments.size() > 1)
                if ((i > 0) && (i <= segments.size() - 1))
                    plot->set_lsr_plot_ages(segments[i].get_lsr_plot_age(0)); // repeat first element from the segment to be copied

            plot->copy_lsr_plot_ages_to_plot();

            if (segments.size() > 1)
                if ((i >= 0) && (i < segments.size() - 1))
                    plot->set_lsr_plot_ages(plot->get_lsr_plot_age(plot->get_lsr_ages_vector_size() - 1));

            plot->set_g3_ptr();

            if (segments.size() > 1)
                if ((i > 0) && (i <= segments.size() - 1))
                    plot->set_smoothed_lsr_plot_values(0);

            plot->copy_smoothed_lsr_plot_values_to_plot();

            if (segments.size() > 1)
                if ((i >= 0) && (i < segments.size() - 1))
                    plot->set_smoothed_lsr_plot_values(0);

            plot->set_g4_ptr();
        }

        plot->plot_from_array();
        rc = (TRootCanvas *)plot->cnv->GetCanvasImp();
        rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    }

    app.Run(kTRUE);
}



void MainWindow::on_pushButton_7_clicked()
{

}

/* Export output into two CSV files: one for tabular data, one for polynomial regression equation for each segment */
void MainWindow::on_pushButton_6_clicked()
{
    if (!dataset)
    {
        QMessageBox message5;
        message5.setText("No input data loaded!");
        message5.exec();
    }

    if ((dataset) && (!plot))
    {
        QMessageBox message6;
        message6.setText("Analyze data before exporting output!");
        message6.exec();
    }

    if (dataset && plot)
    {
        std::ofstream output_file("output.csv");
        std::string output_line;

        if (output_file.is_open())
        {
            output_line = "Depth (mbsf),Age (Ma), raw LSR (cm/kyr), smoothed LSR (cm/kyr)";
            output_line.append("\n");
            output_file << output_line;
            output_line.clear();

            for (size_t i = 0; i < dataset->get_raw_data_size(); i++)
            {
                output_line.append(std::to_string(dataset->get_depths(i)));
                output_line.append(",");
                output_line.append(std::to_string(dataset->get_ages(i)));
                output_line.append(",");

                if (i == 0)
                {
                    output_line.append(std::to_string(plot->get_lsr_plot_value(i)));
                    output_line.append(",");
                }
                else if ((i + i) >= plot->get_lsr_ages_vector_size())
                    output_line.append("");
                else
                {
                    output_line.append(std::to_string(plot->get_lsr_plot_value(i+i)));
                    output_line.append(",");
                }

                if (i == 0)
                {
                    output_line.append(std::to_string(plot->get_smoothed_lsr_plot_value(i)));
                }
                else if ((i + i) >= plot->get_lsr_ages_vector_size())
                    output_line.append("");
                else
                {
                    output_line.append(std::to_string(plot->get_smoothed_lsr_plot_value(i+i)));
                }

                output_line.append("\n");

                output_file << output_line;
                output_line.clear();
            }
        }

        output_file.close();

        QMessageBox message7;
        message7.setText("Output export successful.");
        message7.exec();
    }
}

