#include "workplaceform.h"
#include "ui_workplaceform.h"
#include "QCloseEvent"
#include "myfuncs.h"

WorkplaceForm::WorkplaceForm(const int &mode, const QTableView *data_tableView, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkplaceForm)
    , m_mode(mode)
{
    ui->setupUi(this);
    connect(ui->pushButton_back, &QPushButton::clicked, this, &WorkplaceForm::on_pushButton_back_clicked);
    switch(mode){
    case 0:
        setWindowTitle("Линейная регрессия");
        break;
    case 1:
        setWindowTitle("Обратная линейная регрессия");
        break;
    case 2:
        setWindowTitle("Экспоненциальная регрессия");
        break;
    case 3:
        setWindowTitle("Гиперболическая регрессия");
        break;
    case 4:
        setWindowTitle("Параболическая регрессия");
        break;
    case 5:
        setWindowTitle("Логарифмическая регрессия");
        break;
    case 6:
        setWindowTitle("Степенная регрессия");
        break;
    case 7:
        setWindowTitle("Полиномиальная регрессия");
        break;
    default:
        setWindowTitle("Неопознанный тип регрессии");
        break;
    }
    // Чтение исходных данных и преобразование к нужному виду
    readDataAndCurs(data_tableView, this->dataColumn, this->numericDates, this->cursValues);
    // Вычисление значений квадратов, сумм и средних значений
    calculateRegressionValues(this->numericDates, this->cursValues, this->xSquared, this->ySquared, this->xyProduct, this->values);
    // Заполнение таблицы
    fillTotalTable(ui->total_tableView, this->dataColumn, this->numericDates, this->cursValues,
                   this->xSquared, this->ySquared, this->xyProduct);
    // Отображение значений величин вне таблицы
    ui->sumX_label->setText(ui->sumX_label->text() + QString::number(this->values.sumX, 'f', 6));
    ui->sumY_label->setText(ui->sumY_label->text() + QString::number(this->values.sumY, 'f', 6));
    ui->sumX2_label->setText(ui->sumX2_label->text() + QString::number(this->values.sumX2, 'f', 6));
    ui->sumY2_label->setText(ui->sumY2_label->text() + QString::number(this->values.sumY2, 'f', 6));
    ui->sumXY_label->setText(ui->sumXY_label->text() + QString::number(this->values.sumXY, 'f', 6));
    ui->meanX_label->setText(ui->meanX_label->text() + QString::number(this->values.meanX, 'f', 6));
    ui->meanY_label->setText(ui->meanY_label->text() + QString::number(this->values.meanY, 'f', 6));
}

WorkplaceForm::~WorkplaceForm()
{
    delete ui;
}

void WorkplaceForm::on_pushButton_back_clicked(){
    emit backToMain();
    this->close();
}

void WorkplaceForm::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit backToMain();
}
