#ifndef MYFUNCS_H
#define MYFUNCS_H
#include "QString"
#include "QTableView"
#include "QVector"

// ----- Общие функии ----- //
void centerTableItems(QTableView *tableView);
QString toSuperscript(const int &number);

/*------------------
 |  Главное окно  |
 -----------------*/
bool loadDataFromExcel(const QString &filePath, QTableView *tableView);
bool readDataAndCurs(const QTableView *tableView, QVector<QString> &dataColumn, QVector<double> &numericDates, QVector<double> &cursValues);


/*------------------
 |     БЛОК 1      |
 -----------------*/
// --- Функции заполнения виджетов --- //
void fillTotalTableForPolynomialRegressions(QTableView *tableView, const int &mode, const int& n, const int& degree,
                                            const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                            const QHash<QString, QVector<double>> vector_values);
QString getColumnNameForNotPolynomialRegressions(const QString& key);
void fillTotalTableForNotPolynomialRegressions(QTableView *tableView, const int& n, const int& mode,
                                               const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                               const QHash<QString, QVector<double>> vector_values);
QString fillTextEditWithSumsForPolynomialRegression(const int &mode, const QHash<QString, double> &values, const int& degree);
QString getSumLHSForNotPolynomialRegressions(const QString& key);
QString fillTextEditWithSumsForNotPolynomialRegression(const int &mode, const QHash<QString, double> &values);

// --- Функции вычисления для соответствующего типа регрессии --- //
// Полиномиальные (линейная, обратная линейная, параболическая, полиномиальная) регрессии
void calculatePolynomialRegressionsSums(const QVector<double> &numericDates, const QVector<double> &cursValues,
                                        int &n, const int &degree, QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values);
// Неполиномиальные (экспоненциальная, гиперболическая, логарифмическая, степенная)
void calculateNotPolynomialRegressionsSums(const QVector<double> &numericDates, const QVector<double> &cursValues,
                                           int &n, const int& mode, QHash<QString, QVector<double>> &vector_values, QHash<QString, double> &values);

/*------------------
 |     БЛОК 2      |
 -----------------*/
// --- Функции заполнения виджетов и описания --- //
void fillCalculateTable(QTableView* tableView, const int &mode, const int &n,
                        const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                        const QVector<double> &predicts, const QHash<QString, double> &values);
void fillCalculateTableForExpOrPowerRegression(QTableView* tableView, const int &n,
                                               const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                               const QVector<double> &lny, const QVector<double> &predicts, const QHash<QString, double> &values);
void getCorCoefDescription(QString &r_descr, const double &r);

// --- Функции вычисления для соответствующего типа регрессии --- //
// Линейная и обратная линейная регрессия
void getCoefsForLinearOrInverse(const bool& inverse, const int& n, QHash<QString, double> &values, QHash<QString, double> &coefficients);
void calculateLinearOrInverseRegression(const bool &inverse, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                        QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                        const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
// Полиномиальные (параболическая, полиномиальная) регрессии
bool gaussSolve(QVector<QVector<double>> &A, QVector<double> &B, QVector<double> &solve, const double &eps = 1e-20);
bool buildAndSolvePolynomRegression(const QHash<QString, double> &values, QHash<QString, double> &coefficients, const int &n, const int &degree);
void calculatePolynomRegression(const int& degree, const QVector<double> &numericDates, const QVector<double> &cursValues,
                                QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
// Неполиномиальные регрессии (экспоненциальная, гиперболическая, логарифмическая, степенная)
void calculateCorrelationCoefficientByClassic(const int& n, const QVector<double> &x_values, const QVector<double> &y_values,
                                              const double &meanForX, const double &meanForY, double &r);
// Экспоненциальная регрессия
void getCoefsForExponential(const int& n, const QVector<double> &numericDates, const QVector<double> &lny,
                            QHash<QString, double> &values, QHash<QString, double> &coefficients);
void calculateExponentialRegression(const QVector<double> &numericDates, const QVector<double> &lny,
                                    QVector<double> &predicts, QVector<double> &y_predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                    const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
// Гиперболическая регрессия
void getCoefsForHyperbolic(const int& n, const QVector<double> &z, const QVector<double> &cursValues,
                           QHash<QString, double> &values, QHash<QString, double> &coefficients);
void calculateHyperbolicRegression(const QVector<double> &z, const QVector<double> &cursValues,
                                   QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                   const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
// Логарифмическая регрессия
void getCoefsForLogarithmic(const int& n, const QVector<double> &lnx, const QVector<double> &cursValues,
                            QHash<QString, double> &values, QHash<QString, double> &coefficients);
void calculateLogarithmicRegression(const QVector<double> &lnx, const QVector<double> &cursValues,
                                    QVector<double> &predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                                    const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
// Степенная регрессия
void getCoefsForPower(const int& n, const QVector<double> &lnx, const QVector<double> &lny,
                      QHash<QString, double> &values, QHash<QString, double> &coefficients);
void calculatePowerRegression(const QVector<double> &lnx, const QVector<double> &lny, const QVector<double> &numericDates,
                              QVector<double> &predicts, QVector<double> &y_predicts, QHash<QString, double> &coefficients, QHash<QString, double> &values,
                              const int& n, QString &trend_eq, QString &r_descr, QString &regCoefStr);
#endif // MYFUNCS_H
