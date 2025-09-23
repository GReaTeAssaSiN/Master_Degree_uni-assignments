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

    // ----- Сигналы ----- //
    connect(ui->pushButton_back, &QPushButton::clicked, this, &WorkplaceForm::on_pushButton_back_clicked);

    // ----- Выделение текста на label'ах ----- //
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel* lbl : labels) {
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }

    // ----- ПРЕДОБРАБОТКА ДАННЫХ ----- //
    // Чтение исходных данных и преобразование к нужному виду
    readDataAndCurs(data_tableView, this->dataColumn, this->numericDates, this->cursValues);

    // ----- ОБЩАЯ ТАБЛИЦА ----- //
    // Вычисление значений квадратов, сумм и средних значений + количества экспериментов (n)
    calculateRegressionTotalValues(this->numericDates, this->cursValues, this->xSquared, this->ySquared, this->xyProduct, this->values);
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

    // ----- ВЫЧИСЛЯЕМАЯ ТАБЛИЦА ----- //
    // Вычиление значений констант и коэффициентов регрессии + построение уравнения регрессии
    bool correlation_flag{};
    switch (mode) {
    case 0: // Линейная
        setWindowTitle("Линейная регрессия");
        correlation_flag = calculateLinearRegressionCoefficients(this->values, this->coefficients);
        calculateLinearRegressionValues(this->numericDates, this->yT, this->values.n, this->coefficients);
        break;
    case 1: // Обратная линейная
        setWindowTitle("Обратная линейная регрессия");
        break;
    case 2: // Экспоненциальная
        setWindowTitle("Экспонениальная регрессия");
        break;
    case 3: // Гиперболическая
        setWindowTitle("Гиперболическая регрессия");
        break;
    case 4: // Параболическая
        setWindowTitle("Параболическая регрессия");
        break;
    case 5: // Логарифмическая
        setWindowTitle("Логарифмическая регрессия");
        break;
    case 6: // Степенная
        setWindowTitle("Степенная регрессия");
        break;
    case 7: // Полиномиальная
        setWindowTitle("Полиномиальная регрессия");
        break;
    default:
        setWindowTitle("Неопознанный тип регрессии");
        QMessageBox::warning(nullptr, "Предупреждение", "Неизвестный тип регрессии, не удалось вычислить коэффициенты!");
        break;
    }
    // Вычисление Sост., Sрегр., Sполн. + R2, MSE + Sx2, Sy2, meanSx, meanSy
    bool Sfull_flag = calculateRegressionCalcValues(this->cursValues, this->yT, this->Sost, this->Sregr, this->Sfull, this->values, this->coefficients);
    // Заполнение вычисляемой таблицы
    fillCalculateTable(ui->calculate_tableView, this->dataColumn, this->numericDates, this->cursValues, this->yT, this->values);
    // --- Отображение значений величин вне вычисляемой таблицы --- //
    // Уравнение
    this->trend_eq = QString("y = %1 + %2*x").arg(coefficients["a0"], 0, 'g', 6).arg(coefficients["a1"], 0, 'f', 2);
    ui->equation_label->setText(trend_eq);
    // Корреляция
    if (!correlation_flag)
        QMessageBox::warning(this, "Предупреждение", "Коэффициент корреляции линейной и обратной регрессии не совпадает (r1 != r2).");
    ui->correlation_determination_textEdit->append(getRegressionRelationship(this->coefficients["r1"]));
    // Коэффииенты
    ui->coefficients_textEdit->append("Вспомогательные величины:");
    ui->coefficients_textEdit->append("A =" + QString::number(this->coefficients["A"], 'g', 6) + "\t" + "B =" + QString::number(this->coefficients["B"], 'g', 6));
    ui->coefficients_textEdit->append("A0=" + QString::number(this->coefficients["A0"], 'g', 6) + "\t" + "B0=" + QString::number(this->coefficients["B0"], 'g', 6));
    ui->coefficients_textEdit->append("A1=" + QString::number(this->coefficients["A1"], 'g', 6) + "\t" + "B1=" + QString::number(this->coefficients["B1"], 'g', 6));
    ui->coefficients_textEdit->append("\nКоэффициенты:");
    ui->coefficients_textEdit->append("a0=" + QString::number(this->coefficients["a0"], 'g', 6) + "\t" + "b0=" + QString::number(this->coefficients["b0"], 'g', 6));
    ui->coefficients_textEdit->append("a1=" + QString::number(this->coefficients["a1"], 'g', 6) + "\t" + "b1=" + QString::number(this->coefficients["b1"], 'g', 6));
    // Величины
    ui->R2_label->setText(ui->R2_label->text() + QString::number(this->coefficients["R2"], 'g', 6));
    ui->descr_R2_label->setText(getDeterminationDescription(this->coefficients["R2"]));
    ui->Sost_label->setText(ui->Sost_label->text() + QString::number(this->values.sumOst, 'g', 6));
    ui->Sregr_label->setText(ui->Sregr_label->text() + QString::number(this->values.sumRegr, 'g', 6));
    ui->Sfull_label->setText(ui->Sfull_label->text() + QString::number(this->values.sumFull, 'g', 6));
    ui->n_label->setText(ui->n_label->text() + QString::number(this->values.n));
    if (!Sfull_flag)
        QMessageBox::warning(this, "Предупреждение", "Sполн. != Sрегр. + Sост.");
    ui->MSE_label->setText(ui->MSE_label->text() + QString::number(this->coefficients["MSE"], 'g', 6));
    ui->Sx2_label->setText(ui->Sx2_label->text() + QString::number(this->values.Sx2, 'g', 6));
    ui->Sy2_label->setText(ui->Sy2_label->text() + QString::number(this->values.Sy2, 'g', 6));
    ui->meanSx_label->setText(ui->meanSx_label->text() + QString::number(this->values.meanSx, 'g', 6));
    ui->meanSy_label->setText(ui->meanSy_label->text() + QString::number(this->values.meanSy, 'g', 6));

    // ---- ГРАФИК ---- //
    // Отрисовка графика
    WorkplaceForm::MakePlot();

    // ---- УСТАНОВКА ДАТЫ ---- //
    this->default_date = QDate::fromString(this->dataColumn.first(), "dd.MM.yyyy").addDays(1);
    ui->selectDate_dateEdit->setDate(this->default_date);
    ui->selectDate_dateEdit->setMinimumDate(this->default_date);
}

