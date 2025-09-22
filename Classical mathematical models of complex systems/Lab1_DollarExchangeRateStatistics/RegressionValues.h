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
    // Суммы для R2 и проверки регрессии
    double sumOst = 0.0;
    double sumRegr = 0.0;
    double sumFull = 0.0;
    // n
    int n = 0;
    // Несмещенная дисперсия и среднее квадратическое отклонение выборочного срденего
    double Sx2 = 0.0;
    double Sy2 = 0.0;
    double meanSx = 0.0;
    double meanSy = 0.0;
};

#endif // REGRESSIONVALUES_H
