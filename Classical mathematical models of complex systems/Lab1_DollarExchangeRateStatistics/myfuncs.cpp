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
//Перевод степени в верхний индекс
QString toSuperscript(const int &number)
{
    QString result{};
    for (QChar ch : QString::number(number))
    {
        switch(ch.unicode())
        {
        case '0': result += QChar(0x2070); break;
        case '1': result += QChar(0x00B9); break;
        case '2': result += QChar(0x00B2); break;
        case '3': result += QChar(0x00B3); break;
        case '4': result += QChar(0x2074); break;
        case '5': result += QChar(0x2075); break;
        case '6': result += QChar(0x2076); break;
        case '7': result += QChar(0x2077); break;
        case '8': result += QChar(0x2078); break;
        case '9': result += QChar(0x2079); break;
        }
    }
    return result;
}


/*------------------
 |  Главное окно  |
 -----------------*/
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
bool readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues)
{
    const QAbstractItemModel *model = tableView->model();
    if (!model) return false;

    int colData = -1;
    int colCurs = -1;
    for (int c = 0; c < model->columnCount(); ++c){
        QString header = model->headerData(c, Qt::Horizontal).toString().trimmed().toLower();
        if (header == "data") colData = c;
        if (header == "curs") colCurs = c;
    }

    if (colData == -1 && colCurs == -1){
        QMessageBox::critical(nullptr, "Ошибка", "Отсутствуют столбцы 'data' и 'curs' для вычислений!");
        return false;
    }

    numericDates.clear();
    cursValues.clear();

    QDate epoch(1970, 1, 1);
    QVector<int> rawDates{}; // Временное хранилище дат в днях

    for (int r = 0; r < model->rowCount(); ++r){
        // Конвертация даты в число дней
        QVariant valData = model->index(r, colData).data();
        QDate date = QDate::fromString(valData.toString(), "dd.MM.yyyy");
        if (date.isValid()) {
            dataColumn.append(date.toString("dd.MM.yyyy"));
            rawDates.append(epoch.daysTo(date));
        } else {
            rawDates.append(0);
            QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось корректно обработать дату:%1!").arg(valData.toString()));
            return false;
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
            return false;
        }
    }

    // --- Нормализация дат ---
    if (!rawDates.isEmpty())
    {
        int minDateDays = *std::min_element(rawDates.begin(), rawDates.end());
        int maxDateDays = *std::max_element(rawDates.begin(), rawDates.end());
        int range = maxDateDays - minDateDays;

        numericDates.resize(rawDates.size());
        for (int i = 0; i < rawDates.size(); ++i)
            numericDates[i] = 0.01 + 0.99 * double(rawDates[i] - minDateDays) / double(range);
    }

    return true;
}


/*------------------
 |     БЛОК 1      |
 -----------------*/
