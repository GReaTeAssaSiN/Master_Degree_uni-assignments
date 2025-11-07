/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QFrame *frame_lab_model_labels;
    QGridLayout *gridLayout;
    QLabel *lab_label;
    QLabel *model_label;
    QLabel *data_label;
    QLabel *regressions_label;
    QTableView *data_tableView;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_linear_regr;
    QPushButton *pushButton_inverse_linear_regr;
    QPushButton *pushButton_exp_regr;
    QPushButton *pushButton_hyper_regr;
    QPushButton *pushButton_parab_regr;
    QPushButton *pushButton_log_regr;
    QPushButton *pushButton_power_regr;
    QPushButton *pushButton_polynom_regr;
    QSpacerItem *verticalSpacer;
    QLabel *info_label;
    QLabel *info_link_label;
    QLabel *link_label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_ChooseFile;
    QLabel *choosenFile_label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(976, 725);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(976, 725));
        MainWindow->setMaximumSize(QSize(976, 725));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/images/RegressionsIcon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow{\n"
"	 background-color: rgb(30, 30, 30);  /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        frame_lab_model_labels = new QFrame(centralwidget);
        frame_lab_model_labels->setObjectName("frame_lab_model_labels");
        frame_lab_model_labels->setMinimumSize(QSize(954, 110));
        frame_lab_model_labels->setStyleSheet(QString::fromUtf8("QFrame {\n"
"    background-color: rgb(245, 245, 245);  /* \321\201\320\262\320\265\321\202\320\273\321\213\320\271 \321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    border-radius: 15px;\n"
"    border: 2px solid rgb(200, 200, 200); /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\201\320\265\321\200\320\276-\320\261\320\265\320\273\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));
        frame_lab_model_labels->setFrameShape(QFrame::WinPanel);
        frame_lab_model_labels->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_lab_model_labels);
        gridLayout->setObjectName("gridLayout");
        lab_label = new QLabel(frame_lab_model_labels);
        lab_label->setObjectName("lab_label");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_label->sizePolicy().hasHeightForWidth());
        lab_label->setSizePolicy(sizePolicy1);
        lab_label->setMinimumSize(QSize(930, 45));
        lab_label->setMaximumSize(QSize(16777215, 45));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setBold(true);
        lab_label->setFont(font);
        lab_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(60, 60, 60);     /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    color: white;                           /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    border-radius: 10px;\n"
"    border: 2px solid rgb(100, 100, 100);  /* \321\201\320\265\321\200\321\213\320\271 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202 \320\264\320\273\321\217 \321\200\320\260\320\274\320\272\320\270 */\n"
"    qproperty-alignment: 'AlignCenter';\n"
"}\n"
""));
        lab_label->setFrameShape(QFrame::StyledPanel);
        lab_label->setFrameShadow(QFrame::Plain);
        lab_label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lab_label, 0, 0, 1, 1);

        model_label = new QLabel(frame_lab_model_labels);
        model_label->setObjectName("model_label");
        sizePolicy1.setHeightForWidth(model_label->sizePolicy().hasHeightForWidth());
        model_label->setSizePolicy(sizePolicy1);
        model_label->setMinimumSize(QSize(930, 34));
        model_label->setMaximumSize(QSize(16777215, 34));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setWeight(QFont::DemiBold);
        font1.setItalic(true);
        model_label->setFont(font1);
        model_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(100, 100, 100);   /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: white;                           /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font-size: 16px;\n"
"    font-weight: 600;\n"
"    padding: 6px 12px;\n"
"    border-radius: 8px;\n"
"    border: 1px solid rgb(150, 150, 150);  /* \321\202\320\276\320\275\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    qproperty-alignment: 'AlignCenter';\n"
"}\n"
""));
        model_label->setFrameShape(QFrame::StyledPanel);
        model_label->setFrameShadow(QFrame::Plain);
        model_label->setLineWidth(1);
        model_label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(model_label, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame_lab_model_labels, 0, 0, 1, 3);

        data_label = new QLabel(centralwidget);
        data_label->setObjectName("data_label");
        data_label->setMinimumSize(QSize(556, 40));
        data_label->setMaximumSize(QSize(16777215, 40));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Segoe UI")});
        font2.setPointSize(13);
        font2.setWeight(QFont::ExtraBold);
        data_label->setFont(font2);
        data_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(80, 80, 80);        /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(240, 240, 240);               /* \321\201\320\262\320\265\321\202\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(120, 120, 120);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                       /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 */\n"
