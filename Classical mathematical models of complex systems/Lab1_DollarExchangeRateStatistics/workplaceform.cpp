#include "workplaceform.h"
#include "ui_workplaceform.h"
#include "QCloseEvent"
#include "myfuncs.h"
#include <QMessageBox>

WorkplaceForm::WorkplaceForm(const int &mode, const QTableView *data_tableView, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkplaceForm)
{
    ui->setupUi(this);
    // Выделение текста на label
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel* lbl : labels) {
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }
    connect(ui->pushButton_back, &QPushButton::clicked, this, &WorkplaceForm::on_pushButton_back_clicked);
    // Заголовок окна
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
    // ---- ПРЕДОБРАБОТКА ---- //
    // Чтение исходных данных и преобразование к нужному виду
    readDataAndCurs(data_tableView, this->dataColumn, this->numericDates, this->cursValues);

    // ---- ОБЩАЯ ТАБЛИЦА ---- //
    // Вычисление значений квадратов, сумм и средних значений + количества экспериментов (n)
    calculateRegressionTotalValues(this->numericDates, this->cursValues, this->xSquared, this->ySquared, this->xyProduct, this->values, this->coefficients);
    // Заполнение общей таблицы
    fillTotalTable(ui->total_tableView, this->dataColumn, this->numericDates, this->cursValues,
                   this->xSquared, this->ySquared, this->xyProduct);
    // Отображение значений величин вне общей таблицы
    ui->sumX_label->setText(ui->sumX_label->text() + QString::number(this->values.sumX, 'f', 6));
    ui->sumY_label->setText(ui->sumY_label->text() + QString::number(this->values.sumY, 'f', 6));
    ui->sumX2_label->setText(ui->sumX2_label->text() + QString::number(this->values.sumX2, 'f', 6));
    ui->sumY2_label->setText(ui->sumY2_label->text() + QString::number(this->values.sumY2, 'f', 6));
    ui->sumXY_label->setText(ui->sumXY_label->text() + QString::number(this->values.sumXY, 'f', 6));
    ui->meanX_label->setText(ui->meanX_label->text() + QString::number(this->values.meanX, 'f', 6));
    ui->meanY_label->setText(ui->meanY_label->text() + QString::number(this->values.meanY, 'f', 6));

    // ---- ВЫЧИСЛЯЕМАЯ ТАБЛИЦА ---- //
    // Вычиление значений констант и коэффициентов регрессии + построение уравнения регрессии
    bool correlation_flag{};
    switch (mode) {
    case 0: // Линейная
        correlation_flag = calculateLinearRegressionValues(this->numericDates, this->yT, this->values, this->coefficients);
        this->trendLabel = QString("y = %1 + %2*x").arg(coefficients["a0"], 0, 'f', 6).arg(coefficients["a1"], 0, 'f', 2);
        break;
    case 1: // Обратная линейная
        break;
    case 2: // Экспоненциальная
        break;
    case 3: // Гиперболическая
        break;
    case 4: // Параболическая
        break;
    case 5: // Логарифмическая
        break;
    case 6: // Степенная
        break;
    case 7: // Полиномиальная
        break;
    default:
        QMessageBox::warning(nullptr, "Предупреждение", "Неизвестный тип регрессии, не удалось вычислить коэффициенты!");
        break;
    }
    // Вычисление Sост., Sрегр., Sполн. + R2, MSE + Sx2, Sy2, meanSx, meanSy
    bool Sfull_flag = calculateRegressionCalcValues(this->cursValues, this->yT, this->Sost, this->Sregr, this->Sfull, this->values, this->coefficients);
    // Заполнение вычисляемой таблицы
    fillCalculateTable(ui->calculate_tableView, this->numericDates, this->cursValues, this->yT, this->values);
    // --- Отображение значений величин вне вычисляемой таблицы --- //
    // Уравнение
    ui->equation_label->setText(this->trendLabel);
    // Корреляция
    ui->correlation_determination_textEdit->append("r1=" + QString::number(this->coefficients["r1"]));
    ui->correlation_determination_textEdit->setText(ui->correlation_determination_textEdit->toPlainText() +
                                                    ";r2=" + QString::number(this->coefficients["r2"]));
    ui->correlation_determination_textEdit->setText(ui->correlation_determination_textEdit->toPlainText() +
                                                    ";r1=r2? " + QString(correlation_flag ? "True" : "False"));
    ui->correlation_determination_textEdit->append("Значение линейного коэффициента корреляции: r=" + QString::number(this->coefficients["r1"]));
    ui->correlation_determination_textEdit->append(getRegressionRelationship(this->coefficients["r1"]));
    // Коэффииенты
    ui->coefficients_textEdit->append("A =" + QString::number(this->coefficients["A"]) + "\t" + "B =" + QString::number(this->coefficients["B"]));
    ui->coefficients_textEdit->append("A0=" + QString::number(this->coefficients["A0"]) + "\t" + "B0=" + QString::number(this->coefficients["B0"]));
    ui->coefficients_textEdit->append("A1=" + QString::number(this->coefficients["A1"]) + "\t" + "B1=" + QString::number(this->coefficients["B1"]));
    ui->coefficients_textEdit->append("a0=" + QString::number(this->coefficients["a0"]) + "\t" + "b0=" + QString::number(this->coefficients["b0"]));
    ui->coefficients_textEdit->append("a1=" + QString::number(this->coefficients["a1"]) + "\t" + "b1=" + QString::number(this->coefficients["b1"]));
    // Величины
    ui->R2_label->setText(ui->R2_label->text() + QString::number(this->coefficients["R2"]));
    ui->descr_R2_label->setText(getDeterminationDescription(this->coefficients["R2"]));
    ui->Sost_label->setText(ui->Sost_label->text() + QString::number(this->values.sumOst));
    ui->Sregr_label->setText(ui->Sregr_label->text() + QString::number(this->values.sumRegr));
    ui->Sfull_label->setText(ui->Sfull_label->text() + QString::number(this->values.sumFull));
    ui->n_label->setText(ui->n_label->text() + QString::number(this->values.n));
    ui->check_Sfull_label->setText("Sполн.=Sрегр.+Sост.? " + QString(Sfull_flag ? "True" : "False"));
    ui->MSE_label->setText(ui->MSE_label->text() + QString::number(this->coefficients["MSE"]));
    ui->calc_meanY_label->setText(ui->calc_meanY_label->text() + QString::number(this->values.meanY));
    ui->Sx2_label->setText(ui->Sx2_label->text() + QString::number(this->values.Sx2, 'g', 6));
    ui->Sy2_label->setText(ui->Sy2_label->text() + QString::number(this->values.Sy2, 'g', 6));
    ui->meanSx_label->setText(ui->meanSx_label->text() + QString::number(this->values.meanSx, 'g', 6));
    ui->meanSy_label->setText(ui->meanSy_label->text() + QString::number(this->values.meanSy, 'g', 6));

    // ---- ГРАФИК ---- //
    // Отрисовка графика
    WorkplaceForm::MakePlot();
}

