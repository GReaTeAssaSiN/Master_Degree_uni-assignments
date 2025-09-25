#include "workplaceform.h"
#include "ui_workplaceform.h"
#include "QCloseEvent"
#include "myfuncs.h"
#include <QMessageBox>

WorkplaceForm::WorkplaceForm(const int &mode, const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                             QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkplaceForm)
    , mode(mode)
    , dataColumns(dataColumns)
    , numericDates(numericDates)
    , cursValues(cursValues)
{
    ui->setupUi(this);

    // ----- Сигналы ----- //
    // Возврат на основное окно
    connect(ui->pushButton_back, &QPushButton::clicked, this, &WorkplaceForm::on_pushButton_back_clicked);

    // ----- Выделение текста на label'ах для удобства ----- //
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel* lbl : labels) {
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }

    // ----- ОБЩАЯ ТАБЛИЦА ----- //
    // Вычисление значений квадратов, сумм и средних значений + количества экспериментов (n)
    calculateRegressionTotalValues(this->numericDates, this->cursValues, this->n, this->vector_values, this->values);
    // Заполнение общей таблицы
    fillTotalTable(ui->total_tableView, this->mode, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);

    // ----- ВНЕ ОБЩЕЙ ТАБЛИЦЫ ----- //
    ui->totalTable_info_textEdit->append(((mode != 1) ? "∑x=" : "∑y=") + QString::number((mode != 1) ? this->values["sumX"] : this->values["sumY"], 'f', 5));
    ui->totalTable_info_textEdit->append(((mode != 1) ? "∑y=" : "∑x=") + QString::number((mode != 1) ? this->values["sumY"] : this->values["sumX"], 'f', 5));
    ui->totalTable_info_textEdit->append(((mode != 1) ? "∑x\u00B2=" : "∑y\u00B2=") + QString::number((mode != 1) ? this->values["sumX2"] : this->values["sumY2"], 'f', 5));
    ui->totalTable_info_textEdit->append(((mode != 1) ? "∑y\u00B2=" : "∑x\u00B2=") + QString::number((mode != 1) ? this->values["sumY2"] : this->values["sumX2"], 'f', 5));
    ui->totalTable_info_textEdit->append("∑xy=" + QString::number(this->values["sumXY"], 'f', 5));
    ui->meanX_label->setText(((mode != 1) ? "x\u0304=" : "y\u0304=") + QString::number((mode != 1) ? this->values["meanX"] : this->values["meanY"], 'f', 5));
    ui->meanY_label->setText(((mode != 1) ? "y\u0304=" : "x\u0304=") + QString::number((mode != 1) ? this->values["meanY"] : this->values["meanX"], 'f', 5));

    // ----- ВЫЧИСЛЕНИЯ ----- //
    // Флаги для проверки корректности корреляции и Sполн. для коэффициента детерминации
    bool correlation_flag{}, Sfull_flag{};
    // Вычиление коэффициентов регрессии и значений
    switch (mode) { // Регрессии
    case 0: // Линейная
        setWindowTitle("Линейная регрессия");
        correlation_flag = calculateLinearRegressionCoefficients(this->n, this->values, this->coefficients);
        calculateLinearRegressionValues(this->mode, this->numericDates, this->n, this->vector_values["yT"], this->coefficients);
        // Корреляция
        if (!correlation_flag)
            QMessageBox::warning(this, "Предупреждение", "Коэффициент корреляции линейной и обратной линейной регрессии не совпадает (r1 != r2).");
        // --- ВЫВОД ВЫЧИСЛЕННЫХ ЗНАЧЕНИЙ --- //
        // Корреляция
        ui->correlation_textEdit->append(getRegressionRelationship(mode, this->coefficients["r1"]));
        // Коэффициенты
        ui->coefficients_textEdit->append("<b>Вспомогательные величины:</b>");
        ui->coefficients_textEdit->append("A = " + QString::number(this->coefficients["A"], 'f', 5));
        ui->coefficients_textEdit->append("A\u2080 = " + QString::number(this->coefficients["A0"], 'f', 5));
        ui->coefficients_textEdit->append("A\u2081 = " + QString::number(this->coefficients["A1"], 'f', 5));
        ui->coefficients_textEdit->append("<br>B = " + QString::number(this->coefficients["B"], 'f', 5));
        ui->coefficients_textEdit->append("B\u2080 = " + QString::number(this->coefficients["B0"], 'f', 5));
        ui->coefficients_textEdit->append("B\u2081 = " + QString::number(this->coefficients["B1"], 'f', 5));
        ui->coefficients_textEdit->append("<br><b>Коэффициенты:</b>");
        ui->coefficients_textEdit->append("a\u2080 = " + QString::number(this->coefficients["a0"], 'f', 5));
        ui->coefficients_textEdit->append("a\u2081 = " + QString::number(this->coefficients["a1"], 'f', 5));
        ui->coefficients_textEdit->append("b\u2080 = " + QString::number(this->coefficients["b0"], 'f', 5));
        ui->coefficients_textEdit->append("b\u2081 = " + QString::number(this->coefficients["b1"], 'f', 5));
        // Уравнение регрессии
        this->trend_eq = QString("y = %1 + %2 \u00B7 x").arg(coefficients["a0"], 0, 'g', 6).arg(coefficients["a1"], 0, 'f', 2);
        break;
    case 1: // Обратная линейная
        setWindowTitle("Обратная линейная регрессия");
        correlation_flag = calculateLinearRegressionCoefficients(this->n, this->values, this->coefficients);
        calculateLinearRegressionValues(this->mode, this->cursValues, this->n, this->vector_values["xT"], this->coefficients);
        // Корреляция
        if (!correlation_flag)
            QMessageBox::warning(this, "Предупреждение", "Коэффициент корреляции обратной линейной и линейной регрессии не совпадает (r1 != r2).");
        // --- ВЫВОД ВЫЧИСЛЕННЫХ ЗНАЧЕНИЙ --- //
        // Корреляция
        ui->correlation_textEdit->append(getRegressionRelationship(mode, this->coefficients["r1"]));
        // Коэффициенты
        ui->coefficients_textEdit->append("<b>Вспомогательные величины:</b>");
        ui->coefficients_textEdit->append("A = " + QString::number(this->coefficients["A"], 'f', 5));
        ui->coefficients_textEdit->append("A\u2080 = " + QString::number(this->coefficients["A0"], 'f', 5));
        ui->coefficients_textEdit->append("A\u2081 = " + QString::number(this->coefficients["A1"], 'f', 5));
        ui->coefficients_textEdit->append("<br>B = " + QString::number(this->coefficients["B"], 'f', 5));
        ui->coefficients_textEdit->append("B\u2080 = " + QString::number(this->coefficients["B0"], 'f', 5));
        ui->coefficients_textEdit->append("B\u2081 = " + QString::number(this->coefficients["B1"], 'f', 5));
        ui->coefficients_textEdit->append("<br><b>Коэффициенты:</b>");
        ui->coefficients_textEdit->append("a\u2080 = " + QString::number(this->coefficients["a0"], 'f', 5));
        ui->coefficients_textEdit->append("a\u2081 = " + QString::number(this->coefficients["a1"], 'f', 5));
        ui->coefficients_textEdit->append("b\u2080 = " + QString::number(this->coefficients["b0"], 'f', 5));
        ui->coefficients_textEdit->append("b\u2081 = " + QString::number(this->coefficients["b1"], 'f', 5));
        // Уравнение регрессии
        this->trend_eq = QString("x = %1 + %2 \u00B7 y").arg(coefficients["b0"], 0, 'g', 6).arg(coefficients["b1"], 0, 'g', 2);
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
    // Вычисление Sост., Sрегр., Sполн. + R2, а также  MSE + Sx2, Sy2, meanSx, meanSy
    if (mode != 1) Sfull_flag = calculateRegressionCalcValues(this->mode, this->n, this->cursValues, this->vector_values, this->values, this->coefficients);
    else Sfull_flag = calculateRegressionCalcValues(this->mode, this->n, this->numericDates, this->vector_values, this->values, this->coefficients);
    if (!Sfull_flag)
        QMessageBox::warning(this, "Предупреждение", "Sполн. != Sрегр. + Sост.");

    // ----- ВЫЧИСЛЯЕМАЯ ТАБЛИЦА ----- //
    // Заполнение вычисляемой таблицы
    fillCalculateTable(ui->calculate_tableView, mode, this->dataColumns, this->numericDates, this->cursValues, this->vector_values, this->values);

    // ----- ВНЕ ВЫЧИСЛЯЕМОЙ ТАБЛИЦЫ ----- //
    // Уравнение регрессии
    ui->equation_label->setText(trend_eq);
    // Величины
    ui->n_label->setText("n=" + QString::number(this->n));
    ui->MSE_label->setText("MSE=" + QString::number(this->coefficients["MSE"], 'g', 5));
    ui->R2_label->setText("R\u00B2=" + QString::number(this->coefficients["R2"], 'g', 5));
    ui->descr_R2_label->setText(getDeterminationDescription(this->coefficients["R2"]));
    ui->Sx2_label->setText(((mode != 1) ? "(S<sub>x</sub>)\u00B2=" : "(S<sub>y</sub>)\u00B2=") +
                           QString::number((mode != 1) ? this->values["Sx2"] : this->values["Sy2"], 'g', 5));
    ui->Sy2_label->setText(((mode != 1) ? "(S<sub>y</sub>)\u00B2=" : "(S<sub>x</sub>)\u00B2=") +
                           QString::number((mode != 1) ? this->values["Sy2"] : this->values["Sx2"], 'g', 5));
    ui->meanSx_label->setText(((mode != 1) ? "S<sub>x\u0304</sub>=" : "S<sub>y\u0304</sub>=") +
                              QString::number((mode != 1) ? this->values["meanSx"] : this->values["meanSy"], 'g', 5));
    ui->meanSy_label->setText(((mode != 1) ? "S<sub>y\u0304</sub>=" : "S<sub>x\u0304</sub>=") +
                              QString::number((mode != 1) ? this->values["meanSy"] : this->values["meanSx"], 'g', 5));
    ui->Sregr_label->setText("S<sub>регр.</sub>=" + QString::number(this->values["sumRegr"], 'g', 5));
    ui->Sost_label->setText("S<sub>ост.</sub>=" + QString::number(this->values["sumOst"], 'g', 5));
    ui->Sfull_label->setText("S<sub>полн.</sub>=" + QString::number(this->values["sumFull"], 'g', 5));

    // ----- УСТАНОВКА ВЫБИРАЕМОЙ ДАТЫ ----- //
    this->default_date = QDate::fromString(this->dataColumns.first(), "dd.MM.yyyy").addDays(1);
    ui->selectDate_dateEdit->setDate(this->default_date);
    ui->selectDate_dateEdit->setMinimumDate(this->default_date);

    // ----- ГРАФИК ----- //
    // Отрисовка графика
    (mode != 1) ? WorkplaceForm::MakePlot() : WorkplaceForm::MakeInversePlot();
}

