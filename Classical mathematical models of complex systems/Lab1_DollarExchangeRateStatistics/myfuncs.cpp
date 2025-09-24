#include "myfuncs.h"
#include "xlsxdocument.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QMessageBox>

// ----- Общие функии ----- //
//Центрирование значений в таблицах
void centerTableItems(QTableView *tableView)
{
    if (!tableView)
        return;

    QAbstractItemModel *model = tableView->model();
    if (!model)
        return;

    int rows = model->rowCount();
    int cols = model->columnCount();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QStandardItem *item = qobject_cast<QStandardItemModel*>(model)->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}
//Загрузка исходных данных из Excel в таблицу
bool loadDataFromExcel(const QString &filePath, QTableView *tableView)
{
    QFileInfo fileInfo(filePath);
    // 1. Проверка расширения
    if (fileInfo.suffix().toLower() != "xlsx") {
        QMessageBox::warning(nullptr, "Предупреждение", "Неверное расширение файла. Требуется .xlsx");
        return false;
    }

    // 2. Попытка открыть файл
    QXlsx::Document xlsx(filePath);
    if (!xlsx.isLoadPackage()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл Excel!");
        return false;
    }

    // 3. Определяем диапазон данных
    auto dim = xlsx.dimension();
    if (!dim.isValid()) {
        QMessageBox::warning(nullptr, "Предупреждение", "Файл пуст или необходимо вручную сохранить формат .xlsx!");
        return false;
    }

    int rowCount = dim.rowCount();
    int colCount = dim.columnCount();

    // Создаем модель для QTableView
    QStandardItemModel *model = new QStandardItemModel(rowCount - 1, colCount, tableView);

    // Заголовки из первой строки
    QStringList headers;
    for (int col = 1; col <= colCount; ++col) {
        auto cell = xlsx.cellAt(1, col);
        if (cell) headers << cell->value().toString();
        else headers << QString("Column%1").arg(col);
    }
    model->setHorizontalHeaderLabels(headers);

    // Считываем все данные
    for (int row = 2; row <= rowCount; ++row) {
        for (int col = 1; col <= colCount; ++col) {
            auto cell = xlsx.cellAt(row, col);
            QString value{};

            // Если ячейка читаемая и не пустая
            if (cell) {
                QVariant cellValue = cell->value();
                bool ok = false;

                // Попытка конвертировать в число
                double d = cellValue.toDouble(&ok);
                if (ok && col != 2) {  // если это не дата
                    value = QString::number(d, 'g', 6);
                }
                else if (col == 2) { // столбец с датой
                    // Попытка распарсить как строку даты
                    QDate date = QDate::fromString(cellValue.toString(), "dd.MM.yyyy");
                    if (!date.isValid())
                        date = QDate::fromString(cellValue.toString(), Qt::ISODate);
                    if (!date.isValid()) {
                        // Попытка как Excel-число
                        double excelDate = cellValue.toDouble(&ok);
                        if (ok)
                            date = QDate(1900, 1, 1).addDays(int(excelDate) - 2); //1900г. считается високосным + смещение Excel (с 1)
                    }
                    value = date.isValid() ? date.toString("dd.MM.yyyy") : cellValue.toString();
                }
                else { // Просто текст
                    value = cellValue.toString();
                }
            } else {
                value = "";
            }

            QStandardItem *item = new QStandardItem(value);
            model->setItem(row - 2, col - 1, item);
        }
    }

    // Привязываем модель к QTableView
    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Центрирование
    centerTableItems(tableView);

    // Информационное сообщение
    QMessageBox::information(nullptr, "Успех", "Данные успешно загружены!");

    return true;
}
//Чтение исходных данных из таблицы и приведение к нужному виду
void readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues)
{
    const QAbstractItemModel *model = tableView->model();
    if (!model) return;

    int colData = -1;
    int colCurs = -1;
    for (int c = 0; c < model->columnCount(); ++c){
        QString header = model->headerData(c, Qt::Horizontal).toString().trimmed().toLower();
        if (header == "data") colData = c;
        if (header == "curs") colCurs = c;
    }

    if (colData == -1 && colCurs == -1){
        QMessageBox::critical(nullptr, "Ошибка", "Отсутствуют столбцы 'data' и 'curs' для вычислений!");
        return;
    }

    numericDates.clear();
    cursValues.clear();

    QDate epoch(1970, 1, 1);

    for (int r = 0; r < model->rowCount(); ++r){
        // Конвертация даты в число дней
        QVariant valData = model->index(r, colData).data();
        QDate date = QDate::fromString(valData.toString(), "dd.MM.yyyy");
        if (date.isValid()) {
            dataColumn.append(date.toString("dd.MM.yyyy"));
            numericDates.append(epoch.daysTo(date)/10000.0); // Чтобы не работать с большими числами, уменьшим в 10000 раз число дней
        } else {
            numericDates.append(0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать дату:%1!").arg(valData.toString()));
            return;
        }

        // Конвертация курса в число
        QVariant valCurs = model->index(r, colCurs).data();
        bool ok;
        double curs = valCurs.toString().toDouble(&ok);
        if (ok) {
            cursValues.append(curs);
        } else {
            cursValues.append(0.0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать курс:%1!").arg(valCurs.toString()));
            return;
        }
    }
}

// ----- Функции общих вычислений ----- //
//Вычисление значений, необходимых для построения регрессии
void calculateRegressionTotalValues(const QVector<double> numericDates, const QVector<double> &cursValues,
                                    QVector<double> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                                    RegressionValues &values)
{
    values.n = numericDates.size();
    if (values.n==0) return;

    xSquared.clear();
    ySquared.clear();
    xyProduct.clear();

    values.sumX = values.sumY = values.sumX2 = values.sumY2 = values.sumXY = 0.0;

    for (int i = 0; i < values.n; ++i){
        double x = numericDates[i];
        double y = cursValues[i];

        double x2 = x * x;
        double y2 = y * y;
        double xy = x * y;

        xSquared.append(x2);
        ySquared.append(y2);
        xyProduct.append(xy);

        values.sumX  += x;
        values.sumY  += y;
        values.sumX2 += x2;
        values.sumY2 += y2;
        values.sumXY += xy;
    }

    values.meanX = values.sumX / values.n;
    values.meanY = values.sumY / values.n;
}
//Вычисление значений, необходимых для проверки регрессии
bool calculateRegressionCalcValues(const int& mode,
                                   const QVector<double> &cursValues,
                                   const QVector<double> &yT,
                                   QVector<double> &Sost, QVector<double> &Sregr, QVector<double> Sfull,
                                   RegressionValues &values, QHash<QString, double> &coefficients, const double eps)
{
    Sost.clear();
    Sregr.clear();
    Sfull.clear();

    values.sumOst = values.sumRegr = values.sumFull = 0.0;

    for (int i = 0; i < values.n; ++i){
        double elem_of_Sost = std::pow(cursValues[i] - yT[i], 2);
        double elem_of_Sregr = std::pow((mode != 1) ? (yT[i] - values.meanY) : (yT[i] - values.meanX), 2);
        double elem_of_Sfull = std::pow((mode != 1) ? (cursValues[i] - values.meanY) : (cursValues[i] - values.meanX), 2);

        Sost.append(elem_of_Sost);
        Sregr.append(elem_of_Sregr);
        Sfull.append(elem_of_Sfull);

        values.sumOst += elem_of_Sost;
        values.sumRegr += elem_of_Sregr;
        values.sumFull += elem_of_Sfull;
    }

    // Коэффииент детерминации и MSE
    coefficients.insert("R2", 1 - values.sumOst / values.sumFull);
    coefficients.insert("MSE", values.sumOst / values.n);

    // Несмещенная дисперсия и среднее кв. отклонение выборочнного среднего
    values.Sx2 = 1.0 / (values.n - 1.0) * (values.sumX2 - 1.0 / values.n * std::pow(values.sumX, 2));
    values.Sy2 = 1.0 / (values.n - 1.0) * (values.sumY2 - 1.0 / values.n * std::pow(values.sumY, 2));
    values.meanSx = std::sqrt(values.Sx2 / values.n);
    values.meanSy = std::sqrt(values.Sy2 / values.n);

    return std::abs(values.sumOst + values.sumRegr - values.sumFull) < eps;
}

//Заполнение общей таблицы значениями
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<double> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct)
{
    int rowCount = numericDates.size();
    int colCount = 6; // data, xi, yi, xi^2, yi^2, xi*yi или data, yi, xi, yi^2, xi^2, yi*xi

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
    model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");
    model->setHeaderData(3, Qt::Horizontal, (mode != 1) ? "x\u1D62\u00B2" : "y\u1D62\u00B2");
    model->setHeaderData(4, Qt::Horizontal, (mode != 1) ? "y\u1D62\u00B2" : "x\u1D62\u00B2");
    model->setHeaderData(5, Qt::Horizontal, (mode != 1) ? "x\u1D62 \u00B7 y\u1D62" : "y\u1D62 \u00B7 x\u1D62");

    for (int r = 0; r < rowCount; ++r) {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number((mode != 1) ? numericDates[r] : cursValues[r], 'f', 5)));
        model->setItem(r, 2, new QStandardItem(QString::number((mode != 1) ? cursValues[r] : numericDates[r], 'f', 5)));
        model->setItem(r, 3, new QStandardItem(QString::number((mode != 1) ? xSquared[r] : ySquared[r], 'f', 5)));
        model->setItem(r, 4, new QStandardItem(QString::number((mode != 1) ? ySquared[r] : xSquared[r], 'f', 5)));
        model->setItem(r, 5, new QStandardItem(QString::number(xyProduct[r], 'f', 5)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}
//Заполнение вычисляемой таблицы значениями
void fillCalculateTable(QTableView *tableView,
                        const int& mode,
                        const QVector<QString> &dataColumn,
                        const QVector<double> &numericDates,
                        const QVector<double> &cursValues,
                        const QVector<double> &yT,
                        const RegressionValues &values)
{
    int rowCount = numericDates.size();
    int colCount = 7; // data, xi, yi, yi^T, (yi-yi^T)^2, (yi^T - mean(y))^2, (yi - mean(y))^2
                      // data, yi, xi, xi^T, (xi-xi^T)^2, (xi^T - mean(x))^2, (xi - mean(x))^2

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
    model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");
    model->setHeaderData(3, Qt::Horizontal, (mode != 1) ? "y\u1D62\u1D40" : "x\u1D62\u1D40");
    model->setHeaderData(4, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u1D62\u1D40)\u00B2" : "(x\u1D62 - x\u1D62\u1D40)\u00B2");
    model->setHeaderData(5, Qt::Horizontal, (mode != 1) ? "(y\u1D62\u1D40 - y\u0304)\u00B2" : "(x\u1D62\u1D40 - x\u0304)\u00B2");
    model->setHeaderData(6, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u0304)\u00B2" : "(x\u1D62 - x\u0304)\u00B2");

    for (int r = 0; r < rowCount; ++r) {
        const char fmt = (mode != 1) ? 'f' : 'g';
        const int sign_count = (mode != 1) ? 6 : 8;
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number((mode != 1) ? numericDates[r] : cursValues[r], fmt, sign_count)));
        model->setItem(r, 2, new QStandardItem(QString::number((mode != 1) ? cursValues[r] : numericDates[r], fmt, sign_count)));
        model->setItem(r, 3, new QStandardItem(QString::number(yT[r], 'f', 6)));
        model->setItem(r, 4, new QStandardItem(QString::number((mode != 1) ? std::pow(cursValues[r]-yT[r], 2) : std::pow(numericDates[r]-yT[r], 2), fmt, sign_count)));
        model->setItem(r, 5, new QStandardItem(QString::number((mode != 1) ? std::pow(yT[r]-values.meanY, 2) : std::pow(yT[r]-values.meanX, 2), fmt, sign_count)));
        model->setItem(r, 6, new QStandardItem(QString::number((mode != 1) ? std::pow(cursValues[r]-values.meanY, 2) : std::pow(numericDates[r]-values.meanX, 2), fmt, sign_count)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}

// ----- Расчет значений для регрессионной модели ----- //
// ОБЩЕЕ
QString getRegressionRelationship(const int& mode, const double& r){
    QString regr_description = "Характер связи: ";
    if (r == 0) {
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: Отсутствует.\n"
                                   "Интерпретация связи: -.")
                               .arg(QString::number(r, 'f', 2));
    }
    else if (r > 0 && r < 1) {
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: Вероятностная, прямая.\n%2")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "Интерпретация связи: С увеличением X увеличивается Y."
                                                : "Интерпретация связи: С увеличением Y увеличивается X.");

    }
    else if (r < 0 && r > -1){
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: Вероятностная, обратная.\n%2")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "Интерпретация связи: С увеличением X уменьшается Y."
                                                : "Интерпретация связи: С увеличением Y уменьшается X.");

    }
    else if (r == 1){
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: Функциональная, прямая.\n"
                                   "Интерпретация связи: Каждому значению факторного признака строго соответствует одно значение функции, %2")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "с увеличением X увеличивается Y."
                                                : "с увеличением Y увеличивается X.");
    }
    else if (r == -1){
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Характер связи: Функциональная, обратная.\n"
                                   "Интерпретация связи: Каждому значению факторного признака строго соответствует одно значение функции, %2)")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "с увеличением X уменьшается Y, и наоборот."
                                                : "с увеличением Y уменьшается X, и наоборот.");
    }
    else{
        regr_description = QString("Значение линейного коэффициента связи: r = %1\n"
                                   "Ошибка вычислений.")
                               .arg(QString::number(r, 'f', 2));
        return regr_description;
    }

    // Характер связи
    if (std::abs(r) < 0.3)
        regr_description += QString("\nХарактер связи: Практически отсустствует.");
    else if (std::abs(r) >= 0.3 && std::abs(r) < 0.5)
        regr_description += QString("\nХарактер связи: Слабая.");
    else if (std::abs(r) >= 0.5 && std::abs(r) < 0.7)
        regr_description += QString("\nХарактер связи: Умеренная.");
    else
        regr_description += QString("\nХарактер связи: Сильная.");

    return regr_description;
}
QString getDeterminationDescription(const double& R2){
    QString determination_descr = "Можно ли использовать для прогнозирования (R\u00B2>=75%): ";
    if (R2 * 100 >= 75)
        determination_descr += "Да!";
    else
        determination_descr += "Не рекомендуется!";
    return determination_descr;
}
// ЛИНЕЙНАЯ И ОБРАТНАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ
bool calculateLinearRegressionCoefficients(RegressionValues &values, QHash<QString, double> &coefficients, const double eps){
    // y = a0 + a1*x                        // y = b0 + b1*x
    // Система:                             // Система:
    // {a0*n + a1*E(xi) = E(yi)             // {b0*n + b1*E(yi) = E(xi)
    // {a0*E(xi) + a1*E(xi^2) = E(xi*yi)    // {b0*E(yi) + b1*E(yi^2) = E(xi*yi)

    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1, B, B0, B1, b0, b1)
    // A
    coefficients.insert("A", values.n * values.sumX2 - values.sumX * values.sumX);
    // A0
    coefficients.insert("A0", values.sumY * values.sumX2 - values.sumX * values.sumXY);
    // A1
    coefficients.insert("A1", values.n * values.sumXY - values.sumY * values.sumX);
    // B
    coefficients.insert("B" , values.n * values.sumY2 - values.sumY * values.sumY);
    // B0
    coefficients.insert("B0", values.sumX * values.sumY2 - values.sumY * values.sumXY);
    // B1
    coefficients.insert("B1", values.n * values.sumXY - values.sumY * values.sumX);
    // a0
    coefficients.insert("a0", coefficients["A0"] / coefficients["A"]);
    // a1
    coefficients.insert("a1", coefficients["A1"] / coefficients["A"]);
    // b0
    coefficients.insert("b0", coefficients["B0"] / coefficients["B"]);
    // b1
    coefficients.insert("b1", coefficients["B1"] / coefficients["B"]);
    // r1
    coefficients.insert("r1", coefficients["A1"] / std::sqrt(coefficients["A"] * coefficients["B"]));
    // r2
    coefficients.insert("r2", coefficients["B1"] / std::sqrt(coefficients["A"] * coefficients["B"]));

    return std::abs(coefficients["r1"] - coefficients["r2"]) < eps;
}
void calculateLinearRegressionValues(const int& mode, const QVector<double> &numericDates, QVector<double> &yT,
                                     const int &n, const QHash<QString, double> &coefficients){
    // yT или xT
    yT.clear();
    for (int i=0; i<n; ++i){
        if (mode != 1)
            yT.append(coefficients["a0"] + coefficients["a1"] * numericDates[i]);
        else
            yT.append(coefficients["b0"] + coefficients["b1"] * numericDates[i]);
    }
}
void calculateInverseLinearRegressionValuesByDates(const QVector<double> &yReg, QVector<double> &xT,
                                                   const int &n, const QHash<QString, double> &coefficients){
    // xT
    xT.clear();
    for (int i = 0; i < n; ++i){
        xT.append((yReg[i]/60/60/24/10000 - coefficients["b0"]) / coefficients["b1"]);
    }
}