"    padding: 4px 8px;                         /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;      /"
                        "* \320\272\320\276\321\200\320\277\320\276\321\200\320\260\321\202\320\270\320\262\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-weight: 800;                          /* \320\266\320\270\321\200\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 13pt;                           /* \321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"}\n"
""));
        data_label->setFrameShape(QFrame::WinPanel);
        data_label->setFrameShadow(QFrame::Plain);
        data_label->setTextFormat(Qt::AutoText);
        data_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(data_label, 1, 0, 1, 2);

        regressions_label = new QLabel(centralwidget);
        regressions_label->setObjectName("regressions_label");
        regressions_label->setMinimumSize(QSize(391, 40));
        regressions_label->setMaximumSize(QSize(16777215, 40));
        regressions_label->setFont(font2);
        regressions_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(80, 80, 80);        /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(240, 240, 240);               /* \321\201\320\262\320\265\321\202\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(120, 120, 120);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                       /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 */\n"
"    padding: 4px 8px;                         /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;      /"
                        "* \320\272\320\276\321\200\320\277\320\276\321\200\320\260\321\202\320\270\320\262\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-weight: 800;                          /* \320\266\320\270\321\200\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 13pt;                           /* \321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"}\n"
""));
        regressions_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(regressions_label, 1, 2, 1, 1);

        data_tableView = new QTableView(centralwidget);
        data_tableView->setObjectName("data_tableView");
        data_tableView->setMinimumSize(QSize(556, 409));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Segoe UI")});
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setItalic(false);
        data_tableView->setFont(font3);
        data_tableView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        data_tableView->setStyleSheet(QString::fromUtf8("/* ------------------- QTableView ------------------- */\n"
"QTableView {\n"
"    background-color: #f8f8f8;\n"
"    alternate-background-color: #e8e8e8;\n"
"    gridline-color: #c0c0c0;\n"
"    font: 10pt \"Segoe UI\";\n"
"    color: #1c1c1c;\n"
"}\n"
"\n"
"/* \320\227\320\260\320\263\320\276\320\273\320\276\320\262\320\272\320\270 */\n"
"QHeaderView::section {\n"
"    background-color: #3a3a3a;\n"
"    color: #ffffff;\n"
"    padding: 4px;\n"
"    border: 1px solid #2a2a2a;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* \320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\320\265 \321\215\320\273\320\265\320\274\320\265\320\275\321\202\321\213 */\n"
"QTableView::item:selected {\n"
"    background-color: #a0a0a0;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"/* \320\255\321\204\321\204\320\265\320\272\321\202 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"QTableView::item:hover {\n"
"    background-color: #d0d0d0;\n"
"}\n"
"\n"
"/* ---------"
                        "---------- \320\222\320\265\321\200\321\202\320\270\320\272\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\276\320\273\320\267\321\203\320\275\320\276\320\272 ------------------- */\n"
"QTableView QScrollBar:vertical {\n"
"    background-color: #f0f0f0;\n"
"    width: 12px;\n"
"    margin: 0px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QTableView QScrollBar::handle:vertical {\n"
"    background-color: #888888;\n"
"    min-height: 20px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QTableView QScrollBar::add-line:vertical,\n"
"QTableView QScrollBar::sub-line:vertical {\n"
"    background: none;\n"
"    height: 0px;\n"
"}\n"
"\n"
"QTableView QScrollBar::add-page:vertical,\n"
"QTableView QScrollBar::sub-page:vertical {\n"
"    background: none;\n"
"}\n"
"\n"
"/* ------------------- \320\223\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\276\320\273\320\267\321\203\320\275\320\276\320\272 ------------------- */\n"
"QTableView QScroll"
                        "Bar:horizontal {\n"
"    background-color: #f0f0f0;\n"
"    height: 12px;\n"
"    margin: 0px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QTableView QScrollBar::handle:horizontal {\n"
"    background-color: #888888;\n"
"    min-width: 20px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QTableView QScrollBar::add-line:horizontal,\n"
"QTableView QScrollBar::sub-line:horizontal {\n"
"    background: none;\n"
"    width: 0px;\n"
"}\n"
"\n"
"QTableView QScrollBar::add-page:horizontal,\n"
"QTableView QScrollBar::sub-page:horizontal {\n"
"    background: none;\n"
"}\n"
""));
        data_tableView->setFrameShape(QFrame::StyledPanel);
        data_tableView->setFrameShadow(QFrame::Raised);
        data_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(data_tableView, 2, 0, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pushButton_linear_regr = new QPushButton(centralwidget);
        pushButton_linear_regr->setObjectName("pushButton_linear_regr");
        pushButton_linear_regr->setMinimumSize(QSize(389, 39));
        pushButton_linear_regr->setMaximumSize(QSize(16777215, 41));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Segoe UI")});
        font4.setPointSize(12);
        font4.setWeight(QFont::Medium);
        pushButton_linear_regr->setFont(font4);
        pushButton_linear_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_linear_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_linear_regr);

        pushButton_inverse_linear_regr = new QPushButton(centralwidget);
        pushButton_inverse_linear_regr->setObjectName("pushButton_inverse_linear_regr");
        pushButton_inverse_linear_regr->setMinimumSize(QSize(389, 39));
        pushButton_inverse_linear_regr->setFont(font4);
        pushButton_inverse_linear_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_inverse_linear_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_inverse_linear_regr);

        pushButton_exp_regr = new QPushButton(centralwidget);
        pushButton_exp_regr->setObjectName("pushButton_exp_regr");
        pushButton_exp_regr->setMinimumSize(QSize(389, 39));
        pushButton_exp_regr->setFont(font4);
        pushButton_exp_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_exp_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_exp_regr);

        pushButton_hyper_regr = new QPushButton(centralwidget);
        pushButton_hyper_regr->setObjectName("pushButton_hyper_regr");
        pushButton_hyper_regr->setMinimumSize(QSize(389, 39));
        pushButton_hyper_regr->setFont(font4);
        pushButton_hyper_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_hyper_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_hyper_regr);

        pushButton_parab_regr = new QPushButton(centralwidget);
        pushButton_parab_regr->setObjectName("pushButton_parab_regr");
        pushButton_parab_regr->setMinimumSize(QSize(389, 39));
        pushButton_parab_regr->setFont(font4);
        pushButton_parab_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_parab_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
