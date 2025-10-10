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
    // Деактивация кнопок
    ui->pushButton_linear_regr->setEnabled(false);
    ui->pushButton_inverse_linear_regr->setEnabled(false);
    ui->pushButton_exp_regr->setEnabled(false);
    ui->pushButton_hyper_regr->setEnabled(false);
    ui->pushButton_parab_regr->setEnabled(false);
    ui->pushButton_log_regr->setEnabled(false);
    ui->pushButton_power_regr->setEnabled(false);
    ui->pushButton_polynom_regr->setEnabled(false);
    // Подключение сигналов
    connect(ui->pushButton_linear_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_inverse_linear_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_exp_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_hyper_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_parab_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_log_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_power_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
    connect(ui->pushButton_polynom_regr, &QPushButton::clicked, this, &MainWindow::regression_button_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ChooseFile_clicked()
{
    QString str{};
    str = QFileDialog::getOpenFileName(this, "Выбрать файл", "D:/KAI_MAGISTRATURA/KMMSS/Labs/Lab1",
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
    // Определение нажатой кнопки
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int idx{};
    if (button->objectName() == "pushButton_linear_regr")
        idx = 0;
    else if (button->objectName() == "pushButton_inverse_linear_regr")
        idx = 1;
    else if (button->objectName() == "pushButton_exp_regr")
        idx = 2;
    else if (button->objectName() == "pushButton_hyper_regr")
        idx = 3;
    else if (button->objectName() == "pushButton_parab_regr")
        idx = 4;
    else if (button->objectName() == "pushButton_log_regr")
        idx = 5;
    else if (button->objectName() == "pushButton_power_regr")
        idx = 6;
    else if (button->objectName() == "pushButton_polynom_regr")
        idx = 7;

    // Переменные для записи эксперементальных данных из .xlsx файла
    QVector<QString> dataColumns{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};
    // Проверка загруженной таблицы перед построением регрессионной модели
    bool check = readDataAndCurs(ui->data_tableView, dataColumns, numericDates, cursValues);
    if (check){
        WorkplaceForm *workplace_form = new WorkplaceForm(idx, dataColumns, numericDates, cursValues);
        connect(workplace_form, &WorkplaceForm::backToMain, this, &MainWindow::show_window);
        this->hide();
        workplace_form->show();
    }
}