void WorkplaceForm::MakePlot()
{
    // Проверка данных
    if (this->dataColumns.isEmpty() || this->cursValues.isEmpty() || this->vector_values["yT"].isEmpty())
        return;

    // Очистка графика
    ui->customPlot->clearGraphs();
    ui->forecastCurs_label->setText("Прогноз курса Доллара США: не определен.");

    // Подготовка векторов
    int n = this->n;
    QVector<double> x(n), y(n), yReg(n);
    QVector<QDateTime> dateTimes(n);

    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumns[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = dateTimes[i].toSecsSinceEpoch(); // ось абсисс
        y[i] = this->cursValues[i];             // ось ординат (эксперементальные)
        yReg[i] = this->vector_values["yT"][i]; // ось ординат (регрессия)
    }

    // Определение минимальных и максимальных значений даты
    QDateTime minDate = *std::min_element(dateTimes.constBegin(), dateTimes.constEnd());
    QDateTime maxDate = *std::max_element(dateTimes.constBegin(), dateTimes.constEnd());

    // ----------------- Дополнение графика по выбранной дате (будущей) ----------------- //
    QVector<double> temp_x = x;
    int temp_n = n;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = yReg;

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
        calculateLinearRegressionValues(this->mode, temp_numericDates, temp_n, temp_yReg, this->coefficients);
        // minDate остается той же, maxDate изменилась
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

void WorkplaceForm::MakeInversePlot()
{
    // Проверка данных
    if (this->dataColumns.isEmpty() || this->cursValues.isEmpty() || this->vector_values["xT"].isEmpty())
        return;

    // Очистка графика
    ui->customPlot->clearGraphs();
    ui->forecastCurs_label->setText("Прогноз курса Доллара США: не определен.");

    // Подготовка векторов
    int n = this->n;
    QVector<double> x(n), y(n), yReg(n);
    QVector<QDateTime> dateTimes(n);

    for (int i = 0; i < n; ++i)
    {
        QDate date = QDate::fromString(this->dataColumns[i], "dd.MM.yyyy");
        dateTimes[i] = QDateTime(date, QTime(0,0));

        x[i] = this->cursValues[i];                              // ось абсисс
        y[i] = dateTimes[i].toSecsSinceEpoch();                  // ось ординат (экспереминатльные)
        yReg[i] = (this->vector_values["xT"][i])*10000*24*60*60; // ось ординат (регрессия) [Кол-во дней/10000] * 10000 * 24 * 60 * 60
    }

    // Определение минимальных и максимальных значений даты
    QDateTime minDate = *std::min_element(dateTimes.constBegin(), dateTimes.constEnd());
    QDateTime maxDate = *std::max_element(dateTimes.constBegin(), dateTimes.constEnd());

    // ----------------- Дополнение графика по выбранной дате (будущей) ----------------- //
    QVector<double> temp_x = x;
    int temp_n = n;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = yReg;

    qDebug() << "WAS:";
    for (int i=0; i < n; ++i)
        qDebug() << temp_x[i] << temp_yReg[i];
    if (this->select_date.isValid()){
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения
        while (lastDate < this->select_date){
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);
            temp_n++;
            // Значение даты в секундах для оси ординат (по убыванию идут)
            temp_yReg.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут)
            temp_numericDates.prepend(epoch.daysTo(lastDate)/10000.0);
        }
        // Решение уравнения [x = b0 + b1 * y] относительно неизвестной y (курса доллара), т.к. хотим предсказывать курс по дате, а не наоборот.
        // В этой функции полагается регрессия вида (переобозначение относительно программных переменных):
        //   y = b0 + b1 * x, где x - курс доллара США, y - предсказанная дата, уравнение которой решается относительно x.
        // Иными словами, уравнение решается не относительно y (как положено по обратной регрессии), а относительно x,
        //т.е. мы предсказываем не дату по курсу доллара США, а как будто-бы по уже предсказанной дате моделируем курс доллара США.
        calculateInverseLinearRegressionValuesByDates(temp_yReg, temp_x, temp_n, this->coefficients);
        // minDate остается той же, maxDate изменилась
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
        double xPos = temp_x.first();
        double yPos = QDateTime(this->select_date, QTime(0,0)).toSecsSinceEpoch();
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setData({xPos}, {yPos});
        ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
        ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(2)->setName("Прогноз");
        // Надпись
        ui->forecastCurs_label->setText("Прогноз курса Доллара США: " + QString::number(xPos, 'f', 2) + " руб.");
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
    ui->customPlot->xAxis->setLabel("Курс Доллара США к рублю");
    ui->customPlot->yAxis->setLabel("Дата");

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
    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
    xTicker->setTickStep(1.0);
    xTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->customPlot->xAxis->setTicker(xTicker);
    ui->customPlot->xAxis->setSubTicks(true);
    connect(ui->customPlot->xAxis,
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
                xTicker->setTickStep(step);
                ui->customPlot->xAxis->setTicker(xTicker);
            });

    // Ось Y — формат делений
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("dd.MM.yyyy");
    dateTicker->setTickCount(10);
    dateTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    ui->customPlot->yAxis->setTicker(dateTicker);
    ui->customPlot->yAxis->setTickLabelRotation(30);
    ui->customPlot->yAxis->setSubTicks(true);

    // Диапазоны осей
    ui->customPlot->rescaleAxes(); // Автоматическое изменение диапазона осей
    auto xRange = ui->customPlot->xAxis->range();
    auto yRange = ui->customPlot->yAxis->range();
    ui->customPlot->xAxis->setRange(xRange.lower - 0.5, xRange.upper + 0.5);     // 0.5 отступ
    ui->customPlot->yAxis->setRange(yRange.lower - 86400, yRange.upper + 86400); // 1 день отступ

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
    (this->mode != 1) ? WorkplaceForm::MakePlot() : WorkplaceForm::MakeInversePlot();
}

void WorkplaceForm::on_clean_pushButton_clicked()
{
    this->select_date = QDate();
    ui->selectDate_dateEdit->setDate(this->default_date);
    (this->mode != 1) ? WorkplaceForm::MakePlot() : WorkplaceForm::MakeInversePlot();
}