""
                        "    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_parab_regr);

        pushButton_log_regr = new QPushButton(centralwidget);
        pushButton_log_regr->setObjectName("pushButton_log_regr");
        pushButton_log_regr->setMinimumSize(QSize(389, 39));
        pushButton_log_regr->setFont(font4);
        pushButton_log_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_log_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_log_regr);

        pushButton_power_regr = new QPushButton(centralwidget);
        pushButton_power_regr->setObjectName("pushButton_power_regr");
        pushButton_power_regr->setMinimumSize(QSize(389, 39));
        pushButton_power_regr->setFont(font4);
        pushButton_power_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_power_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_power_regr);

        pushButton_polynom_regr = new QPushButton(centralwidget);
        pushButton_polynom_regr->setObjectName("pushButton_polynom_regr");
        pushButton_polynom_regr->setMinimumSize(QSize(389, 39));
        pushButton_polynom_regr->setFont(font4);
        pushButton_polynom_regr->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_polynom_regr->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(230, 230, 230);    /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                  /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                       /* \320\267\320\260\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 1px solid rgb(176, 176, 176);    /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 5px 12px;                        /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: 500;                         /* \321\201\321\200\320\265\320\264\320\275\320\270\320\271 \320\262\320\265\321\201 \321\210\321\200\320\270\321\204\321"
                        "\202\320\260 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 12pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(216, 216, 216);    /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 1px solid rgb(160, 160, 160);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(191, 191, 191);    /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 1px solid rgb(140, 140, 140);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(200, 200, 200);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 1px solid rgb(144, 144, 144);\n"
"}\n"
"\n"
"QPushButton:disabled"
                        " {\n"
"    background-color: rgb(240, 240, 240);    /* \321\204\320\276\320\275 \320\264\320\273\321\217 \320\276\321\202\320\272\320\273\321\216\321\207\321\221\320\275\320\275\320\276\320\271 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: rgb(156, 156, 156);               /* \321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(192, 192, 192);    /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        verticalLayout->addWidget(pushButton_polynom_regr);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout_2->addLayout(verticalLayout, 2, 2, 1, 1);

        info_label = new QLabel(centralwidget);
        info_label->setObjectName("info_label");
        info_label->setMinimumSize(QSize(954, 33));
        info_label->setMaximumSize(QSize(16777215, 33));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Segoe UI")});
        font5.setPointSize(9);
        font5.setWeight(QFont::Medium);
        font5.setItalic(true);
        info_label->setFont(font5);
        info_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(138, 138, 138);\n"
"    color:rgb(255, 255, 255);\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 9pt;\n"
"	font-weight: 500;\n"
"    padding: 6px;\n"
"}\n"
""));
        info_label->setFrameShape(QFrame::WinPanel);
        info_label->setFrameShadow(QFrame::Raised);
        info_label->setLineWidth(3);

        gridLayout_2->addWidget(info_label, 3, 0, 1, 3);

        info_link_label = new QLabel(centralwidget);
        info_link_label->setObjectName("info_link_label");
        info_link_label->setMinimumSize(QSize(184, 33));
        info_link_label->setMaximumSize(QSize(184, 33));
        info_link_label->setFont(font5);
        info_link_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(138, 138, 138);\n"
"    color:rgb(255, 255, 255);\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 9pt;\n"
"	font-weight: 500;\n"
"    padding: 6px;\n"
"}\n"
""));
        info_link_label->setFrameShape(QFrame::WinPanel);
        info_link_label->setFrameShadow(QFrame::Raised);
        info_link_label->setLineWidth(3);

        gridLayout_2->addWidget(info_link_label, 4, 0, 1, 1);

        link_label = new QLabel(centralwidget);
        link_label->setObjectName("link_label");
        link_label->setMinimumSize(QSize(365, 33));
        link_label->setMaximumSize(QSize(365, 33));
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Segoe UI")});
        font6.setPointSize(9);
        font6.setWeight(QFont::ExtraBold);
        font6.setItalic(false);
        font6.setUnderline(true);
        link_label->setFont(font6);
        link_label->setCursor(QCursor(Qt::PointingHandCursor));
        link_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 255, 255);\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 9pt;\n"
"    font-weight: 800;\n"
"    padding: 5px 8px;\n"
"}\n"
""));
        link_label->setFrameShape(QFrame::WinPanel);
        link_label->setFrameShadow(QFrame::Sunken);
        link_label->setLineWidth(3);
        link_label->setTextFormat(Qt::RichText);
        link_label->setOpenExternalLinks(true);
        link_label->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout_2->addWidget(link_label, 4, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(388, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 4, 2, 1, 1);

        pushButton_ChooseFile = new QPushButton(centralwidget);
        pushButton_ChooseFile->setObjectName("pushButton_ChooseFile");
        pushButton_ChooseFile->setMinimumSize(QSize(556, 43));
        pushButton_ChooseFile->setMaximumSize(QSize(16777215, 43));
        QFont font7;
        font7.setFamilies({QString::fromUtf8("Segoe UI")});
        font7.setPointSize(12);
        font7.setBold(true);
        pushButton_ChooseFile->setFont(font7);
        pushButton_ChooseFile->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_ChooseFile->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(100, 100, 100);   /* \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\271 \321\204\320\276\320\275 \320\272\320\275\320\276\320\277\320\272\320\270 */\n"
