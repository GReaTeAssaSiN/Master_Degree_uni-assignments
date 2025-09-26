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

    return true;
}

// ----- Функции общих вычислений ----- //
//Вычисление значений, необходимых для построения регрессии
void calculateRegressionTotalValues(const int& mode, const QVector<double> &numericDates, const QVector<double> &cursValues, int &n,
                                    QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values)
{
    n = numericDates.size();
    if (n==0) return;

    if (mode == 0 || mode == 1)
    {
        vector_values["x2"].clear();
        vector_values["y2"].clear();
        vector_values["xy"].clear();
        values["sumX"] = values["sumY"] = values["sumX2"] = values["sumY2"] = values["sumXY"] = 0.0;

        for (int i = 0; i < n; ++i){
            double x = numericDates[i];
            double y = cursValues[i];

            double x2 = x * x;
            double y2 = y * y;
            double xy = x * y;

            vector_values["x2"].append(x2);
            vector_values["y2"].append(y2);
            vector_values["xy"].append(xy);

            values["sumX"] += x;
            values["sumY"] += y;
            values["sumX2"] += x2;
            values["sumY2"] += y2;
            values["sumXY"] += xy;
        }

        values["meanX"] = values["sumX"] / n;
        values["meanY"] = values["sumY"] / n;
    }
    else if (mode == 2){
        vector_values["lny"].clear();
        vector_values["x2"].clear();
        vector_values["lny2"].clear();
        vector_values["xlny"].clear();

        values["sumX"] = values["sumLnY"] = values["sumX2"] = values["sumLnY2"] = values["sumXLnY"] = 0.0;

        for (int i = 0; i < n; ++i){
            double x = numericDates[i];
            double lny = std::log(cursValues[i]);

            double x2 = x * x;
            double lny2 = lny * lny;
            double xlny = x * lny;

            vector_values["lny"].append(lny);
            vector_values["x2"].append(x2);
            vector_values["lny2"].append(lny2);
            vector_values["xlny"].append(xlny);

            values["sumX"] += x;
            values["sumLnY"] += lny;
            values["sumX2"] += x2;
            values["sumLnY2"] += lny2;
            values["sumXLnY"] += xlny;
        }

        values["meanX"] = values["sumX"] / n;
        values["meanLnY"] = values["sumLnY"] / n;
    }
}
//Вычисление значений, необходимых для проверки регрессии
bool calculateRegressionCalcValues(const int& mode, const int& n,
                                   const QVector<double> &cursValues,
                                   QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values,
                                   QHash<QString, double> &coefficients, const double eps)
{
    vector_values["Sost"].clear();
    vector_values["Sregr"].clear();
    vector_values["Sfull"].clear();

    values["sumOst"] = values["sumRegr"] = values["sumFull"] = 0.0;

    for (int i = 0; i < n; ++i){
        double elem_of_Sost{}, elem_of_Sregr{}, elem_of_Sfull{};
        if (mode == 0 || mode == 1){
            elem_of_Sost = std::pow((mode != 1) ? (cursValues[i] - vector_values["yT"][i]) : (cursValues[i] - vector_values["xT"][i]), 2);
            elem_of_Sregr = std::pow((mode != 1) ? (vector_values["yT"][i] - values["meanY"]) : (vector_values["xT"][i] - values["meanX"]), 2);
            elem_of_Sfull = std::pow((mode != 1) ? (cursValues[i] - values["meanY"]) : (cursValues[i] - values["meanX"]), 2);
        }
        else if (mode == 2){
            elem_of_Sost = std::pow(vector_values["lny"][i] - vector_values["lnyT"][i], 2);
            elem_of_Sregr = std::pow(vector_values["lnyT"][i] - values["meanLnY"], 2);
            elem_of_Sfull = std::pow(vector_values["lny"][i] - values["meanLnY"], 2);
        }

        vector_values["Sost"].append(elem_of_Sost);
        vector_values["Sregr"].append(elem_of_Sregr);
        vector_values["Sfull"].append(elem_of_Sfull);

        values["sumOst"] += elem_of_Sost;
        values["sumRegr"] += elem_of_Sregr;
        values["sumFull"] += elem_of_Sfull;
    }

    // Коэффииент детерминации и MSE
    coefficients["R2"] =  1 - values["sumOst"] / values["sumFull"];
    coefficients["MSE"] =  values["sumOst"] / n;

    // Несмещенная дисперсия и среднее кв. отклонение выборочнного среднего
    if (mode == 0 || mode == 1){
        values["Sx2"] = 1.0 / (n - 1.0) * (values["sumX2"] - 1.0 / n * std::pow(values["sumX"], 2));
        values["Sy2"] = 1.0 / (n - 1.0) * (values["sumY2"] - 1.0 / n * std::pow(values["sumY"], 2));
        values["meanSx"] = std::sqrt(values["Sx2"] / n);
        values["meanSy"] = std::sqrt(values["Sy2"] / n);
    }
    else if (mode == 2){
        values["Sx2"] = 1.0 / (n - 1.0) * (values["sumX2"] - 1.0 / n * std::pow(values["sumX"], 2));
        values["Slny2"] = 1.0 / (n - 1.0) * (values["sumLnY2"] - 1.0 / n * std::pow(values["sumLnY"], 2));
        values["meanSx"] = std::sqrt(values["Sx2"] / n);
        values["meanSlny"] = std::sqrt(values["Slny2"] / n);
    }

    return std::abs(values["sumOst"] + values["sumRegr"] - values["sumFull"]) < eps;
}

