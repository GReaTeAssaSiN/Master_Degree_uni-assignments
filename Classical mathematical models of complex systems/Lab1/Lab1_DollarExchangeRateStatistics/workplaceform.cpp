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
    this->showMaximized();

    // ----- Сигналы ----- //
    // Возврат на основное окно
    connect(ui->pushButton_back, &QPushButton::clicked, this, &WorkplaceForm::on_pushButton_back_clicked);

    // ----- Выделение текста на label'ах для удобства ----- //
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel* lbl : labels) {
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }

    // ----- РЕГРЕССИИ ----- //
    switch(this->mode){
    case 0:
    {
        setWindowTitle("Линейная регрессия");

        // y = a0 + a1*x
        //
        // Система:
        // {a0*n + a1*E(xi) = E(yi)
        // {a0*E(xi) + a1*E(xi^2) = E(xi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForLinearOrInverse(false, this->n, this->values, this->coefficients);
        // Подсчет величин регрессии
        calculateLinearOrInverseRegression(this->mode, this->numericDates, this->cursValues, this->vector_values["yT"],
                                           this->coefficients, this->values, this->n, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 1:
    {
        setWindowTitle("Обратная линейная регрессия");

        // x = b0 + b1*y
        //
        // Система:
        // {b0*n + b1*E(xi) = E(yi)
        // {b0*E(xi) + b1*E(xi^2) = E(xi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений сумм и средних значений (входные данные уже перевернуты)
        calculateRegressionsSums(this->mode, this->cursValues, this->numericDates, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы (входные данные уже перевернуты)
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->cursValues, this->numericDates, this->vector_values);
        // Заполнение текстового поля с суммами (входные данные уже перевернуты)
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForLinearOrInverse(true, this->n, this->values, this->coefficients);
        // Подсчет величин регрессии
        calculateLinearOrInverseRegression(this->mode, this->cursValues, this->numericDates, this->vector_values["xT"],
                                           this->coefficients, this->values, this->n, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 2:
    {
        setWindowTitle("Экспоненциальная регрессия");
        // y = a0*e^(a1*x)
        // ln(y) = ln(a0) + a1*x
        // ln(y) = b0 + b1*x
        //
        // Система:
        // {b0*n + b1*E(xi) = E(lnyi)
        // {b0*E(xi) + b1*E(xi^2) = E(xi*lnyi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForExponential(this->n, this->numericDates, this->vector_values["lny"], this->values, this->coefficients);
        // Подсчет величин регрессии
        calculateExponentialRegression(this->numericDates, this->vector_values["lny"], this->vector_values["lnyT"], this->vector_values["yT"],
                                       this->coefficients, this->values, this->n, this->mode, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 3:
    {
        setWindowTitle("Гиперболическая регрессия");
        // y = a0 + a1/x
        // y = a0 + a1*z
        //
        // Система:
        // {a0*n + a1*E(zi) = E(yi)
        // {a0*E(zi) + a1*E(zi^2) = E(zi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForHyperbolic(this->n, this->vector_values["z"], this->cursValues, this->values, this->coefficients);
        // Подсчет величин регрессии
        calculateHyperbolicRegression(this->vector_values["z"], this->cursValues, this->vector_values["yT"],
                                      this->coefficients, this->values, this->n, this->mode, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 4:
    {
        setWindowTitle("Параболическая регрессия");

        // y = a0 + a1*x + a2*x^2
        //
        // Система:
        // {a0*n + a1*E(xi) + a2*E(xi^2) = E(yi)
        // {a0*E(xi) + a1*E(xi^2) + a2*E(xi^3) = E(xi*yi)
        // {a0*E(xi^2) + a1*E(xi^3) + a2*E(xi^4) = E(xi^2*y)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 2;
        // Вычисление сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление коэффициентов
        if (!buildAndSolvePolynomRegression(this->values, this->coefficients, this->n, this->degree))
        {
            QMessageBox::critical(this, "Ошибка", QString("Не удалось построить параболическую регрессию: система уравнений вырождена или точек меньше 3."));
            return;
        }
        // Подсчет величин регрессии
        calculatePolynomRegression(this->degree, this->numericDates, this->cursValues, this->vector_values["yT"],
                                   this->coefficients, this->values, this->n, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 5:
    {
        setWindowTitle("Логарифмическая регрессия");
        // y = a0 + a1*lnx
        //
        // Система:
        // {a0*n + a1*E(lnxi) = E(yi)
        // {a0*E(lnxi) + a1*E(lnxi^2) = E(lnxi*yi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForLogarithmic(this->n, this->vector_values["lnx"], this->cursValues, this->values, this->coefficients);
        // Подсчет величин регрессии
        calculateLogarithmicRegression(this->vector_values["lnx"], this->cursValues, this->vector_values["yT"],
                                       this->coefficients, this->values, this->n, this->mode, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 6:
    {
        setWindowTitle("Степенная регрессия");
        // y = a0*x^(a1)
        // lny = lna0 + a1*lnx
        // lny = b0 + b1*lnx
        //
        // Система:
        // {b0*n + b1*E(lnxi) = E(lnyi)
        // {b0*E(lnxi) + b1*E(lnxi^2) = E(lnxi*lnyi)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = 1;
        // Вычисление значений сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление вспомогательных величин (метод Крамера, коэффициент Пирсона) и коэффициентов регрессии
        getCoefsForPower(this->n, this->vector_values["lnx"], this->vector_values["lny"], this->values, this->coefficients);
        // Подсчет величин регрессии
        calculatePowerRegression(this->vector_values["lnx"], this->vector_values["lny"], this->numericDates, this->vector_values["lnyT"], this->vector_values["yT"],
                                 this->coefficients, this->values, this->n, this->mode, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    case 7:
    {
        setWindowTitle("Полиномиальная регрессия");
        // Получение степени полинома
        bool ok;
        int degree = QInputDialog::getInt(
            this,
            "Степень полиномиальной регрессиии",
            "Укажите степень полинома (по умолчанию 3, максимум 10):",
            3, // по умолчанию
            3, // min
            10,// max
            1, // шаг
            &ok// нажата ли кнопка ОК
            );

        // y = a0 + a1*x + a2*x^2 + ... + an*x^n
        //
        // Система:
        // {a0*n       + a1*E(xi)       + a2*E(xi^2)     + ... + an*E(xi^n)     = E(yi)
        // {a0*E(xi)   + a1*E(xi^2)     + a2*E(xi^3)     + ... + an*E(xi*(n+1)) = E(xi*yi)
        // {a0*E(xi^2) + a1*E(xi^3)     + a2*E(xi^4)     + ... + an*E(xi*(n+2)  = E(xi^2*y)
        // {................................................................................
        // {a0*E(xi^n) + a1*E(xi^(n+1)) + a2*E(xi^(n+2)) + ... + an*E(xi*(n+n)) = E(xi^n*y)

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 1) ---- //
        this->degree = degree;
        // Вычисление сумм и средних значений
        calculateRegressionsSums(this->mode, this->numericDates, this->cursValues, this->n, this->degree, this->vector_values, this->values);
        // Заполнение таблицы
        fillTotalTable(ui->total_tableView, this->mode, this->n, this->degree, this->dataColumns, this->numericDates, this->cursValues, this->vector_values);
        // Заполнение текстового поля с суммами
        QString info{fillTextEditWithSums(this->mode, this->values, this->degree)};
        ui->totalTable_info_textEdit->setText(info);

        // ---- ОБРАБОТКА ДАННЫХ (БЛОК 2) ---- //
        // Вычисление коэффициентов
        if (!buildAndSolvePolynomRegression(this->values, this->coefficients, this->n, this->degree))
        {
            QMessageBox::critical(this, "Ошибка", QString("Не удалось построить параболическую регрессию: система уравнений вырождена или точек меньше 3."));
            return;
        }
        // Подсчет величин регрессии
        calculatePolynomRegression(this->degree, this->numericDates, this->cursValues, this->vector_values["yT"],
                                   this->coefficients, this->values, this->n, this->trend_eq, this->r_descr, this->regCoefStr);
        break;
    }
    default:
        setWindowTitle("Нерассматриваемый тип регрессии");
        QMessageBox::warning(nullptr, "Предупреждение", "Неизвестный тип регрессии, не удалось вычислить коэффициенты!");
        return;
    }

    // Отображение значений величин под таблицей
    if (mode == 2){
        ui->meanX_label->setText(("x\u0304 = ")  + QString::number(this->values["meanX"], 'g', 6));
        ui->meanY_label->setText(("l\u0304n\u0304y\u0304 = ")  + QString::number(this->values["meanLNY"], 'g', 6));
    }
    else if (mode == 3){
        ui->meanX_label->setText(("z\u0304 = ")  + QString::number(this->values["meanZ"], 'g', 6));
        ui->meanY_label->setText(("y\u0304 = ")  + QString::number(this->values["meanY"], 'g', 6));
    }
    else if (mode == 5){
        ui->meanX_label->setText(("l\u0304n\u0304x\u0304 = ")  + QString::number(this->values["meanLNX"], 'g', 6));
        ui->meanY_label->setText(("y\u0304 = ")  + QString::number(this->values["meanY"], 'g', 6));
    }
    else if (mode == 6){
        ui->meanX_label->setText(("l\u0304n\u0304x\u0304 = ")  + QString::number(this->values["meanLNX"], 'g', 6));
        ui->meanY_label->setText(("l\u0304n\u0304y\u0304 = ")  + QString::number(this->values["meanLNY"], 'g', 6));
    }
    else{
        ui->meanX_label->setText(((mode != 1) ? "x\u0304 = " : "y\u0304 = ")  + QString::number(this->values["meanX"], 'g', 6));
        ui->meanY_label->setText(((mode != 1) ? "y\u0304 = " : "x\u0304 = ")  + QString::number(this->values["meanY"], 'g', 6));
    }
    // Заполнение таблицы предсказаний модели
    QVector<double> predicts{(mode != 1) ? ((mode != 2 && mode != 6) ? this->vector_values["yT"] : this->vector_values["lnyT"]) : this->vector_values["xT"]};
    if (mode == 2 || mode == 6)
        fillCalculateTable(ui->calculate_tableView, this->mode, this->n, this->dataColumns, this->numericDates, this->cursValues, predicts, this->values, this->vector_values["lny"]);
    else if (mode == 1)
        fillCalculateTable(ui->calculate_tableView, this->mode, this->n, this->dataColumns, this->cursValues, this->numericDates, predicts, this->values);
    else
        fillCalculateTable(ui->calculate_tableView, this->mode, this->n, this->dataColumns, this->numericDates, this->cursValues, predicts, this->values);
    // Отображение найденных значений под таблицей
    double Sx2{this->values["Sx2"]}, Sy2{this->values["Sy2"]}, SxMean{this->values["SxMean"]}, SyMean{this->values["SyMean"]};
    QString str_Sx2{"S<sub>x</sub><sup>2</sup> = "}, str_Sy2{"S<sub>y</sub><sup>2</sup> = "}, str_SxMean{"S<sub>x\u0304</sub> = "}, str_SyMean{"S<sub>y\u0304</sub> = "};
    if (mode == 2){
        str_Sy2 = "S<sub>lny</sub><sup>2</sup> = ";
        str_SyMean = "S<sub>l\u0304n\u0304y\u0304</sub> = ";
        Sy2 = this->values["Slny2"];
        SyMean = this->values["SlnyMean"];
    }
    if (mode == 3){
        str_Sx2 = "S<sub>z</sub><sup>2</sup> = ";
        str_SxMean = "S<sub>z\u0304</sub> = ";
        Sx2 = this->values["Sz2"];
        SxMean = this->values["SzMean"];
    }
    if (mode == 5){
        str_Sx2 = "S<sub>lnx</sub><sup>2</sup> = ";
        str_SxMean = "S<sub>l\u0304n\u0304x\u0304</sub> = ";
        Sx2 = this->values["Slnx2"];
        SxMean = this->values["SlnxMean"];
    }
    if (mode == 6){
        str_Sx2 = "S<sub>lnx</sub><sup>2</sup> = ";
        str_SxMean = "S<sub>l\u0304n\u0304x\u0304</sub> = ";
        str_Sy2 = "S<sub>lny</sub><sup>2</sup> = ";
        str_SyMean = "S<sub>l\u0304n\u0304y\u0304</sub> = ";
        Sx2 = this->values["Slnx2"];
        SxMean = this->values["SlnxMean"];
        Sy2 = this->values["Slny2"];
        SyMean = this->values["SlnyMean"];
    }
    ui->n_label->setText("n = " + QString::number(this->n));
    ui->equation_label->setText(this->trend_eq);
    ui->coefficients_textEdit->setText(this->regCoefStr);
    ui->correlation_textEdit->setText(this->r_descr);
    ui->Sx2_label->setText(((mode != 1) ? str_Sx2 : str_Sy2) + QString::number(Sx2, 'g', 6));
    ui->Sy2_label->setText(((mode != 1) ? str_Sy2 : str_Sx2) + QString::number(Sy2, 'g', 6));
    ui->meanSx_label->setText(((mode != 1) ? str_SxMean : str_SyMean) + QString::number(SxMean, 'g', 6));
    ui->meanSy_label->setText(((mode != 1) ? str_SyMean  : str_SxMean) + QString::number(SyMean, 'g', 6));
    ui->R2_label->setText("R<sup>2</sup> = " + QString::number(this->values["R2"], 'g', 6));
    ui->Sfull_label->setText("S<sub>полн.</sub> = " + QString::number(this->values["Spoln"], 'g', 6));
    ui->Sost_label->setText("S<sub>ост.</sub> = " + QString::number(this->values["Sost"], 'g', 6));
    ui->Sregr_label->setText("S<sub>регр.</sub> = " + QString::number(this->values["Sregr"], 'g', 6));
    ui->MSE_label->setText("MSE = " + QString::number(this->values["MSE"], 'g', 6));
    ui->descr_R2_label->setText(QString("<b>Возможность прогноза (R%1>=75%):</b> ").arg(toSuperscript(2)) + QString("<i>%1</i>").arg(this->values["R2"] * 100 >= 75 ? "имеется." : "отсутствует."));

    // ---- ОТРИСОВКА ГРАФИКА ---- //
    (mode != 1) ? WorkplaceForm::MakePlot() : WorkplaceForm::MakeInversePlot();

    // ---- УСТАНОВКА ДАТЫ ---- //
    this->default_date = QDate::fromString(this->dataColumns.first(), "dd.MM.yyyy").addDays(1);
    //this->default_date = QDate::currentDate();
    ui->selectDate_dateEdit->setDate(this->default_date);
    ui->selectDate_dateEdit->setMinimumDate(this->default_date);
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
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = yReg;

    if (this->select_date.isValid()){
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения
        while (lastDate < this->select_date){
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);
            // Значение даты в секундах для оси абсцисс (по убыванию идут)
            temp_x.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут)
            temp_numericDates.prepend(0.01 + 0.99 * double(epoch.daysTo(lastDate) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())));
            // Значения курса доллара США
            double temp_pred{};
            switch(this->mode){
            case 0:
                temp_pred = this->coefficients["a0"] + this->coefficients["a1"] * temp_numericDates.first();
                break;
            case 2:
                temp_pred = this->coefficients["a0"] * std::exp(this->coefficients["a1"] * temp_numericDates.first());
                break;
            case 3:
                temp_pred = this->coefficients["a0"] + this->coefficients["a1"] / temp_numericDates.first();
                break;
            case 4:
                for (int i=0; i<=this->degree; ++i)
                    temp_pred += coefficients[QString("a%1").arg(i)] * std::pow(temp_numericDates.first(), i);
                break;
            case 5:
                temp_pred = this->coefficients["a0"] + this->coefficients["a1"] * std::log(temp_numericDates.first());
                break;
            case 6:
                temp_pred = this->coefficients["a0"] * std::pow(temp_numericDates.first(), this->coefficients["a1"]);
                break;
            case 7:
                for (int i=0; i<=this->degree; ++i)
                    temp_pred += coefficients[QString("a%1").arg(i)] * std::pow(temp_numericDates.first(), i);
                break;
            default:
                break;
            }
            temp_yReg.prepend(temp_pred);
        }
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
    ui->customPlot->graph(1)->setName(this->trend_eq + QString("\nR%1 = %2").arg(toSuperscript(2)).arg(values["R2"], 0, 'f', 6));

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
        ui->forecastCurs_label->setText("Прогноз курса Доллара США: " + QString::number(yPos, 'g', 6) + " руб.");
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
    ui->customPlot->xAxis->setRange(xRange.lower - ((temp_x.first() - x.first()) / 10 + 86400), xRange.upper + ((temp_x.first() - x.first()) / 10 + 86400));         // 1 день отступ
    ui->customPlot->yAxis->setRange(yRange.lower - ((temp_yReg.first() - yReg.first()) / 10 + 0.5), yRange.upper + ((temp_yReg.first() - yReg.first()) / 10 + 0.5)); // 0.5 отступ

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

        x[i] = this->cursValues[i];              // ось абсисс
        y[i] = dateTimes[i].toSecsSinceEpoch();  // ось ординат (экспереминатльные)
    }

    // Определение минимальных и максимальных значений даты
    QDateTime minDate = *std::min_element(dateTimes.constBegin(), dateTimes.constEnd());
    QDateTime maxDate = *std::max_element(dateTimes.constBegin(), dateTimes.constEnd());

    //
    for (int i=0; i<n; ++i){
        QDate epoch(1970, 1, 1);
        // Ось ординат (регрессия) [норм. кол-во дней -> кол-во дней] * 24 * 60 * 60
        yReg[i] = ((this->vector_values["xT"][i] - 0.01) / 0.99 * double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())) + epoch.daysTo(minDate.date()))*24*60*60;
    }

    // ----------------- Дополнение графика по выбранной дате (будущей) ----------------- //
    QVector<double> temp_x = x;
    QVector<double> temp_numericDates = this->numericDates;
    QVector<double> temp_yReg = yReg;

    if (this->select_date.isValid()){
        QDate lastDate = maxDate.date();
        // Пока последняя дата < введенной, добавляем значения
        while (lastDate < this->select_date){
            QDate epoch(1970, 1, 1);
            lastDate = lastDate.addDays(1);
            // Значение даты в секундах для оси ординат (по убыванию идут)
            temp_yReg.prepend(QDateTime(lastDate, QTime(0,0)).toSecsSinceEpoch());
            // Значения даты в днях для расчета предсказанного моделью значения (по убыванию идут)
            temp_numericDates.prepend(0.01 + 0.99 * double(epoch.daysTo(lastDate) - epoch.daysTo(minDate.date())) / double(epoch.daysTo(maxDate.date()) - epoch.daysTo(minDate.date())));

            // Решение уравнения [x = b0 + b1 * y] относительно неизвестной y (курса доллара), т.к. хотим предсказывать курс по дате, а не наоборот.
            // В этой функции полагается регрессия вида (переобозначение относительно программных переменных):
            //   y = b0 + b1 * x, где x - курс доллара США, y - предсказанная дата, уравнение которой решается относительно x.
            // Иными словами, уравнение решается не относительно y (как положено по обратной регрессии), а относительно x,
            //т.е. мы предсказываем не дату по курсу доллара США, а как будто-бы по уже предсказанной дате моделируем курс доллара США.
            temp_x.prepend((temp_numericDates.first() - coefficients["b0"]) / coefficients["b1"]);
        }
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
    ui->customPlot->graph(1)->setName(this->trend_eq + QString("\nR%1 = %2").arg(toSuperscript(2)).arg(values["R2"], 0, 'f', 6));

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
        ui->forecastCurs_label->setText("Прогноз курса Доллара США: " + QString::number(xPos, 'g', 6) + " руб.");
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
    ui->customPlot->xAxis->setRange(xRange.lower - ((temp_x.first() - x.first()) / 10 + 0.5), xRange.upper + ((temp_x.first() - x.first()) / 10 + 0.5));               // 0.5 отступ
    ui->customPlot->yAxis->setRange(yRange.lower - ((temp_yReg.first() - yReg.first())/ 10 + 86400), yRange.upper + ((temp_yReg.first() - yReg.first())/ 10 + 86400)); // 1 день отступ

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
