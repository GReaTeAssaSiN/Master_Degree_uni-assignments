#ifndef MYFUNCS_H
#define MYFUNCS_H
#include "QString"
#include "QTableView"
#include "QVector"
#include "RegressionValues.h"

// ----- Общие функии ----- //
void centerTableItems(QTableView *tableView);
bool loadDataFromExcel(const QString &filePath, QTableView *tableView);
void readDataAndCurs(const QTableView *tableView,
                     QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues);
void calculateRegressionTotalValues(const QVector<double> numericDates, const QVector<double> &cursValues,
                                    QVector<double> &xSquared, QVector<double> &ySquared, QVector<double> &xyProduct,
                                    RegressionValues &values);

// ----- Функции общих вычислений ----- //
bool calculateRegressionCalcValues(const QVector<double> &cursValues,
                                   const QVector<double> &yT,
                                   QVector<double> &Sost, QVector<double> &Sregr, QVector<double> Sfull,
                                   RegressionValues &values, const double eps=1e-7);
void fillTotalTable(QTableView *tableView,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<double> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct);
void fillCalculateTable(QTableView *tableView,
                        const QVector<double> &numericDates,
                        const QVector<double> &cursValues,
                        const QVector<double> &yT,
                        const RegressionValues &values);

// ----- Расчет значений для регрессионной модели ----- //
// ОБЩЕЕ
QString getRegressionRelationship(const double& r);
QString getDeterminationDescription(const double& R2);
// РЕГРЕССИИ
bool calculateLinearRegressionValues(const QVector<double> &numericDates, QVector<double> &yT, RegressionValues &values, const double eps=1e-9);

#endif // MYFUNCS_H