//Заполнение общей таблицы значениями
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QHash<QString, QVector<double>> &vector_values)
{
    int rowCount = numericDates.size();
    int colCount{};
    QStandardItemModel *model{};

    if (mode == 0 || mode == 1) {
        colCount = 6; // data, xi, yi, xi^2, yi^2, xi*yi или data, yi, xi, yi^2, xi^2, yi*xi

        model = new QStandardItemModel(rowCount, colCount, tableView);

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
            model->setItem(r, 3, new QStandardItem(QString::number((mode != 1) ? vector_values["x2"][r] : vector_values["y2"][r], 'f', 5)));
            model->setItem(r, 4, new QStandardItem(QString::number((mode != 1) ? vector_values["y2"][r] : vector_values["x2"][r], 'f', 5)));
            model->setItem(r, 5, new QStandardItem(QString::number(vector_values["xy"][r], 'f', 5)));
        }
    }
    else if (mode == 2){
        colCount = 7; // data, xi, yi, lnyi, xi^2, (lnyi)^2, xi*lnyi

        model = new QStandardItemModel(rowCount, colCount, tableView);

        // Заголовки
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "x\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "y\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "lny\u1D62");
        model->setHeaderData(4, Qt::Horizontal, "x\u1D62\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "(lny\u1D62)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "x\u1D62 \u00B7 lny\u1D62");

        for (int r = 0; r < rowCount; ++r) {
            model->setItem(r, 0, new QStandardItem(dataColumn[r]));
            model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 5)));
            model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 5)));
            model->setItem(r, 3, new QStandardItem(QString::number(vector_values["lny"][r], 'f', 5)));
            model->setItem(r, 4, new QStandardItem(QString::number(vector_values["x2"][r], 'f', 5)));
            model->setItem(r, 5, new QStandardItem(QString::number(vector_values["lny2"][r], 'f', 5)));
            model->setItem(r, 6, new QStandardItem(QString::number(vector_values["xlny"][r], 'f', 5)));
        }
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
                        const QHash<QString, QVector<double>> &vector_values,
                        const QHash<QString, double> &values)
{
    int rowCount = numericDates.size();
    int colCount{};
    QStandardItemModel *model{};

    if (mode == 0 || mode == 1){
        colCount = 7; // data, xi, yi, yi^T, (yi-yi^T)^2, (yi^T - mean(y))^2, (yi - mean(y))^2
                      // data, yi, xi, xi^T, (xi-xi^T)^2, (xi^T - mean(x))^2, (xi - mean(x))^2

        model = new QStandardItemModel(rowCount, colCount, tableView);

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
            model->setItem(r, 3, new QStandardItem(QString::number((mode != 1) ? vector_values["yT"][r] : vector_values["xT"][r], fmt, sign_count)));
            model->setItem(r, 4, new QStandardItem(QString::number((mode != 1) ? std::pow(cursValues[r]-vector_values["yT"][r], 2) : std::pow(numericDates[r]-vector_values["xT"][r], 2), fmt, sign_count)));
            model->setItem(r, 5, new QStandardItem(QString::number((mode != 1) ? std::pow(vector_values["yT"][r]-values["meanY"], 2) : std::pow(vector_values["xT"][r]-values["meanX"], 2), fmt, sign_count)));
            model->setItem(r, 6, new QStandardItem(QString::number((mode != 1) ? std::pow(cursValues[r]-values["meanY"], 2) : std::pow(numericDates[r]-values["meanX"], 2), fmt, sign_count)));
        }
    }
    else if (mode == 2){
        colCount = 7; // data, xi, yi, lnyi^T, (lnyi-lnyi^T)^2, (lnyi^T - mean(lny))^2, (lnyi - mean(lny))^2

        model = new QStandardItemModel(rowCount, colCount, tableView);

        // Заголовки
        model->setHeaderData(0, Qt::Horizontal, "data");
        model->setHeaderData(1, Qt::Horizontal, "x\u1D62");
        model->setHeaderData(2, Qt::Horizontal, "y\u1D62");
        model->setHeaderData(3, Qt::Horizontal, "(lny\u1D62)\u1D40");
        model->setHeaderData(4, Qt::Horizontal, "(lny\u1D62 - (lny\u1D62)\u1D40)\u00B2");
        model->setHeaderData(5, Qt::Horizontal, "((lny\u1D62)\u1D40 - l\u0305n\u0305y\u0305)\u00B2");
        model->setHeaderData(6, Qt::Horizontal, "(lny\u1D62 - l\u0305n\u0305y\u0305)\u00B2");

        for (int r = 0; r < rowCount; ++r) {
            model->setItem(r, 0, new QStandardItem(dataColumn[r]));
            model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r], 'f', 6)));
            model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));
            model->setItem(r, 3, new QStandardItem(QString::number(vector_values["lnyT"][r], 'f', 6)));
            model->setItem(r, 4, new QStandardItem(QString::number(std::pow(vector_values["lny"][r] - vector_values["lnyT"][r], 2), 'g', 6)));
            model->setItem(r, 5, new QStandardItem(QString::number(std::pow(vector_values["lnyT"][r]-values["meanLnY"], 2), 'g', 6)));
            model->setItem(r, 6, new QStandardItem(QString::number(std::pow(vector_values["lny"][r]-values["meanLnY"], 2), 'g', 6)));
        }
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}

