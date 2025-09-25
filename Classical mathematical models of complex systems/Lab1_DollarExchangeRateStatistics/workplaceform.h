#ifndef WORKPLACEFORM_H
#define WORKPLACEFORM_H

#include <QWidget>
#include <QTableView>
#include <QHash>
#include <QDate>

namespace Ui {
class WorkplaceForm;
}

class WorkplaceForm : public QWidget
{
    Q_OBJECT

public:
    explicit WorkplaceForm(const int &mode, const QVector<QString> &dataColumns, const QVector<double> &numericDates, const QVector<double> &cursValues,
                           QWidget *parent = nullptr);
    ~WorkplaceForm();

signals:
    void backToMain();

private slots:
    void on_pushButton_back_clicked();
    void MakePlot();
    void MakeInversePlot();
    void on_calculate_pushButton_clicked();
    void on_clean_pushButton_clicked();

private:
    Ui::WorkplaceForm *ui;
    // Исходные данные
    int mode{}; // Тип регрессии
    QVector<QString> dataColumns{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};

    // Переменные и структуры для хранения значений при построении регрессии
    int n{};                                           // Количество экспериментов
    QHash<QString, QVector<double>> vector_values{};   // Все вычисляемые значения в виде векторов
    QHash<QString, double> values{};                   // Все вычисляемые значения из значений вычисляемых векторов
    QHash<QString, double> coefficients{};             // Все коэффициенты регрессии + MSE

    // Вспомогательные ременные для хранения значений, связанных с выбором даты и графиком
    QDate select_date{}; // Выбранная дата
    QDate default_date{}; // Дата по умолчанию
    QString trend_eq{}; // Уравнение регрессии

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // WORKPLACEFORM_H
