#include "myfuncs.h"
#include "xlsxdocument.h"
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QMessageBox>

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
        QMessageBox::warning(nullptr, "Предупреждение", "Файл пуст или данные отсутствуют (необходим формат .xlsx)!");
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

            if (cell) {
                QVariant cellValue = cell->value();

                if (col == 2 && cellValue.toDouble()) {
                    double excelDate = cellValue.toDouble();
                    QDate date = QDate(1900, 1, 1).addDays(int(excelDate) - 2); //1900г. считается високосным + QDate считает дни с 1
                    value = date.toString("dd.MM.yyyy");
                } else {
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

    // Информационное сообщение
    QMessageBox::information(nullptr, "Успех", "Данные успешно загружены!");

    return true;
}

void readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<int> &numericDates, QVector<double> &cursValues)
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
            numericDates.append(epoch.daysTo(date));
        } else {
            numericDates.append(0);
            qDebug() << "Некорректная дата:" << valData.toString();
        }

        // Конвертация курса в число
        QVariant valCurs = model->index(r, colCurs).data();
        bool ok;
        double curs = valCurs.toString().toDouble(&ok);
        if (ok) {
            cursValues.append(curs);
        } else {
            cursValues.append(0.0);
            qDebug() << "Некорректный курс:" << valCurs.toString();
        }
    }
}

void calculateRegressionValues(const QVector<int> numericDates, const QVector<double> &cursValues,
                               QVector<int> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                               RegressionValues &values)
{
    int n = numericDates.size();
    if (n==0) return;

    xSquared.clear();
    ySquared.clear();
    xyProduct.clear();

    values.sumX = values.sumY = values.sumX2 = values.sumY2 = values.sumXY = 0.0;

    for (int i = 0; i < n; ++i){
        int x = numericDates[i];
        double y = cursValues[i];

        int x2 = x * x;
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

    values.meanX = values.sumX / n;
    values.meanY = values.sumY / n;
}

void fillTotalTable(QTableView *tableView,
                    const QVector<QString> &dataColumn,
                    const QVector<int> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<int> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct)
{
    int rowCount = numericDates.size();
    int colCount = 6; // data, xi, yi, xi^2, yi^2, xi*yi

    QStandardItemModel *model = new QStandardItemModel(rowCount, colCount, tableView);

    // Заголовки
    model->setHeaderData(0, Qt::Horizontal, "data");
    model->setHeaderData(1, Qt::Horizontal, "xi");
    model->setHeaderData(2, Qt::Horizontal, "yi");
    model->setHeaderData(3, Qt::Horizontal, "xi^2");
    model->setHeaderData(4, Qt::Horizontal, "yi^2");
    model->setHeaderData(5, Qt::Horizontal, "xi*yi");

    for (int r = 0; r < rowCount; ++r) {
        model->setItem(r, 0, new QStandardItem(dataColumn[r]));
        model->setItem(r, 1, new QStandardItem(QString::number(numericDates[r])));
        model->setItem(r, 2, new QStandardItem(QString::number(cursValues[r], 'f', 6)));
        model->setItem(r, 3, new QStandardItem(QString::number(xSquared[r])));
        model->setItem(r, 4, new QStandardItem(QString::number(ySquared[r], 'f', 6)));
        model->setItem(r, 5, new QStandardItem(QString::number(xyProduct[r], 'f', 6)));
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}