void WorkplaceForm::MakePlot()
{
    // Проверка данных
    if (this->dataColumn.isEmpty() || cursValues.isEmpty() || yT.isEmpty())
        return;

    // Очистка графика
    ui->customPlot->clearGraphs();

    // Подготовка векторов
    int n = this->dataColumn.size();
    QVector<double> x(n), y(n), yReg(n);
    QVector<QDateTime> dateTimes(n);

    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumn[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = dateTimes[i].toSecsSinceEpoch(); // координата X
        y[i] = cursValues[i];                   // исходные данные
        yReg[i] = yT[i];                        // регрессия
    }

    // ----------------- График исходных данных -----------------
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // только точки
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 6));
    ui->customPlot->graph(0)->setName("Исходные данные");

    // ----------------- Линия тренда -----------------
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(x, yReg);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red, 2));

    // ----------------- Подпись линии тренда -----------------
    this->trendLabel += QString("\nR² = %1").arg(coefficients["R2"], 0, 'f', 4);
    ui->customPlot->graph(1)->setName(this->trendLabel);

    // ----------------- Оси -----------------
    ui->customPlot->xAxis->setLabel("Дата");
    ui->customPlot->yAxis->setLabel("Курс доллара к рублю");

    // Стиль осей
    QFont axisFont;
    axisFont.setBold(true);
    axisFont.setPointSize(10);
    ui->customPlot->xAxis->setLabelFont(axisFont);
    ui->customPlot->yAxis->setLabelFont(axisFont);

    // Ось X — формат даты
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy");
    dateTicker->setTickCount(10);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    ui->customPlot->xAxis->setTicker(dateTicker);
    ui->customPlot->xAxis->setTickLabelRotation(45); // поворот меток
    ui->customPlot->xAxis->setSubTicks(true);

    // Ось Y — шаг делений
    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    double yStep = ( *std::max_element(y.constBegin(), y.constEnd()) -
                    *std::min_element(y.constBegin(), y.constEnd()) ) / 10.0;
    yTicker->setTickStep(yStep);
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
    ui->customPlot->yAxis->setTicker(yTicker);
    ui->customPlot->yAxis->setSubTicks(true);

    // Диапазоны осей
    ui->customPlot->xAxis->setRange(x.first(), x.last());
    double minY = *std::min_element(y.constBegin(), y.constEnd());
    double maxY = *std::max_element(y.constBegin(), y.constEnd());
    ui->customPlot->yAxis->setRange(minY - yStep, maxY + yStep);

    // ----------------- Масштабирование и drag -----------------
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);
    ui->customPlot->setInteraction(QCP::iSelectPlottables, true);

    // ----------------- Заголовок -----------------
    QDateTime minDate = *std::min_element(dateTimes.constBegin(), dateTimes.constEnd());
    QDateTime maxDate = *std::max_element(dateTimes.constBegin(), dateTimes.constEnd());
    QString title = QString("Курс доллара США с %1 по %2")
                        .arg(minDate.date().toString("dd.MM.yyyy"))
                        .arg(maxDate.date().toString("dd.MM.yyyy"));
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, title, QFont("Arial", 12, QFont::Bold)));

    // ----------------- Легенда -----------------
    QFont legendFont = ui->customPlot->legend->font();
    legendFont.setPointSize(8);
    legendFont.setWeight(QFont::Normal);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
    ui->customPlot->legend->setBorderPen(QPen(QColor(150, 150, 150, 180)));

    // Отрисовка
    ui->customPlot->replot();
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
