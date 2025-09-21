#ifndef WORKPLACEFORM_H
#define WORKPLACEFORM_H

#include <QWidget>
#include <QTableView>
#include "RegressionValues.h"

namespace Ui {
class WorkplaceForm;
}

class WorkplaceForm : public QWidget
{
    Q_OBJECT

public:
    explicit WorkplaceForm(const int &mode, const QTableView *data_tableView, QWidget *parent = nullptr);
    ~WorkplaceForm();

signals:
    void backToMain();

private slots:
    void on_pushButton_back_clicked();
    void MakePlot();

private:
    Ui::WorkplaceForm *ui;
    QVector<QString> dataColumn{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};
    QVector<double> xSquared{};
    QVector<double> ySquared{};
    QVector<double> xyProduct{};
    QVector<double> yT{};
    QVector<double> Sost{};
    QVector<double> Sregr{};
    QVector<double> Sfull{};
    RegressionValues values{};
    QString trendLabel{};

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // WORKPLACEFORM_H