// ----- Расчет значений для регрессионной модели ----- //
// ОБЩЕЕ
QString getRegressionRelationship(const int& mode, const double& r){
    QString regr_description{};
    if (r == 0) {
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>Отсутствует.</i><br>"
                                   "<b>Интерпретация связи:</b> <i>-.</i>")
                               .arg(QString::number(r, 'f', 2));
    }
    else if (r > 0 && r < 1) {
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>Вероятностная, прямая.</i><br>"
                                   "<b>Интерпретация связи:</b> <i>%2</i>")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "С увеличением X увеличивается Y."
                                                : "С увеличением Y увеличивается X.");
    }
    else if (r < 0 && r > -1){
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>Вероятностная, обратная.</i><br>"
                                   "<b>Интерпретация связи:</b> <i>%2</i>")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "С увеличением X уменьшается Y."
                                                : "С увеличением Y уменьшается X.");
    }
    else if (r == 1){
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>Функциональная, прямая.</i><br>"
                                   "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, %2</i>")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "с увеличением X увеличивается Y."
                                                : "с увеличением Y увеличивается X.");
    }
    else if (r == -1){
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Характер связи:</b> <i>Функциональная, обратная.</i><br>"
                                   "<b>Интерпретация связи:</b> <i>Каждому значению факторного признака строго соответствует одно значение функции, %2</i>")
                               .arg(QString::number(r, 'f', 2))
                               .arg((mode != 1) ? "с увеличением X уменьшается Y, и наоборот."
                                                : "с увеличением Y уменьшается X, и наоборот.");
    }
    else{
        regr_description = QString("<b>Значение линейного коэффициента связи:</b><br><i>r = %1</i><br>"
                                   "<b>Ошибка вычислений.</b>")
                               .arg(QString::number(r, 'f', 2));
        return regr_description;
    }

    // Характер силы связи
    if (std::abs(r) < 0.3)
        regr_description += QString("<br><b>Характер силы связи:</b> <i>Практически отсутствует.</i>");
    else if (std::abs(r) >= 0.3 && std::abs(r) < 0.5)
        regr_description += QString("<br><b>Характер силы связи:</b> <i>Слабая.</i>");
    else if (std::abs(r) >= 0.5 && std::abs(r) < 0.7)
        regr_description += QString("<br><b>Характер силы связи:</b> <i>Умеренная.</i>");
    else
        regr_description += QString("<br><b>Характер силы связи:</b> <i>Сильная.</i>");

    return regr_description;
}
QString getDeterminationDescription(const double& R2){
    QString determination_descr = "<b>Можно ли использовать для прогнозирования (R\u00B2>=75%):</b> ";
    if (R2 * 100 >= 75)
        determination_descr += "<i>Да!</i>";
    else
        determination_descr += "<i>Не рекомендуется!</i>";
    return determination_descr;
}
void calculateCorrelationCoefficientByClassic(const int& n, const QVector<double> &numericDates, double &r,
                                              const QHash<QString, QVector<double>> &vector_values, const QHash<QString, double> &values){
    const double meanX = values["meanX"];
    const double meanLnY = values["meanLnY"];
    const QVector<double>& lny = vector_values["lny"];

    double numerator = 0.0;
    double sum_x2 = 0.0;
    double sum_lny2 = 0.0;

    for (int i = 0; i < n; ++i) {
        double dx = numericDates[i] - meanX;
        double dy = lny[i] - meanLnY;

        numerator += dx * dy;
        sum_x2 += dx * dx;
        sum_lny2 += dy * dy;
    }

    r = numerator / std::sqrt(sum_x2 * sum_lny2);
}
// ЛИНЕЙНАЯ И ОБРАТНАЯ ЛИНЕЙНАЯ РЕГРЕССИЯ
bool calculateLinearRegressionCoefficients(const int& n, const QHash<QString, double> &values, QHash<QString, double> &coefficients, const double eps){
    // y = a0 + a1*x                        // y = b0 + b1*x
    // Система:                             // Система:
    // {a0*n + a1*E(xi) = E(yi)             // {b0*n + b1*E(yi) = E(xi)
    // {a0*E(xi) + a1*E(xi^2) = E(xi*yi)    // {b0*E(yi) + b1*E(yi^2) = E(xi*yi)

    // Решение СЛУ методом Крамера (A, A0, A1, a0, a1, B, B0, B1, b0, b1)
    // A
    coefficients["A"] = n * values["sumX2"] - values["sumX"] * values["sumX"];
    // A0
    coefficients["A0"] = values["sumY"] * values["sumX2"] - values["sumX"] * values["sumXY"];
    // A1
    coefficients["A1"] = n * values["sumXY"] - values["sumY"] * values["sumX"];
    // B
    coefficients["B"] = n * values["sumY2"] - values["sumY"] * values["sumY"];
    // B0
    coefficients["B0"] = values["sumX"] * values["sumY2"] - values["sumY"] * values["sumXY"];
    // B1
    coefficients["B1"] = n * values["sumXY"] - values["sumY"] * values["sumX"];
    // a0
    coefficients["a0"] = coefficients["A0"] / coefficients["A"];
    // a1
    coefficients["a1"] = coefficients["A1"] / coefficients["A"];
    // b0
    coefficients["b0"] = coefficients["B0"] / coefficients["B"];
    // b1
    coefficients["b1"] = coefficients["B1"] / coefficients["B"];
    // r1
    coefficients["r1"] = coefficients["A1"] / std::sqrt(coefficients["A"] * coefficients["B"]);
    // r2
    coefficients["r2"] = coefficients["B1"] / std::sqrt(coefficients["A"] * coefficients["B"]);

    return std::abs(coefficients["r1"] - coefficients["r2"]) < eps;
}
void calculateLinearRegressionValues(const int &mode, const QVector<double> &numericDates, const int &n,
                                     QVector<double> &pred_vector_values, const QHash<QString, double> &coefficients){
    // yT или xT
    pred_vector_values.clear();
    for (int i=0; i<n; ++i){
        if (mode != 1)
            pred_vector_values.append(coefficients["a0"] + coefficients["a1"] * numericDates[i]);
        else
            pred_vector_values.append(coefficients["b0"] + coefficients["b1"] * numericDates[i]);
    }
}
void calculateInverseLinearRegressionValuesByDates(const QVector<double> &yReg, QVector<double> &xT,
                                                   const int &n, const QHash<QString, double> &coefficients){
    // xT - курс доллара США
    xT.clear();
    for (int i = 0; i < n; ++i){
        // [количество дней в секундах]/60/60/24/10000 -> [количество дней в днях]/10000
        xT.append((yReg[i]/60/60/24/10000 - coefficients["b0"]) / coefficients["b1"]);
    }
}
// ЭКСПОНЕНЦИАЛЬНАЯ РЕГРЕССИЯ
void calculateExponentialRegressionCoefficients(const int &n, const QHash<QString, double> &values, QHash<QString, double> &coefficients){
    // y = a0 * e^(a1*x)
    // lny = lna0 + a1*x
    // lny = bo + b1*x
    // Система:
    // {b0*n + b1*E(xi) = E(lnyi)
    // {b0*E(xi) + b1*E(xi^2) = E(xi*lnyi)

    // Решение СЛУ методом Крамера (B, B0, B1, b0, b1, a0, a1)
    // B
    coefficients["B"] = n * values["sumX2"] - values["sumX"] * values["sumX"];
    // B0
    coefficients["B0"] = values["sumLnY"] * values["sumX2"] - values["sumX"] * values["sumXLnY"];
    // B1
    coefficients["B1"] = n * values["sumXLnY"] - values["sumLnY"] * values["sumX"];
    // b0
    coefficients["b0"] = coefficients["B0"] / coefficients["B"];
    // b1
    coefficients["b1"] = coefficients["B1"] / coefficients["B"];
    // a0
    coefficients["a0"] = std::exp(coefficients["b0"]);
    // a1
    coefficients["a1"] = coefficients["b1"];
}
void calculateExponentialRegressionValues(const QVector<double> &numericDates, const int &n, QVector<double> &ln_pred_vector_values,
                                          QVector<double> &pred_vector_values, const QHash<QString, double> &coefficients){
    // lnyT и yT
    ln_pred_vector_values.clear();
    pred_vector_values.clear();
    for (int i=0; i<n; ++i){
        pred_vector_values.append(coefficients["a0"] * std::exp(coefficients["a1"] * numericDates[i]));
        ln_pred_vector_values.append(coefficients["b0"] + coefficients["b1"] * numericDates[i]);
    }
}
