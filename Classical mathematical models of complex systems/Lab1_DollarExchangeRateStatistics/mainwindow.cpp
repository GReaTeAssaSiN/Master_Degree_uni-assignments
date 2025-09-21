#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "myfuncs.h"
#include "workplaceform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_linear_regr->setEnabled(false);
    ui->pushButton_inverse_linear_regr->setEnabled(false);
    ui->pushButton_exp_regr->setEnabled(false);
    ui->pushButton_hyper_regr->setEnabled(false);
    ui->pushButton_parab_regr->setEnabled(false);
    ui->pushButton_log_regr->setEnabled(false);
    ui->pushButton_power_regr->setEnabled(false);
    ui->pushButton_polynom_regr->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ChooseFile_clicked()
{
    QString str{};
    str = QFileDialog::getOpenFileName(this, "Выбрать файл", "D:/KAI_MAGISTRATURA/KMMSS/Labs",
                                       "All Files (*.*);; XLSX Files (*.xlsx);");
    if (ui->choosenFile_label->text().isEmpty() || !str.isEmpty())
        ui->choosenFile_label->setText(str);
    if (!str.isEmpty() && loadDataFromExcel(str, ui->data_tableView)){
        ui->pushButton_linear_regr->setEnabled(true);
        ui->pushButton_inverse_linear_regr->setEnabled(true);
        ui->pushButton_exp_regr->setEnabled(true);
        ui->pushButton_hyper_regr->setEnabled(true);
        ui->pushButton_parab_regr->setEnabled(true);
        ui->pushButton_log_regr->setEnabled(true);
        ui->pushButton_power_regr->setEnabled(true);
        ui->pushButton_polynom_regr->setEnabled(true);
    }
}

void MainWindow::show_window(){
    this->show();
}

void MainWindow::on_pushButton_linear_regr_clicked()
{
    WorkplaceForm *workplace_form = new WorkplaceForm(0, ui->data_tableView);
    connect(workplace_form, &WorkplaceForm::backToMain, this, &MainWindow::show_window);
    this->hide();
    workplace_form->show();
}