"    color: white;                            /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border-radius: 8px;                      /* \320\276\320\272\321\200\321\203\320\263\320\273\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    border: 2px solid rgb(70, 70, 70);      /* \321\202\321\221\320\274\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    padding: 6px 14px;                       /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-weight: bold;                        /* \320\277\320\276\320\273\321\203\320\266\320\270\321\200\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font-"
                        "family: \"Segoe UI\", sans-serif;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(130, 130, 130);   /* \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"    border: 2px solid rgb(100, 100, 100);  /* \321\200\320\260\320\274\320\272\320\260 \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(70, 70, 70);      /* \321\204\320\276\320\275 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    border: 2px solid rgb(50, 50, 50);     /* \321\202\321\221\320\274\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(80, 80, 80);      /* \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321"
                        "\217 \342\200\234\320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\276\342\200\235 */\n"
"    border: 2px solid rgb(70, 70, 70);     /* \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        gridLayout_2->addWidget(pushButton_ChooseFile, 5, 0, 1, 2);

        choosenFile_label = new QLabel(centralwidget);
        choosenFile_label->setObjectName("choosenFile_label");
        choosenFile_label->setMinimumSize(QSize(391, 36));
        choosenFile_label->setMaximumSize(QSize(16777215, 36));
        QFont font8;
        font8.setFamilies({QString::fromUtf8("Segoe UI")});
        font8.setPointSize(8);
        font8.setBold(false);
        choosenFile_label->setFont(font8);
        choosenFile_label->setCursor(QCursor(Qt::ArrowCursor));
        choosenFile_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(230, 230, 230);        /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(28, 28, 28);                      /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(200, 200, 200);       /* \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                          /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
"    padding: 2px 6px;                            /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;        /* \320\276\321\204\320\270\321\206\320\270"
                        "\320\260\320\273\321\214\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 8pt;                             /* \321\203\320\264\320\276\320\261\320\276\321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"}\n"
""));
        choosenFile_label->setFrameShape(QFrame::StyledPanel);
        choosenFile_label->setFrameShadow(QFrame::Sunken);
        choosenFile_label->setWordWrap(true);

        gridLayout_2->addWidget(choosenFile_label, 5, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "4199_\320\223\320\276\321\200\321\210\320\272\320\276\320\262 \320\220\320\273.\320\236._\320\233\320\260\320\261\320\276\321\200\320\260\321\202\320\276\321\200\320\275\320\260\321\217 \321\200\320\260\320\261\320\276\321\202\320\260 \342\204\2261", nullptr));
        lab_label->setText(QCoreApplication::translate("MainWindow", "\320\233\320\260\320\261\320\276\321\200\320\260\321\202\320\276\321\200\320\275\320\260\321\217 \321\200\320\260\320\261\320\276\321\202\320\260 \342\204\2261", nullptr));
        model_label->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\264\320\265\320\273\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\200\320\276\321\201\321\202\320\260 \320\272\321\203\321\200\321\201\320\260 \320\264\320\276\320\273\320\273\320\260\321\200\320\260 \320\241\320\250\320\220 \320\272 \321\200\321\203\320\261\320\273\321\216", nullptr));
        data_label->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\265\321\200\320\265\320\274\320\265\320\275\321\202\320\260\320\273\321\214\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", nullptr));
        regressions_label->setText(QCoreApplication::translate("MainWindow", "\320\242\320\270\320\277\321\213 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\320\271", nullptr));
        pushButton_linear_regr->setText(QCoreApplication::translate("MainWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_inverse_linear_regr->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\200\320\260\321\202\320\275\320\260\321\217 \320\273\320\270\320\275\320\265\320\271\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_exp_regr->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\276\320\275\320\265\320\275\321\206\320\270\320\260\320\273\321\214\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_hyper_regr->setText(QCoreApplication::translate("MainWindow", "\320\223\320\270\320\277\320\265\321\200\320\261\320\276\320\273\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_parab_regr->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\261\320\276\320\273\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_log_regr->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\260\321\200\320\270\321\204\320\274\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_power_regr->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\265\320\277\320\265\320\275\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_polynom_regr->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\320\270\320\275\320\276\320\274\320\270\320\260\320\273\321\214\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        info_label->setText(QCoreApplication::translate("MainWindow", "\320\224\320\273\321\217 \321\203\320\264\320\276\320\261\321\201\321\202\320\262\320\260 \321\204\320\276\321\200\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217  .xlsx \321\204\320\260\320\271\320\273\320\260 (Excel) \321\215\320\272\321\201\320\277\320\276\321\200\321\202\320\270\321\200\321\203\320\271\321\202\320\265 \320\264\320\270\320\275\320\260\320\274\320\270\320\272\321\203 \320\276\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\320\276\320\263\320\276 \320\272\321\203\321\200\321\201\320\260 \320\267\320\260\320\264\320\260\320\275\320\275\320\276\320\271 \320\262\320\260\320\273\321\216\321\202\321\213 (\320\264\320\276\320\273\320\273\320\260\321\200 \320\241\320\250\320\220) \321\201 \321\201\320\260\320\271\321\202\320\260 \320\246\320\221\320\240.", nullptr));
        info_link_label->setText(QCoreApplication::translate("MainWindow", "\320\236\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\271 \321\201\320\260\320\271\321\202 \320\246\320\221\320\240:", nullptr));
        link_label->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:800; font-style:normal; text-decoration: underline;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://www.cbr.ru/currency_base/dynamics/\"><span style=\" font-style:italic; color:#2d5c8d;\">https://www.cbr.ru/currency_base/dynamics/</span></a></p></body></html>", nullptr));
        pushButton_ChooseFile->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 .xlsx \321\204\320\260\320\271\320\273 \321\201 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\320\274\320\270 \320\264\320\260\320\275\320\275\321\213\320\274\320\270", nullptr));
        choosenFile_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