void WorkplaceForm::MakePlot()
{
    // Проверка данных
    if (this->dataColumn.isEmpty() || this->cursValues.isEmpty() || this->yT.isEmpty())
        return;

    // Очистка графика
    ui->customPlot->clearGraphs();
    ui->forecastCurs_label->setText("Прогноз курса Доллара США: не определен.");

    // Подготовка векторов
    int n = this->values.n;
    QVector<double> x(n), y(n), yReg(n);
    QVector<QDateTime> dateTimes(n);

    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumn[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = dateTimes[i].toSecsSinceEpoch(); // координата X
        y[i] = this->cursValues[i];             // исходные данные
        yReg[i] = this->yT[i];                  // регрессия
    }

    // Определение минимальных и максимальных значений даты
    QDateTime minDate = *std::min_element(dateTimes.constBegin(), dateTimes.constEnd());
    QDateTime maxDate = *std::max_element(dateTimes.constBegin(), dateTimes.constEnd());

    // ----------------- Дополнение графика по выбранной дате (будущей) ----------------- //
    QVector<double> temp_x = x;
    int temp_n = n;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = this->yT;

    if (this->select_date.isValid()){
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения
        while (lastDate < this->select_date){
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);
            temp_n++;
            // Значение даты в секундах для оси абсцисс (по убыванию идут)
            temp_x.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут)
            temp_numericDates.prepend(epoch.daysTo(lastDate)/10000.0);
        }
        calculateLinearRegressionValues(temp_numericDates, temp_yReg, temp_n, this->coefficients);
        // minDate остается той же, maxDate изменилась, minY и maxY могли измениться
        maxDate = QDateTime(lastDate, QTime(0,0));
    }

    // ----------------- График исходных данных ----------------- //
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph(0)->setPen(QPen(Qt::transparent));
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(Qt::blue), 6));
    ui->customPlot->graph(0)->setName("Эксперементальные данные");

    // ----------------- Линия тренда ----------------- //
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(temp_x, temp_yReg);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red, 2));
    ui->customPlot->graph(1)->setName(this->trend_eq + QString("\nR² = %3").arg(coefficients["R2"], 0, 'f', 6));

    // ----------------- Выделение выбранной даты и соответствующего курса ----------------- //
    if (this->select_date.isValid())
    {
        // Отрисовка
        double xPos = QDateTime(this->select_date, QTime(0,0)).toSecsSinceEpoch();
        double yPos = temp_yReg.first();
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setData({xPos}, {yPos});
        ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
        ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(2)->setName("Прогноз");
        // Надпись
        ui->forecastCurs_label->setText("Прогноз курса Доллара США: " + QString::number(yPos, 'f', 2) + " руб.");
    }

    // ----------------- Подсветка графика ----------------- //
    // Для графика эксперементальных данных
    QCPSelectionDecorator *decor0 = new QCPSelectionDecorator;
    decor0->setPen(QPen(Qt::green, 2));
    ui->customPlot->graph(0)->setSelectionDecorator(decor0);

    // Для графика линии тренда
    QCPSelectionDecorator *decor1 = new QCPSelectionDecorator;
    decor1->setPen(QPen(Qt::green, 2));
    ui->customPlot->graph(1)->setSelectionDecorator(decor1);

    // Для графика прогноза
    if (this->select_date.isValid()){
        QCPSelectionDecorator *decor2 = new QCPSelectionDecorator;
        decor2->setPen(QPen(Qt::green, 2));
        ui->customPlot->graph(2)->setSelectionDecorator(decor2);
    }

    // ----------------- Оси ----------------- //
    ui->customPlot->xAxis->setLabel("Дата");
    ui->customPlot->yAxis->setLabel("Курс Доллара США к рублю");

    // Стиль осей
    // Подписи
    QFont axisFont;
    axisFont.setBold(true);
    axisFont.setPointSize(10);
    ui->customPlot->xAxis->setLabelFont(axisFont);
    ui->customPlot->yAxis->setLabelFont(axisFont);
    // Тики
    QFont tickFont;
    tickFont.setItalic(true);
    tickFont.setPointSize(8);
    ui->customPlot->xAxis->setTickLabelFont(tickFont);
    ui->customPlot->yAxis->setTickLabelFont(tickFont);

    // Ось X - формат делений
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy");
    dateTicker->setTickCount(10);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    ui->customPlot->xAxis->setTicker(dateTicker);
    ui->customPlot->xAxis->setTickLabelRotation(30);
    ui->customPlot->xAxis->setSubTicks(true);

    // Ось Y — формат делений
    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setTickStep(1.0);
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->customPlot->yAxis->setTicker(yTicker);
    ui->customPlot->yAxis->setSubTicks(true);
    connect(ui->customPlot->yAxis,
            static_cast<void (QCPAxis::*)(const QCPRange &)>(&QCPAxis::rangeChanged),
            this,
            [=](const QCPRange &newRange){
                // Текущий масштаб
                double range = newRange.size();

                // * Масштабирование шага * //
                // Примерный шаг делений оси
                double roughStep = range / 20.0;
                // Представим: roughStep = fraction * 10^exponent ([что-то между 1 и 10] * 10^n)
                double exponent = std::floor(std::log10(roughStep));
                double fraction = roughStep / std::pow(10, exponent);
                double niceFraction{};

                // Красивый Fraction
                if (fraction <= 1.0) niceFraction = 1.0;
                else if (fraction <= 2.0) niceFraction = 2.0;
                else if (fraction <= 5.0) niceFraction = 5.0;
                else niceFraction = 10.0;

                // Полученный шаг
                double step = niceFraction * std::pow(10, exponent);

                // Установка шага
                yTicker->setTickStep(step);
                ui->customPlot->yAxis->setTicker(yTicker);
            });

    // Диапазоны осей
    ui->customPlot->rescaleAxes(); // Автоматическое изменение диапазона осей
    auto xRange = ui->customPlot->xAxis->range();
    auto yRange = ui->customPlot->yAxis->range();
    ui->customPlot->xAxis->setRange(xRange.lower - 86400, xRange.upper + 86400); // 1 день отступ
    ui->customPlot->yAxis->setRange(yRange.lower - 0.5, yRange.upper + 0.5);     // 0.5 отступ

    // ----------------- Масштабирование и drag ----------------- //
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);        // Перетаскивание мышью
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);        // Масштаб колесиком
    ui->customPlot->setInteraction(QCP::iSelectPlottables, true); // Подсветка графика

    // ----------------- Заголовок ----------------- //
    QString title = QString("Курс Доллара США с %1 по %2")
                        .arg(minDate.date().toString("dd.MM.yyyy"))
                        .arg(maxDate.date().toString("dd.MM.yyyy"));
    // Паддинг от краев графика
    ui->customPlot->axisRect()->setMargins(QMargins(20, 20, 20, 20));

    // Проверка на существование заголовка в окне. Если нет - добавление, иначе - замена текста.
    if (!ui->customPlot->plotLayout()->elementCount() ||
        !qobject_cast<QCPTextElement*>(ui->customPlot->plotLayout()->elementAt(0)))
    {
        // Установка названия графика
        ui->customPlot->plotLayout()->insertRow(0);
        QCPTextElement *titleElement = new QCPTextElement(ui->customPlot, title, QFont("Arial", 12, QFont::Bold));
        titleElement->setMargins(QMargins(0, 10, 0, 0));
        ui->customPlot->plotLayout()->addElement(0, 0, titleElement);
    }
    else
    {
        auto header = qobject_cast<QCPTextElement*>(ui->customPlot->plotLayout()->elementAt(0));
        header->setText(title);
    }

    // ----------------- Легенда ----------------- //
    QFont legendFont = ui->customPlot->legend->font();
    legendFont.setPointSize(8);// Кегль
    legendFont.setWeight(QFont::Normal); // Вес
    ui->customPlot->legend->setFont(legendFont); // Установка шрифта
    ui->customPlot->legend->setVisible(true); // Видимость легенды
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150)); // Прозрачность фона
    ui->customPlot->legend->setBorderPen(QPen(QColor(150, 150, 150, 180))); // Границы
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // Установка в левый верхний угол

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

void WorkplaceForm::on_calculate_pushButton_clicked()
{
    this->select_date = ui->selectDate_dateEdit->date();
    WorkplaceForm::MakePlot();
}

void WorkplaceForm::on_clean_pushButton_clicked()
{
    this->select_date = QDate();
    ui->selectDate_dateEdit->setDate(this->default_date);
    WorkplaceForm::MakePlot();
}
