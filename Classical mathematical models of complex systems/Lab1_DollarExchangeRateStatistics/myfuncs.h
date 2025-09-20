#ifndef MYFUNCS_H
#define MYFUNCS_H
#include "QString"
#include "QTableView"
#include "QVector"
#include "RegressionValues.h"

bool loadDataFromExcel(const QString &filePath, QTableView *tableView);

void readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<int> &numericDates, QVector<double> &cursValues);

void calculateRegressionValues(const QVector<int> numericDates, const QVector<double> &cursValues,
                               QVector<int> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                               RegressionValues &values);

void fillTotalTable(QTableView *tableView,
                    const QVector<QString> &dataColumn,
                    const QVector<int> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<int> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct);

#endif // MYFUNCS_H
