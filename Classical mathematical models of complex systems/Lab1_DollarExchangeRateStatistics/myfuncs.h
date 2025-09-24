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
bool calculateRegressionCalcValues(const int& mode,
                                   const QVector<double> &cursValues,
                                   const QVector<double> &yT,
                                   QVector<double> &Sost, QVector<double> &Sregr, QVector<double> Sfull,
                                   RegressionValues &values, QHash<QString, double> &coefficients, const double eps=1e-7);
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QVector<double> &xSquared,
                    const QVector<double> &ySquared,
                    const QVector<double> &xyProduct);
void fillCalculateTable(QTableView *tableView,
                        const int &mode,
                        const QVector<QString> &dataColumn,
                        const QVector<double> &numericDates,
                        const QVector<double> &cursValues,
                        const QVector<double> &yT,
                        const RegressionValues &values);

// ----- Расчет значений для регрессионной модели ----- //
// ОБЩЕЕ
QString getRegressionRelationship(const int &mode, const double& r);
QString getDeterminationDescription(const double& R2);
// РЕГРЕССИИ
// Линейная и обратная линейная регрессия
bool calculateLinearRegressionCoefficients(RegressionValues &values, QHash<QString, double> &coefficients, const double eps=1e-9);
void calculateLinearRegressionValues(const int &mode, const QVector<double> &numericDates, QVector<double> &yT,
                                     const int &n, const QHash<QString, double> &coefficients);
void calculateInverseLinearRegressionValuesByDates(const QVector<double> &numericDates, QVector<double> &xT,
                                                   const int &n, const QHash<QString, double> &coefficients);

#endif // MYFUNCS_H
