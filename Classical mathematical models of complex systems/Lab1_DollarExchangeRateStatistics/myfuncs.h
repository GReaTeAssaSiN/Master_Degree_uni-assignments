#ifndef MYFUNCS_H
#define MYFUNCS_H
#include "QString"
#include "QTableView"
#include "QVector"

// ----- Общие функии ----- //
void centerTableItems(QTableView *tableView);
bool loadDataFromExcel(const QString &filePath, QTableView *tableView);
bool readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues);
void calculateRegressionTotalValues(const QVector<double> &numericDates, const QVector<double> &cursValues, int &n,
                                    QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values);

// ----- Функции общих вычислений ----- //
bool calculateRegressionCalcValues(const int& mode, const int &n,
                                   const QVector<double> &cursValues,
                                   QHash<QString, QVector<double> > &vector_values, QHash<QString, double> &values,
                                   QHash<QString, double> &coefficients, const double eps=1e-7);
void fillTotalTable(QTableView *tableView,
                    const int &mode,
                    const QVector<QString> &dataColumn,
                    const QVector<double> &numericDates,
                    const QVector<double> &cursValues,
                    const QHash<QString, QVector<double>> &vector_values);
void fillCalculateTable(QTableView *tableView,
                        const int &mode,
                        const QVector<QString> &dataColumn,
                        const QVector<double> &numericDates,
                        const QVector<double> &cursValues,
                        const QHash<QString, QVector<double>> &vector_values,
                        const QHash<QString, double> &values);

// ----- Расчет значений для регрессионной модели ----- //
// ОБЩЕЕ
QString getRegressionRelationship(const int &mode, const double& r);
QString getDeterminationDescription(const double& R2);
// РЕГРЕССИИ
// Линейная и обратная линейная регрессия
bool calculateLinearRegressionCoefficients(const int& n, const QHash<QString, double> &values, QHash<QString, double> &coefficients, const double eps=1e-9);
void calculateLinearRegressionValues(const int &mode, const QVector<double> &numericDates, const int &n,
                                     QVector<double> &pred_vector_values, const QHash<QString, double> &coefficients);
void calculateInverseLinearRegressionValuesByDates(const QVector<double> &numericDates, QVector<double> &xT,
                                                   const int &n, const QHash<QString, double> &coefficients);

#endif // MYFUNCS_H