// --- Функции заполнения виджетов --- //
// 1)
// Получение названий вычисляемых столбцов для неполиномиальных регрессий (экспонениальная, гиперболическая, степенная, логарифмическая)
QString getColumnNameForNotPolynomialRegressions(const QString& key){
    QString header;
    if (key == "lny") header = "lny\u1D62";
    else if (key == "lnx") header = "lnx\u1D62";
    else if (key == "z") header = "z\u1D62=1/x\u1D62";
    else if (key == "x2") header = "x\u1D62" + toSuperscript(2);
    else if (key == "z2") header = "z\u1D62" + toSuperscript(2);
    else if (key == "lny2") header = "(lny\u1D62)" + toSuperscript(2);
    else if (key == "xlny") header = "x\u1D62\u00B7lny\u1D62";
    else if (key == "y2") header = "y\u1D62" + toSuperscript(2);
    else if (key == "zy") header = "z\u1D62y\u1D62";
    else if (key == "lnxy") header = "lnx\u1D62\u00B7y\u1D62";
    else if (key == "lnxlny") header = "lnx\u1D62\u00B7lny\u1D62";
    else if (key == "lnx2") header = "(lnx\u1D62)" + toSuperscript(2);
    else header = key; // На всякий случай

    return header;
}
// Заполнение таблицы значениями для регрессий
void fillTotalTable(QTableView *tableView, const int &mode, const int& n, const int& degree,
                    const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                    const QHash<QString, QVector<double>> vector_values){
    int rowCount{n};
    int calcColCount{static_cast<int>(vector_values.size())};
    int colCount{3 + calcColCount}; // data, x, y + вычисляемые

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // --- Заголовки ---
    // Базовые
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
    model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");

    // Автоматические для vector_values
    QVector<QString> keys{};
    if (mode == 0 || mode == 1 || mode == 4 || mode == 7){ // Полиномиальные
        int c = 3; // Итератор номера столбца
        for (int k=0; k<calcColCount; ++k){
            QString header{};
            if (k < 2 * degree - 1){ // k=2,...,degree+degree
                int power{k+2};
                header = QString((mode != 1) ? "x\u1D62" : "y\u1D62") + toSuperscript(power);
                keys.append("x" + QString::number(power));
            }
            else{
                int power{k - (2 * degree - 1) + 1};
                if (mode != 1) // k=1,...,degree
                    header = "x\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "y\u1D62";
                else
                    header = "y\u1D62" + ((power == 1) ? "" : toSuperscript(power)) + "x\u1D62";
                keys.append("x" + QString::number(power) + "y");
            }

            model->setHeaderData(c++, Qt::Horizontal, header);

            // Вывод y^2 отдельно.
            if (k == 0)
            {
                header = QString((mode != 1) ? "y\u1D62" : "x\u1D62") + toSuperscript(2);
                model->setHeaderData(c++, Qt::Horizontal, header);
                keys.append("y2");
            }
        }
    }
    else{
        // Определяем ключи для текущего режима
        switch(mode){
        case 2: keys = {"lny", "x2", "lny2", "xlny"}; break;
        case 3: keys = {"z", "z2", "y2", "zy"}; break;
        case 5: keys = {"lnx", "lnx2", "y2", "lnxy"}; break;
        case 6: keys = {"lnx", "lny", "lnx2", "lny2", "lnxlny"}; break;
        default: break;
        }
        // Названия из vector_values
        for (int c = 0; c < keys.size(); ++c)
            model->setHeaderData(3 + c, Qt::Horizontal, getColumnNameForNotPolynomialRegressions(keys[c]));
    }

    // --- Заполнение таблицы ---
    for (int r=0; r<rowCount; ++r) {
        model->setItem(r, 0, new QStandardItem(dataColumns[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 6)));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));

        // Значения vector_values
        for (int c = 0; c < keys.size(); ++c) {
            const QString &key = keys[c];
            if (vector_values.contains(key))
                model->setItem(r, 3 + c, new QStandardItem(QString::number(vector_values[key][r], 'f', 6)));
        }
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Центрирование
    centerTableItems(tableView);
}
// 2)
// Получение названий левых выражений сумм для для неполиномиальных регрессиий (экспоненциальная, гиперболическая, степенная, логарифмическая)
QString getSumLHSForNotPolynomialRegressions(const QString& key){
    QString LHS_current_sum{};
    if (key == "sumX") LHS_current_sum = "\u2211x";
    else if (key == "sumY") LHS_current_sum = "\u2211y";
    else if (key == "sumLNY") LHS_current_sum = "\u2211lny";
    else if (key == "sumX2") LHS_current_sum = "\u2211x" + toSuperscript(2);
    else if (key == "sumLNY2") LHS_current_sum = "\u2211(lny)" + toSuperscript(2);
    else if (key == "sumXLNY") LHS_current_sum = "\u2211(x\u00B7lny)";
    else if (key == "sumZ") LHS_current_sum = "\u2211z";
    else if (key == "sumZ2") LHS_current_sum = "\u2211z" + toSuperscript(2);
    else if (key == "sumY2") LHS_current_sum = "\u2211y" + toSuperscript(2);
    else if (key == "sumZY") LHS_current_sum = "\u2211zy";
    else if (key == "sumLNX") LHS_current_sum = "\u2211lnx";
    else if (key == "sumLNX2") LHS_current_sum = "\u2211(lnx)" + toSuperscript(2);
    else if (key == "sumLNXLNY") LHS_current_sum = "\u2211(lnx\u00B7lny)";
    else if (key == "sumLNXY") LHS_current_sum = "\u2211(lnx\u00B7y)";
    else LHS_current_sum = key;

    return LHS_current_sum;
}
// Заполнение текстового поля с суммами для регрессий
QString fillTextEditWithSums(const int &mode, const QHash<QString, double> &values, const int &degree){
    QString info{};

    if (mode == 0 || mode == 1 || mode == 4 || mode == 7){
        info += QString((mode != 1) ? "\u2211x = %1\n" : "\u2211y = %1\n").arg(QString::number(values["sumX"], 'f', 6));
        info += QString((mode != 1) ? "\u2211y = %1\n" : "\u2211x = %1\n").arg(QString::number(values["sumY"], 'f', 6));

        // sumXk, k=2,...,2*degree
        for (int k=2; k<=2*degree; ++k){
            info += QString((mode != 1) ? "\u2211x%1 = %2\n" : "\u2211y%1 = %2\n")
                        .arg(toSuperscript(k))
                        .arg(QString::number(values["sumX" + QString::number(k)], 'f', 6));
            // sumY2 отдельно выводится
            if (k == 2){
                info += QString((mode != 1) ? "\u2211y%1 = %2\n" : "\u2211x%1 = %2\n")
                            .arg(toSuperscript(k))
                            .arg(QString::number(values["sumY2"], 'f', 6));
            }
        }

        // sumXkY, k=1,...,degree
        for (int k=1; k<=degree; ++k){
            info += QString((mode != 1) ? "\u2211x%1y = %2\n" : "\u2211y%1x = %2\n")
                        .arg(k == 1 ? "" : toSuperscript(k))
                        .arg(QString::number(values["sumX" + QString::number(k) + "Y"], 'f', 6));
        }
    }
    else{ // mode == 2 || mode == 3 || mode == 5 || mode == 6
        // Определяем ключи для текущего режима
        QVector<QString> keys;
        switch (mode) {
        case 2: keys = {"sumX", "sumLNY", "sumX2", "sumLNY2", "sumXLNY"}; break;
        case 3: keys = {"sumZ", "sumY", "sumZ2", "sumY2", "sumZY"}; break;
        case 5: keys = {"sumLNX", "sumY", "sumLNX2", "sumY2", "sumLNXY"}; break;
        case 6: keys = {"sumLNX", "sumLNY", "sumLNX2", "sumLNY2", "sumLNXLNY"}; break;
        default: break;
        }

        // Заполнение текстового поля
        for (const QString &key : keys) {
            info += getSumLHSForNotPolynomialRegressions(key) + QString(" = %1\n").arg(QString::number(values[key], 'f', 6));
        }
    }

    return info;
}

