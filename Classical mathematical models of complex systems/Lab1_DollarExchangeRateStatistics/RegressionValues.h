#ifndef REGRESSIONVALUES_H
#define REGRESSIONVALUES_H

struct RegressionValues {
    // Суммы и средние значения
    double sumX = 0.0;
    double sumY = 0.0;
    double sumX2 = 0.0;
    double sumY2 = 0.0;
    double sumXY = 0.0;
    double meanX = 0.0;
    double meanY = 0.0;
    // Константы и коэффициенты
    int n = 0;
    double A = 0.0;
    double A0 = 0.0;
    double A1 = 0.0;
    double B = 0.0;
    double B0 = 0.0;
    double B1 = 0.0;
    double a0 = 0.0;
    double a1 = 0.0;
    double b0 = 0.0;
    double b1 = 0.0;
    double r1 = 0.0;
    double r2 = 0.0;
    // Суммы для R2 и проверки регрессии
    double sumOst = 0.0;
    double sumRegr = 0.0;
    double sumFull = 0.0;
    // Коэффииент детерминацции R2 и MSE
    double R2 = 0.0;
    double MSE = 0.0;
};

#endif // REGRESSIONVALUES_H
