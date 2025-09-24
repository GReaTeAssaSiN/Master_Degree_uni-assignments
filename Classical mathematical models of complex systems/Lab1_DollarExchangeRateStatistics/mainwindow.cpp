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

    connect(ui->pushButton_linear_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_inverse_linear_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
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
    if ((ui->choosenFile_label->text().isEmpty() || !str.isEmpty()) && loadDataFromExcel(str, ui->data_tableView)){
        ui->choosenFile_label->setText(str);
        ui->pushButton_linear_regr->setEnabled(true);
        ui->pushButton_inverse_linear_regr->setEnabled(true);
        ui->pushButton_exp_regr->setEnabled(true);
        ui->pushButton_hyper_regr->setEnabled(true);
        ui->pushButton_parab_regr->setEnabled(true);
        ui->pushButton_log_regr->setEnabled(true);
        ui->pushButton_power_regr->setEnabled(true);
        ui->pushButton_polynom_regr->setEnabled(true);
        ui->choosenFile_label->setCursor(Qt::IBeamCursor);
        ui->choosenFile_label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }
    else {
        ui->choosenFile_label->setCursor(Qt::ArrowCursor);
        ui->choosenFile_label->setTextInteractionFlags(Qt::NoTextInteraction);
    }
}

void MainWindow::show_window(){
    this->show();
}

void MainWindow::regression_button_clicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int idx{};
    if (button->objectName() == "pushButton_linear_regr")
        idx = 0;
    else if (button->objectName() == "pushButton_inverse_linear_regr")
        idx = 1;

    WorkplaceForm *workplace_form = new WorkplaceForm(idx, ui->data_tableView);
    connect(workplace_form, &WorkplaceForm::backToMain, this, &MainWindow::show_window);
    this->hide();
    workplace_form->show();
}