// --- Функии вычисления для соответствующего типа регрессии --- //
// Вычисление сумм и средних значений у регрессий
void calculateRegressionsSums(const int& mode, const QVector<double> &numericDates, const QVector<double> &cursValues,
                              int &n, const int &degree, QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values){
    n = numericDates.size();
    if (n == 0) return;

    vector_values.clear();
    values.clear();

    // Лямбда-функция для вычислений
    auto addValue = [&](const QString &key, double val) {
        vector_values[key].append(val);
        values["sum" + key.toUpper()] += val;
    };

    for (int i=0; i<n; ++i){
        // Исходные данные
        double x = numericDates[i];
        double y = cursValues[i];

        // Общие для всех
        values["sumX"] += x;
        values["sumY"] += y;

        if (mode == 0 || mode == 1 || mode == 4 || mode == 7){
            // 1) x^k, k=(2,..,2*degree)
            for (int k=2; k <= degree + degree; ++k){
                double xk{std::pow(x, k)};
                vector_values["x" + QString::number(k)].append(xk);
                values["sumX" + QString::number(k)] += xk;
            }

            // 2) x^k * y, k=(1,...,degree)
            for (int k=1; k <= degree; ++k){
                double xky{std::pow(x,k)*y};
                vector_values["x" + QString::number(k) + "y"].append(xky);
                values["sumX" + QString::number(k) + "Y"] += xky;
            }
            addValue("y2", y*y);
        }
        else if (mode == 2){ // Экспоненциальная: data, x, y, lny, x^2, (lny)^2, xlny
            double lny = std::log(y);
            addValue("lny", lny);
            addValue("x2", x*x);
            addValue("lny2", lny*lny);
            addValue("xlny", x*lny);
        }
        else if (mode == 3){ // Гиперболическая: data, x, y, z, z^2, y^2, zy
            double z = 1/x;
            addValue("z", z);
            addValue("z2", z*z);
            addValue("y2", y*y);
            addValue("zy", z*y);
        }
        else if (mode == 5){ // Логарифмическая: data, x, y, lnx, (lnx)^2, y^2, lnxy
            double lnx = std::log(x);
            addValue("lnx", lnx);
            addValue("lnx2", lnx*lnx);
            addValue("y2", y*y);
            addValue("lnxy", lnx*y);
        }
        else{ // Степенная: data, x, y, lnx, lny, (lnx)^2, (lny)^2, lnx * lny
            double lnx = std::log(x);
            double lny = std::log(y);
            addValue("lnx", lnx);
            addValue("lny", lny);
            addValue("lnx2", lnx*lnx);
            addValue("lny2", lny*lny);
            addValue("lnxlny", lnx*lny);
        }
    }
    // Вычисление средних значений
    if (mode == 0 || mode == 1 || mode == 4 || mode == 7){
        values["meanX"] = values["sumX"] / n;
        values["meanY"] = values["sumY"] / n;
    }
    else if (mode == 2){
        values["meanX"] = values["sumX"] / n;
        values["meanLNY"] = values["sumLNY"] / n;
    }
    else if (mode == 3){
        values["meanZ"] = values["sumZ"] / n;
        values["meanY"] = values["sumY"] / n;
    }
    else if (mode == 5){
        values["meanLNX"] = values["sumLNX"] / n;
        values["meanY"] = values["sumY"] / n;
    }
    else{ // mode == 6
        values["meanLNX"] = values["sumLNX"] / n;
        values["meanLNY"] = values["sumLNY"] / n;
    }
}


/*------------------
 |     БЛОК 2      |
 -----------------*/
