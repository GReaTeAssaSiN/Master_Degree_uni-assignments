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

    // ИСХОДНЫЕ ДАННЫЕ И ИДЕНТИФИКАТОР ТИПА РЕГРЕССИИ
    int mode{};
    QVector<QString> dataColumns{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};

    // ПЕРЕМЕННЫЕ И СТРУКТУРЫ
    int n{};                                         // Количество экспериментов
    int degree{};                                    // Степень полинома

    QHash<QString, QVector<double>> vector_values{}; // Структура хранения векторов значений
    QHash<QString, double> values{};                 // Структура хранения значений
    // Общее
    QHash<QString, double> coefficients{};           // Структура хранения коэффициентов регрессии
    QString r_descr{};                               // Описание коэффициента Пирсона (коэффициента корреляции)
    QString regCoefStr{};                            // Вывод коэффициентов регрессии

    // ГРАФИК
    QDate select_date{};  // Выбранная дата
    QDate default_date{}; // Дата по умолчанию
    QString trend_eq{};   // Уравнение регрессии

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // WORKPLACEFORM_H
