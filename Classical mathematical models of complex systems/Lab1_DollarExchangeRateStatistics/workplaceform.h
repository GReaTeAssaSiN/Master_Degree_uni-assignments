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

private:
    Ui::WorkplaceForm *ui;
    int m_mode{};
    QVector<QString> dataColumn{};
    QVector<int> numericDates{};
    QVector<double> cursValues{};
    QVector<int> xSquared{};
    QVector<double> ySquared{};
    QVector<double> xyProduct{};
    RegressionValues values{};

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // WORKPLACEFORM_H