// --- Функции заполнения виджетов и описания --- //
// Заполнение таблицы с предсказаниями значениями для регрессий
void fillCalculateTable(QTableView* tableView, const int &mode, const int &n,
                        const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                        const QVector<double> &predicts, const QHash<QString, double> &values,
                        const QVector<double> &lny)
{
    int rowCount = n;
    int colCount = (mode != 2 && mode != 6) ? 7 : 8; // data, xi, yi, yi^T, (yi-yi^T)^2, (yi^T-mean(y))^2, (yi-mean(y))^2
                                                     // data, xi, yi, lnyi, lnyi^T, (lnyi-lnyi^T)^2, (lnyi^T-mean(lny))^2, (lnyi^T-mean(lny))^2

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки
    if (mode != 2 && mode != 6) {
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, (mode != 1) ? "x\u1D62" : "y\u1D62");
        model->setHeaderData(2, Qt::Horizontal, (mode != 1) ? "y\u1D62" : "x\u1D62");
        model->setHeaderData(3, Qt::Horizontal, (mode != 1) ? "y\u1D62\u1D40" : "x\u1D62\u1D40");
        model->setHeaderData(4, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u1D62\u1D40)\u00B2" : "(x\u1D62 - x\u1D62\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, (mode != 1) ? "(y\u1D62\u1D40 - y\u0304)\u00B2" : "(x\u1D62\u1D40 - x\u0304)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, (mode != 1) ? "(y\u1D62 - y\u0304)\u00B2" : "(x\u1D62 - x\u0304)\u00B2");
    } else { // Экспоненциальная или степенная
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "x\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "y\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "lny\u1D62");
        model->setHeaderData(4, Qt::Horizontal, "lny\u1D62\u1D40");
        model->setHeaderData(5, Qt::Horizontal, "(lny\u1D62 - (lny\u1D62)\u1D40)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "((lny\u1D62)\u1D40 - l\u0304n\u0304y\u0304)\u00B2");
        model->setHeaderData(7, Qt::Horizontal, "(lny\u1D62 - l\u0304n\u0304y\u0304)\u00B2");
    }

    // Заполнение строк
    for (int r = 0; r < rowCount; ++r) {
        model->setItem(r, 0, new QStandardItem(dataColumns[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 6)));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));

        if (mode != 2 && mode != 6) {
            model->setItem(r, 3, new QStandardItem(QString::number(predicts[r], 'f', 6)));
            model->setItem(r, 4, new QStandardItem(QString::number(std::pow(cursValues[r] - predicts[r], 2), 'f', 6)));
            model->setItem(r, 5, new QStandardItem(QString::number(std::pow(predicts[r] - values["meanY"], 2), 'f', 6)));
            model->setItem(r, 6, new QStandardItem(QString::number(std::pow(cursValues[r] - values["meanY"], 2), 'f', 6)));
        } else {
            model->setItem(r, 3, new QStandardItem(QString::number(lny[r], 'f', 6)));
            model->setItem(r, 4, new QStandardItem(QString::number(predicts[r], 'f', 6)));
            model->setItem(r, 5, new QStandardItem(QString::number(std::pow(lny[r] - predicts[r], 2), 'g', 6)));
            model->setItem(r, 6, new QStandardItem(QString::number(std::pow(predicts[r] - values["meanLNY"], 2), 'g', 6)));
            model->setItem(r, 7, new QStandardItem(QString::number(std::pow(lny[r] - values["meanLNY"], 2), 'g', 6)));
        }
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Центрирование
    centerTableItems(tableView);
}
// Сотавление вида функии и описания коэффициентов
void getFuncCoefDescr(const int& mode, QHash<QString, double> &coefficients, QString& regCoefStr, QString& trend_eq){
    // Описание коэффициентов
    if (mode == 0 || mode == 1){
        regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                             "A = %1<br>"
                             "A<sub>0</sub> = %2<br>"
                             "A<sub>1</sub> = %3<br>"
                             "<br>"
                             "B = %4<br>"
                             "B<sub>0</sub> = %5<br>"
                             "B<sub>1</sub> = %6<br>"
                             "<br>"
                             "<b>Коэффициенты:</b><br>"
                             "a<sub>0</sub> = %7<br>"
                             "a<sub>1</sub> = %8<br>"
                             "<br>"
                             "b<sub>0</sub> = %9<br>"
                             "b<sub>1</sub> = %10")
                         .arg(QString::number(coefficients["A"], 'g', 6))
                         .arg(QString::number(coefficients["A0"], 'g', 6))
                         .arg(QString::number(coefficients["A1"], 'g', 6))
                         .arg(QString::number(coefficients["B"], 'g', 6))
                         .arg(QString::number(coefficients["B0"], 'g', 6))
                         .arg(QString::number(coefficients["B1"], 'g', 6))
                         .arg(QString::number(coefficients["a0"], 'g', 6))
                         .arg(QString::number(coefficients["a1"], 'g', 6))
                         .arg(QString::number(coefficients["b0"], 'g', 6))
                         .arg(QString::number(coefficients["b1"], 'g', 6));
    }
    else if (mode == 2 || mode == 6){
        regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                             "B = %1<br>"
                             "B<sub>0</sub> = %2<br>"
                             "B<sub>1</sub> = %3<br>"
                             "<br>"
                             "<b>Коэффициенты:</b><br>"
                             "b<sub>0</sub> = %4<br>"
                             "b<sub>1</sub> = %5<br>"
                             "<br>"
                             "a<sub>0</sub> = %6<br>"
                             "a<sub>1</sub> = %7")
                         .arg(QString::number(coefficients["B"], 'g', 6))
                         .arg(QString::number(coefficients["B0"], 'g', 6))
                         .arg(QString::number(coefficients["B1"], 'g', 6))
                         .arg(QString::number(coefficients["b0"], 'g', 6))
                         .arg(QString::number(coefficients["b1"], 'g', 6))
                         .arg(QString::number(coefficients["a0"], 'g', 6))
                         .arg(QString::number(coefficients["a1"], 'g', 6));
    }
    else if (mode == 3 || mode == 5){
        regCoefStr = QString("<b>Вспомогательные величины:</b><br>"
                             "A = %1<br>"
                             "A<sub>0</sub> = %2<br>"
                             "A<sub>1</sub> = %3<br>"
                             "<br>"
                             "<b>Коэффициенты:</b><br>"
                             "a<sub>0</sub> = %4<br>"
                             "a<sub>1</sub> = %5<br>")
                         .arg(QString::number(coefficients["A"], 'g', 6))
                         .arg(QString::number(coefficients["A0"], 'g', 6))
                         .arg(QString::number(coefficients["A1"], 'g', 6))
                         .arg(QString::number(coefficients["a0"], 'g', 6))
                         .arg(QString::number(coefficients["a1"], 'g', 6));
    }
    // Заполнение итоговой функции
    switch(mode){
    case 0:{
        double a0 = coefficients["a0"];
        double a1 = coefficients["a1"];

        QString sign = (a1 >= 0) ? " + " : " - ";
        trend_eq = QString("y = %1 %2 %3 \u00B7 x")
                       .arg(QString::number(a0, 'g', 6))
                       .arg(sign)
                       .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 1:{
        double b0 = coefficients["b0"];
        double b1 = coefficients["b1"];

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        trend_eq = QString("x = %1 %2 %3 \u00B7 y")
                       .arg(QString::number(b0, 'g', 6))
                       .arg(sign_b)
                       .arg(QString::number(std::abs(b1), 'g', 6));
        break;
    }
    case 2:{
        double b0 = coefficients["b0"];
        double b1 = coefficients["b1"];

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        trend_eq = QString("lny = %1 %2 %3 \u00B7 x")
                       .arg(QString::number(b0, 'g', 6))
                       .arg(sign_b)
                       .arg(QString::number(std::abs(b1), 'g', 6));

        double a0 = coefficients["a0"];
        double a1 = coefficients["a1"];

        QString sign_a = (a1 >= 0) ? "" : " - ";
        trend_eq += QString(" или y = %1 \u00B7 e^(%2%3 \u00B7 x)")
                        .arg(QString::number(a0, 'g', 6))
                        .arg(sign_a)
                        .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 3:{
        double a0 = coefficients["a0"];
        double a1 = coefficients["a1"];

        QString sign_a = (a1 >= 0) ? " + " : " - ";
        trend_eq = QString("y = %1 %2 %3 \u00B7 z или y = %4 %5 %6/x")
                       .arg(QString::number(a0, 'g', 6))
                       .arg(sign_a)
                       .arg(QString::number(std::abs(a1), 'g', 6))
                       .arg(QString::number(a0, 'g', 6))
                       .arg(sign_a)
                       .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 5:{
        double a0 = coefficients["a0"];
        double a1 = coefficients["a1"];

        QString sign_a = (a1 >= 0) ? " + " : " - ";
        trend_eq = QString("y = %1 %2 %3 \u00B7 lnx")
                       .arg(QString::number(a0, 'g', 6))
                       .arg(sign_a)
                       .arg(QString::number(std::abs(a1), 'g', 6));
        break;
    }
    case 6:{
        double b0 = coefficients["b0"];
        double b1 = coefficients["b1"];

        QString sign_b = (b1 >= 0) ? " + " : " - ";
        trend_eq = QString("lny = %1 %2 %3 \u00B7 lnx")
                       .arg(QString::number(b0, 'f', 6))
                       .arg(sign_b)
                       .arg(QString::number(std::abs(b1), 'f', 6));

        double a0 = coefficients["a0"];
        double a1 = coefficients["a1"];

        QString powerPart;
        if (a1 >= 0) {
            // Без скобок
            powerPart = QString("x^%1").arg(QString::number(a1, 'f', 6));
        } else {
            // Со скобками вокруг степени
            powerPart = QString("x^(%1)").arg(QString::number(a1, 'f', 6));
        }

        trend_eq += QString(" или y = %1 \u00B7 %2")
                        .arg(QString::number(a0, 'f', 6))
                        .arg(powerPart);
        break;
    }
    default:
        break;
    }
}
// Получение выводов о коэффициенте Пирсона (коэффициенте корреляции)
void getCorCoefDescription(QString &r_descr, const double &r)
{
    if (r == 0)
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Характер связи:</b> <i>отсутствует</i>.<br>"
                          "<b>Интерпретация связи:</b> <i>-</i>.")
                      .arg(QString::number(r, 'f', 2));
    }
    else if (r > 0 && r < 1)
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Характер связи:</b> <i>вероятностная, прямая</i>.<br>"
                          "<b>Интерпретация связи:</b> <i>С увеличением X увеличивается Y</i>.")
                      .arg(QString::number(r, 'f', 2));
    }
    else if (r < 0 && r > -1)
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Характер связи:</b> <i>вероятностная, обратная</i>.<br>"
                          "<b>Интерпретация связи:</b> <i>С увеличением X уменьшается Y, и наоборот</i>.")
                      .arg(QString::number(r, 'f', 2));
    }
    else if (r == 1)
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Характер связи:</b> <i>функциональная, прямая</i>.<br>"
                          "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, с увеличением X увеличивается Y</i>.")
                      .arg(QString::number(r, 'f', 2));
    }
    else if (r == -1)
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Характер связи:</b> <i>функциональная, обратная</i>.<br>"
                          "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, увеличением X уменьшается Y, и наоборот</i>.")
                      .arg(QString::number(r, 'f', 2));
    }
    else
    {
        r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                          "<i>r = %1</i><br>"
                          "<b>Ошибка вычислений.</b>")
                      .arg(QString::number(r, 'f', 2));
        return;
    }

    // Характер связи (теснота)
    if (std::abs(r) < 0.3)
        r_descr += QString("<br><b>Теснота связи:</b> <i>Практически отсутствует</i>.");
    else if (std::abs(r) >= 0.3 && std::abs(r) < 0.5)
        r_descr += QString("<br><b>Теснота связи:</b> <i>Слабая</i>.");
    else if (std::abs(r) >= 0.5 && std::abs(r) < 0.7)
        r_descr += QString("<br><b>Теснота связи:</b> <i>Умеренная</i>.");
    else
        r_descr += QString("<br><b>Теснота связи:</b> <i>Сильная</i>.");
}

// --- Функции вычисления для соответствующего типа регрессии --- //
// 1)
// Вычисление коэффициентов линейной или обратной линейной регрессии
void getCoefsForLinearOrInverse(const bool& inverse, const int& n, QHash<QString, double> &values, QHash<QString, double> &coefficients){
    coefficients.clear();
    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1, B, B0, B1, b0, b1)
    // A
    coefficients[((inverse == false) ? "A" : "B")] = n * values["sumX2"] - values["sumX"] * values["sumX"];
    // A0
    coefficients[((inverse == false) ? "A0" : "B0")] = values["sumY"] * values["sumX2"] - values["sumX"] * values["sumX1Y"];
    // A1
    coefficients[((inverse == false) ? "A1" : "B1")] = n * values["sumX1Y"] - values["sumY"] * values["sumX"];
    // B
    coefficients[((inverse == false) ? "B" : "A")] = n * values["sumY2"] - values["sumY"] * values["sumY"];
    // B0
    coefficients[((inverse == false) ? "B0" : "A0")] = values["sumX"] * values["sumY2"] - values["sumY"] * values["sumX1Y"];
    // B1
    coefficients[((inverse == false) ? "B1" : "A1")] = n * values["sumX1Y"] - values["sumY"] * values["sumX"];
    // a0
    coefficients["a0"] = coefficients["A0"] / coefficients["A"];
    // a1
    coefficients["a1"] = coefficients["A1"] / coefficients["A"];
    // b0
    coefficients["b0"] = coefficients["B0"] / coefficients["B"];
    // b1
    coefficients["b1"] = coefficients["B1"] / coefficients["B"];
    // r
    if (inverse) values["r"] = coefficients["B1"] / std::sqrt(coefficients["A"] * coefficients["B"]);
    else values["r"] = coefficients["A1"] / std::sqrt(coefficients["A"] * coefficients["B"]);
}
// Вычисление для линейной и обратной линейной регрессии несмещенной дисперсии, коэф. детерминацции, MSE и т.д. + вывод
void calculateLinearOrInverseRegression(const int &mode, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                        QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                        const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, coefficients, regCoefStr, trend_eq);

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    // Заполнение описания коэффициента Пирсона
    getCorCoefDescription(r_descr, values["r"]);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Sx2"] = 1.0 / (n - 1.0) * (values["sumX2"] - 1.0 / n * std::pow(values["sumX"], 2));
    values["Sy2"] = 1.0 / (n - 1.0) * (values["sumY2"] - 1.0 / n * std::pow(values["sumY"], 2));
    values["SxMean"] = std::sqrt(values["Sx2"])/std::sqrt(n);
    values["SyMean"] = std::sqrt(values["Sy2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double x{ numericDates[i] };
        double y{ cursValues[i] };
        double y_pred{};
        (mode != 1) ? y_pred = coefficients["a0"] + coefficients["a1"] * x
                    : y_pred = coefficients["b0"] + coefficients["b1"] * x;
        predicts.append(y_pred);

        // Вычисление величин по предсказаниям
        values["Sost"] += std::pow(y - y_pred, 2);
        values["Sregr"] += std::pow(y_pred - values["meanY"], 2);

    }
    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}
// 2)
// Метод Гаусса для полиномиальных регрессий (параболическая, полиномиальная)
bool gaussSolve(QVector<QVector<double>> &A, QVector<double> &B, QVector<double> &solve, const double &eps){
    int n = A.size();

    // Прямой ход (обнуление элементов под главной диагональю)
    for (int k=0; k<n; ++k){
        // Поиск опорного элемента (pivot)
        // Чем он больше, тем устойчивее алгоритм
        int maxRow = k;
        for (int i=k+1; i<n; ++i){
            if (std::abs(A[i][k]) > std::abs(A[maxRow][k]))
                maxRow = i;
        }
        A.swapItemsAt(k, maxRow);  // Меняем элемнты k и maxRow строк в матрице A
        std::swap(B[k], B[maxRow]);// Меняем элементы k и maxRow в векторе-столбце B

        // Опорный элемент
        double pivot = A[k][k];
        // Проверка на вырожденность СЛАУ (бесконечно много решений), несовместность (решений нет)
        if (std::abs(pivot) < eps){
            QMessageBox::critical(nullptr, "Ошибка", "Система не имеет единственного решения!");
            return false;
        }
        // Делим элементы строки с максимальным элементом на pivot
        for (int j = k; j < n; ++j)
            A[k][j] /= pivot; // при j=k эл-т главной диагонали становится равен 1
        B[k] /= pivot;

        // Обнуляем элементы под главной диагональю
        for (int i = k+1; i < n; ++i)
        {
            double factor = A[i][k];
            for (int j = k; j < n; ++j)
                A[i][j] -= factor * A[k][j];
            B[i] -= factor * B[k];
        }
    }

    // Обратный ход
    for (int i = n-1; i >= 0; --i)
    {
        solve[i] = B[i];
        for (int j = i+1; j < n; ++j)
            solve[i] -= A[i][j] * solve[j];
    }

    return true;
}
bool buildAndSolvePolynomRegression(const QHash<QString, double> &values, QHash<QString, double> &coefficients, const int &n, const int &degree){
    int m = degree + 1;                                     // Степень полинома
    QVector<QVector<double>> A(m, QVector<double>(m, 0.0)); // Матрица А (левая часть СЛАУ)
    QVector<double> B(m, 0.0);                              // Вектор-столбец B (правая часть системы)

    // Построение матрицы A
    for (int j=0; j<m; ++j){
        for (int k=0; k<m; ++k){
            if (j==0 && k==0)
                A[j][k] = n;
            else{
                int power = j+k;
                QString tag{"sumX"};
                if (power != 1)
                    tag += QString::number(power);
                A[j][k] = values[tag];
            }
        }
    }

    // Построение вектора-столбца B
    for (int j=0; j<m; ++j){
        QString tag{"sum"};
        if (j == 0)
            tag += "Y";
        else
            tag += "X" + QString::number(j) + "Y";
        B[j] = values[tag];
    }

    // Решение СЛАУ методом Гаусса
    QVector<double> solve(A.size());
    if (!gaussSolve(A, B, solve))
        return false;

    for (int i=0; i<m; ++i){
        QString tag = QString("a%1").arg(i);
        coefficients[tag] = solve[i];
    }

    return true;
}
// Вычисление для полиномальных регрессиий (параболическая, полиномиальная) несмещенной дисперсии, коэф. детерминации, MSE и т.д. + вывод
void calculatePolynomRegression(const int& degree, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    // Заполнение данных о коэффициентах
    regCoefStr = "<b>Коэффициенты:</b><br>";
    // Заполнение итоговой функции
    trend_eq = "y = ";

    for (int i = 0; i <= degree; ++i)
    {
        QString key = QString("a%1").arg(i);
        double coef = coefficients[key];

        regCoefStr += QString("a<sub>%1</sub> = %2<br>")
                          .arg(i)
                          .arg(QString::number(coef, 'f', 6));

        QString coefStr = QString::number(coef, 'f', 6);
        if (i == 0)
            trend_eq += coefStr;
        else
        {
            if (coef >= 0)
                trend_eq += " + ";
            else
                trend_eq += " - ";
            trend_eq += QString("%1 \u00B7 x").arg(std::abs(coef));
            if (i > 1) trend_eq += toSuperscript(i);
        }
    }

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    r_descr = QString("<b>Значение линейного коэффициента связи:</b><br>"
                      "<i>r = NaN</i><br>"
                      "<b>Характер связи:</b> <i>-</i>.<br>"
                      "<b>Интерпретация связи:</b> <i>-</i>."
                      "<br><b>Теснота связи:</b> <i>-</i>.");

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Sx2"] = 1.0 / (n - 1.0) * (values["sumX2"] - 1.0 / n * std::pow(values["sumX"], 2));
    values["Sy2"] = 1.0 / (n - 1.0) * (values["sumY2"] - 1.0 / n * std::pow(values["sumY"], 2));
    values["SxMean"] = std::sqrt(values["Sx2"])/std::sqrt(n);
    values["SyMean"] = std::sqrt(values["Sy2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double x{ numericDates[i] };
        double y{ cursValues[i] };
        double y_pred{0.0};

        for (int j = 0; j <= degree; ++j)
            y_pred += coefficients[QString("a%1").arg(j)] * std::pow(x, j);
        predicts.append(y_pred);

        values["Sost"] += std::pow(y - y_pred, 2);                // остаточная сумма
        values["Sregr"] += std::pow(y_pred - values["meanY"], 2); // регрессионная сумма
    }

    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}
// 3)
void calculateCorrelationCoefficientByClassic(const int& n, const QVector<double> &x_values, const QVector<double> &y_values,
                                              const double &meanForX, const double &meanForY, double &r){
    double numerator = 0.0;
    double sum_dx2 = 0.0;
    double sum_dy2 = 0.0;

    for (int i = 0; i < n; ++i) {
        double dx = x_values[i] - meanForX;
        double dy = y_values[i] - meanForY;

        numerator += dx * dy;
        sum_dx2 += dx * dx;
        sum_dy2 += dy * dy;
    }

    r = numerator / std::sqrt(sum_dx2 * sum_dy2);
}
// 4)
// Вычисление коэффициентов для экспоненциальной регрессии
void getCoefsForExponential(const int& n, const QVector<double> &numericDates, const QVector<double> &lny,
                            QHash<QString, double> &values, QHash<QString, double> &coefficients){
    coefficients.clear();
    // Решение СЛУ методом Крамера (B, B0, B1, b0, b1, a0, a1)
    // B
    coefficients["B"] = n * values["sumX2"] - values["sumX"] * values["sumX"];
    // B0
    coefficients["B0"] = values["sumLNY"] * values["sumX2"] - values["sumX"] * values["sumXLNY"];
    // B1
    coefficients["B1"] = n * values["sumXLNY"] - values["sumLNY"] * values["sumX"];
    // b0
    coefficients["b0"] = coefficients["B0"] / coefficients["B"];
    // b1
    coefficients["b1"] = coefficients["B1"] / coefficients["B"];
    // a0
    coefficients["a0"] = std::exp(coefficients["b0"]);
    // a1
    coefficients["a1"] = coefficients["b1"];
    // r
    calculateCorrelationCoefficientByClassic(n, numericDates, lny, values["meanX"], values["meanLNY"], values["r"]);
}
// Вычисление для экспонениальной регрессии несмещенной дисперсии, коэф. детерминацции, MSE и т.д. + вывод
void calculateExponentialRegression(const QVector<double> &numericDates, const QVector<double> &lny,
                                    QVector<double> &predicts, QVector<double> &y_predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                    const int& n, const int&mode, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, coefficients, regCoefStr, trend_eq);

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    // Заполнение описания коэффициента Пирсона
    getCorCoefDescription(r_descr, values["r"]);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Sx2"] = 1.0 / (n - 1.0) * (values["sumX2"] - 1.0 / n * std::pow(values["sumX"], 2));
    values["Slny2"] = 1.0 / (n - 1.0) * (values["sumLNY2"] - 1.0 / n * std::pow(values["sumLNY"], 2));
    values["SxMean"] = std::sqrt(values["Sx2"])/std::sqrt(n);
    values["SlnyMean"] = std::sqrt(values["Slny2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double x{ numericDates[i] };
        double lny_pred = coefficients["b0"] + coefficients["b1"] * x;
        predicts.append(lny_pred);
        y_predicts.append(coefficients["a0"]*std::exp(coefficients["a1"]*x));

        // Вычисление величин по предсказаниям
        values["Sost"] += std::pow(lny[i] - lny_pred, 2);
        values["Sregr"] += std::pow(lny_pred - values["meanLNY"], 2);

    }
    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}
// 5)
// Вычисление коэффициентов для гиперболической регрессии
void getCoefsForHyperbolic(const int& n, const QVector<double> &z, const QVector<double> &cursValues,
                               QHash<QString, double> &values, QHash<QString, double> &coefficients){
    coefficients.clear();
    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1)
    // A
    coefficients["A"] = n * values["sumZ2"] - values["sumZ"] * values["sumZ"];
    // A0
    coefficients["A0"] = values["sumY"] * values["sumZ2"] - values["sumZ"] * values["sumZY"];
    // A1
    coefficients["A1"] = n * values["sumZY"] - values["sumY"] * values["sumZ"];
    // a0
    coefficients["a0"] = coefficients["A0"] / coefficients["A"];
    // a1
    coefficients["a1"] = coefficients["A1"] / coefficients["A"];
    // r
    calculateCorrelationCoefficientByClassic(n, z, cursValues, values["meanZ"], values["meanY"], values["r"]);
}
// Вычисление для гиперболической регрессии несмещенной дисперсии, коэф. детерминацции, MSE и т.д. + вывод
void calculateHyperbolicRegression(const QVector<double> &z, const QVector<double> &cursValues,
                                   QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                   const int& n, const int& mode, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, coefficients, regCoefStr, trend_eq);

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    // Заполнение описания коэффициента Пирсона
    getCorCoefDescription(r_descr, values["r"]);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Sz2"] = 1.0 / (n - 1.0) * (values["sumZ2"] - 1.0 / n * std::pow(values["sumZ"], 2));
    values["Sy2"] = 1.0 / (n - 1.0) * (values["sumY2"] - 1.0 / n * std::pow(values["sumY"], 2));
    values["SzMean"] = std::sqrt(values["Sz2"])/std::sqrt(n);
    values["SyMean"] = std::sqrt(values["Sy2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double y{ cursValues[i] };
        double y_pred = coefficients["a0"] + coefficients["a1"] * z[i];
        predicts.append(y_pred); //Т.к. z[i] = 1/x[i]

        // Вычисление величин по предсказаниям
        values["Sost"] += std::pow(y - y_pred, 2);
        values["Sregr"] += std::pow(y_pred - values["meanY"], 2);

    }
    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}
// 6)
// Вычисление коэффициентов для логарифмической регрессии
void getCoefsForLogarithmic(const int& n, const QVector<double> &lnx, const QVector<double> &cursValues,
                            QHash<QString, double> &values, QHash<QString, double> &coefficients){
    coefficients.clear();
    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1)
    // A
    coefficients["A"] = n * values["sumLNX2"] - values["sumLNX"] * values["sumLNX"];
    // A0
    coefficients["A0"] = values["sumY"] * values["sumLNX2"] - values["sumLNX"] * values["sumLNXY"];
    // A1
    coefficients["A1"] = n * values["sumLNXY"] - values["sumY"] * values["sumLNX"];
    // a0
    coefficients["a0"] = coefficients["A0"] / coefficients["A"];
    // a1
    coefficients["a1"] = coefficients["A1"] / coefficients["A"];
    // r
    calculateCorrelationCoefficientByClassic(n, lnx, cursValues, values["meanLNX"], values["meanY"], values["r"]);
}
// Вычисление для логарифмической регрессии несмещенной дисперсии, коэф. детерминацции, MSE и т.д. + вывод
void calculateLogarithmicRegression(const QVector<double> &lnx, const QVector<double> &cursValues,
                                   QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                   const int& n, const int &mode, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, coefficients, regCoefStr, trend_eq);

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    // Заполнение описания коэффициента Пирсона
    getCorCoefDescription(r_descr, values["r"]);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Slnx2"] = 1.0 / (n - 1.0) * (values["sumLNX2"] - 1.0 / n * std::pow(values["sumLNX"], 2));
    values["Sy2"] = 1.0 / (n - 1.0) * (values["sumY2"] - 1.0 / n * std::pow(values["sumY"], 2));
    values["SlnxMean"] = std::sqrt(values["Slnx2"])/std::sqrt(n);
    values["SyMean"] = std::sqrt(values["Sy2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double y{ cursValues[i] };
        double y_pred = coefficients["a0"] + coefficients["a1"] * lnx[i];
        predicts.append(y_pred);

        // Вычисление величин по предсказаниям
        values["Sost"] += std::pow(y - y_pred, 2);
        values["Sregr"] += std::pow(y_pred - values["meanY"], 2);

    }
    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}

// 7)
// Вычисление коэффициентов для степенной регрессии
void getCoefsForPower(const int& n, const QVector<double> &lnx, const QVector<double> &lny,
                      QHash<QString, double> &values, QHash<QString, double> &coefficients){
    coefficients.clear();
    // Решение СЛУ методом Крамера (B, B0, B1, b0, b1, a0, a1)
    // B
    coefficients["B"] = n * values["sumLNX2"] - values["sumLNX"] * values["sumLNX"];
    // B0
    coefficients["B0"] = values["sumLNY"] * values["sumLNX2"] - values["sumLNX"] * values["sumLNXLNY"];
    // B1
    coefficients["B1"] = n * values["sumLNXLNY"] - values["sumLNY"] * values["sumLNX"];
    // b0
    coefficients["b0"] = coefficients["B0"] / coefficients["B"];
    // b1
    coefficients["b1"] = coefficients["B1"] / coefficients["B"];
    // a0
    coefficients["a0"] = std::exp(coefficients["b0"]);
    // a1
    coefficients["a1"] = coefficients["b1"];
    // r
    calculateCorrelationCoefficientByClassic(n, lnx, lny, values["meanLNX"], values["meanLNY"], values["r"]);
}
// Вычисление для степенной регрессии несмещенной дисперсии, коэф. детерминацции, MSE и т.д. + вывод
void calculatePowerRegression(const QVector<double> &lnx, const QVector<double> &lny, const QVector<double> &numericDates,
                              QVector<double> &predicts, QVector<double> &y_predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                              const int& n, const int&mode, QString &trend_eq, QString &r_descr, QString &regCoefStr){
    predicts.clear();

    // --- ТЕКСТОВЫЕ ПОЛЯ ---
    getFuncCoefDescr(mode, coefficients, regCoefStr, trend_eq);

    // --- КОЭФФИЦИЕНТ ПИРСОНА ---
    // Заполнение описания коэффициента Пирсона
    getCorCoefDescription(r_descr, values["r"]);

    // --- ДИСПЕРСИЯ И ОТКЛОНЕНИЯ ---
    values["Slnx2"] = 1.0 / (n - 1.0) * (values["sumLNX2"] - 1.0 / n * std::pow(values["sumLNX"], 2));
    values["Slny2"] = 1.0 / (n - 1.0) * (values["sumLNY2"] - 1.0 / n * std::pow(values["sumLNY"], 2));
    values["SlnxMean"] = std::sqrt(values["Slnx2"])/std::sqrt(n);
    values["SlnyMean"] = std::sqrt(values["Slny2"])/std::sqrt(n);

    // --- ПРЕДСКАЗАНИЯ И СУММЫ КВАДРАТОВ ---
    values["Sost"] = 0.0;   // остаточная сумма
    values["Sregr"] = 0.0;  // регрессионная сумма
    values["Spoln"] = 0.0;  // полная сумма

    for (int i = 0; i < n; ++i)
    {
        double x{ numericDates[i] };
        double lny_pred = coefficients["b0"] + coefficients["b1"] * lnx[i];
        predicts.append(lny_pred);
        y_predicts.append(coefficients["a0"]*std::pow(x, coefficients["a1"]));

        // Вычисление величин по предсказаниям
        values["Sost"] += std::pow(lny[i] - lny_pred, 2);
        values["Sregr"] += std::pow(lny_pred - values["meanLNY"], 2);

    }
    values["Spoln"] = values["Sost"] + values["Sregr"];
    values["R2"] = (values["Spoln"] != 0.0) ? (1.0 - values["Sost"] / values["Spoln"]) : 0.0; // Sregr/Spoln
    values["MSE"] = values["Sost"] / n;
}
