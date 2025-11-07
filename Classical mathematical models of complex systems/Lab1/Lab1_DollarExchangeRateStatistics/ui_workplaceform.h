/********************************************************************************
** Form generated from reading UI file 'workplaceform.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKPLACEFORM_H
#define UI_WORKPLACEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_WorkplaceForm
{
public:
    QGridLayout *gridLayout_6;
    QFrame *first_frame;
    QGridLayout *gridLayout;
    QLabel *total_label;
    QTableView *total_tableView;
    QTextEdit *totalTable_info_textEdit;
    QLabel *meanX_label;
    QLabel *meanY_label;
    QPushButton *pushButton_back;
    QFrame *third_frame;
    QGridLayout *gridLayout_2;
    QLabel *graphic_label;
    QCustomPlot *customPlot;
    QFrame *second_frame;
    QGridLayout *gridLayout_4;
    QLabel *calc_label;
    QLabel *forecast_label;
    QLabel *equation_label;
    QTableView *calculate_tableView;
    QTextEdit *correlation_textEdit;
    QTextEdit *coefficients_textEdit;
    QGridLayout *gridLayout_3;
    QLabel *n_label;
    QLabel *Sx2_label;
    QLabel *Sy2_label;
    QLabel *Sregr_label;
    QLabel *MSE_label;
    QLabel *meanSx_label;
    QLabel *meanSy_label;
    QLabel *Sost_label;
    QLabel *R2_label;
    QLabel *Sfull_label;
    QLabel *descr_R2_label;
    QLabel *selectDate_label;
    QDateEdit *selectDate_dateEdit;
    QPushButton *calculate_pushButton;
    QPushButton *clean_pushButton;
    QLabel *forecastCurs_label;

    void setupUi(QWidget *WorkplaceForm)
    {
        if (WorkplaceForm->objectName().isEmpty())
            WorkplaceForm->setObjectName("WorkplaceForm");
        WorkplaceForm->resize(1388, 773);
        WorkplaceForm->setMinimumSize(QSize(1388, 773));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/images/RegressionsIcon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WorkplaceForm->setWindowIcon(icon);
        gridLayout_6 = new QGridLayout(WorkplaceForm);
        gridLayout_6->setObjectName("gridLayout_6");
        first_frame = new QFrame(WorkplaceForm);
        first_frame->setObjectName("first_frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(first_frame->sizePolicy().hasHeightForWidth());
        first_frame->setSizePolicy(sizePolicy);
        first_frame->setMinimumSize(QSize(361, 751));
        first_frame->setMaximumSize(QSize(361, 16777215));
        first_frame->setFrameShape(QFrame::WinPanel);
        first_frame->setFrameShadow(QFrame::Sunken);
        gridLayout = new QGridLayout(first_frame);
        gridLayout->setObjectName("gridLayout");
        total_label = new QLabel(first_frame);
        total_label->setObjectName("total_label");
        total_label->setMinimumSize(QSize(341, 32));
        total_label->setMaximumSize(QSize(341, 32));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(10);
        font.setWeight(QFont::Medium);
        font.setItalic(true);
        total_label->setFont(font);
        total_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(120, 120, 120);      /* \320\274\321\217\320\263\320\272\320\270\320\271 \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(240, 240, 240);                /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \321\205\320\276\321\200\320\276\321\210\320\265\320\263\320\276 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(160, 160, 160);     /* \321\202\320\276\320\275\320\272\320\260\321\217, \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                        /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
" "
                        "   padding: 4px 8px;                          /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;       /* \320\276\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-weight: 500;                           /* \321\203\320\274\320\265\321\200\320\265\320\275\320\275\320\276 \320\266\320\270\321\200\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 10pt;                            /* \321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"    font-style: italic;   \n"
"}\n"
""));
        total_label->setFrameShape(QFrame::WinPanel);
        total_label->setFrameShadow(QFrame::Plain);
        total_label->setTextFormat(Qt::AutoText);
        total_label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(total_label, 0, 0, 1, 2);

        total_tableView = new QTableView(first_frame);
        total_tableView->setObjectName("total_tableView");
        sizePolicy.setHeightForWidth(total_tableView->sizePolicy().hasHeightForWidth());
        total_tableView->setSizePolicy(sizePolicy);
        total_tableView->setMinimumSize(QSize(341, 441));
        total_tableView->setMaximumSize(QSize(341, 16777215));
        total_tableView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        total_tableView->setStyleSheet(QString::fromUtf8("/* ------------------- QTableView ------------------- */\n"
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
        total_tableView->setFrameShape(QFrame::WinPanel);
        total_tableView->setFrameShadow(QFrame::Sunken);
        total_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        total_tableView->setGridStyle(Qt::DashDotLine);

        gridLayout->addWidget(total_tableView, 1, 0, 1, 2);

        totalTable_info_textEdit = new QTextEdit(first_frame);
        totalTable_info_textEdit->setObjectName("totalTable_info_textEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(totalTable_info_textEdit->sizePolicy().hasHeightForWidth());
        totalTable_info_textEdit->setSizePolicy(sizePolicy1);
        totalTable_info_textEdit->setMinimumSize(QSize(341, 155));
        totalTable_info_textEdit->setMaximumSize(QSize(341, 155));
        totalTable_info_textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    background-color:  rgba(220, 235, 245, 200);\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(180, 180, 180);\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        totalTable_info_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        totalTable_info_textEdit->setReadOnly(true);

        gridLayout->addWidget(totalTable_info_textEdit, 2, 0, 1, 2);

        meanX_label = new QLabel(first_frame);
        meanX_label->setObjectName("meanX_label");
        meanX_label->setMinimumSize(QSize(165, 31));
        meanX_label->setMaximumSize(QSize(165, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(10);
        font1.setItalic(true);
        meanX_label->setFont(font1);
        meanX_label->setCursor(QCursor(Qt::IBeamCursor));
        meanX_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgba(220, 235, 245, 200);  /* \320\261\320\273\320\265\320\264\320\275\320\276-\320\263\320\276\320\273\321\203\320\261\320\276\320\271 \321\201 \320\273\321\221\320\263\320\272\320\276\320\271 \320\277\321\200\320\276\320\267\321\200\320\260\321\207\320\275\320\276\321\201\321\202\321\214\321\216 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgba(180, 200, 210, 180);\n"
"    border-radius: 6px;\n"
"    padding: 0px 8px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"    font-style: italic;\n"
"}"));
        meanX_label->setFrameShape(QFrame::WinPanel);
        meanX_label->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(meanX_label, 3, 0, 1, 1);

        meanY_label = new QLabel(first_frame);
        meanY_label->setObjectName("meanY_label");
        meanY_label->setMinimumSize(QSize(164, 31));
        meanY_label->setMaximumSize(QSize(164, 31));
        meanY_label->setFont(font1);
        meanY_label->setCursor(QCursor(Qt::IBeamCursor));
        meanY_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgba(220, 235, 245, 200);  /* \320\261\320\273\320\265\320\264\320\275\320\276-\320\263\320\276\320\273\321\203\320\261\320\276\320\271 \321\201 \320\273\321\221\320\263\320\272\320\276\320\271 \320\277\321\200\320\276\320\267\321\200\320\260\321\207\320\275\320\276\321\201\321\202\321\214\321\216 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgba(180, 200, 210, 180);\n"
"    border-radius: 6px;\n"
"    padding: 0px 8px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"    font-style: italic;\n"
"}"));
        meanY_label->setFrameShape(QFrame::WinPanel);
        meanY_label->setFrameShadow(QFrame::Sunken);
        meanY_label->setTextFormat(Qt::AutoText);

        gridLayout->addWidget(meanY_label, 3, 1, 1, 1);

        pushButton_back = new QPushButton(first_frame);
        pushButton_back->setObjectName("pushButton_back");
        pushButton_back->setMinimumSize(QSize(341, 38));
        pushButton_back->setMaximumSize(QSize(341, 38));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Segoe UI")});
        font2.setPointSize(10);
        font2.setWeight(QFont::DemiBold);
        pushButton_back->setFont(font2);
        pushButton_back->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_back->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(100, 149, 237);    /* \321\217\321\200\320\272\320\270\320\271 \320\263\320\276\320\273\321\203\320\261\320\276\320\271 (Cornflower Blue) */\n"
"    color: rgb(255, 255, 255);               /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border-radius: 12px;                     \n"
"    border: 2px solid rgb(65, 105, 225);    /* \320\261\320\276\320\273\320\265\320\265 \320\267\320\260\320\274\320\265\321\202\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 (Royal Blue) */\n"
"    padding: 6px 14px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-weight: 600;                        /* \321\207\321\203\321\202\321\214 \320\261\320\276\320\273\320\265\320\265 \320\266\320\270\321\200\320\275\321\213\320\271 */\n"
"    font-size: 10pt;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb"
                        "(72, 118, 255);     /* \321\217\321\200\321\207\320\265 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 (Dodger Blue) */\n"
"    color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(25, 25, 112);      /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\320\270\320\271 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 (Midnight Blue) */\n"
"    border: 2px solid rgb(0, 0, 139);       /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\321\217\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    background-color: rgb(0, 191, 255);      /* \320\260\320\272\321\202\320\270\320\262\320\275\321\213\320\271 \321\201\320\262\320\265\321\202\320\273\320\276-\320\263\320\276\320\273\321\203\320\261\320\276\320\271 (Deep Sky Blue) */\n"
"    border: 2px solid rgb(30, 144, 255);    /* \320\272"
                        "\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 (Dodger Blue) */\n"
"    color: rgb(0, 51, 102);                  /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\320\270\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"}\n"
""));

        gridLayout->addWidget(pushButton_back, 4, 0, 1, 2);


        gridLayout_6->addWidget(first_frame, 0, 0, 2, 1);

        third_frame = new QFrame(WorkplaceForm);
        third_frame->setObjectName("third_frame");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(third_frame->sizePolicy().hasHeightForWidth());
        third_frame->setSizePolicy(sizePolicy2);
        third_frame->setMinimumSize(QSize(1001, 313));
        third_frame->setFrameShape(QFrame::WinPanel);
        third_frame->setFrameShadow(QFrame::Sunken);
        gridLayout_2 = new QGridLayout(third_frame);
        gridLayout_2->setObjectName("gridLayout_2");
        graphic_label = new QLabel(third_frame);
        graphic_label->setObjectName("graphic_label");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(graphic_label->sizePolicy().hasHeightForWidth());
        graphic_label->setSizePolicy(sizePolicy3);
        graphic_label->setMinimumSize(QSize(644, 31));
        graphic_label->setMaximumSize(QSize(16777215, 31));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Segoe UI")});
        font3.setPointSize(8);
        font3.setWeight(QFont::Medium);
        font3.setItalic(true);
        graphic_label->setFont(font3);
        graphic_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(120, 120, 120);      /* \320\274\321\217\320\263\320\272\320\270\320\271 \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(240, 240, 240);                /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \321\205\320\276\321\200\320\276\321\210\320\265\320\263\320\276 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(160, 160, 160);     /* \321\202\320\276\320\275\320\272\320\260\321\217, \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                        /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
" "
                        "   padding: 4px 8px;                          /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;       /* \320\276\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-weight: 500;                           /* \321\203\320\274\320\265\321\200\320\265\320\275\320\275\320\276 \320\266\320\270\321\200\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 8pt;                            /* \321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"    font-style: italic;   \n"
"}\n"
""));
        graphic_label->setFrameShape(QFrame::WinPanel);
        graphic_label->setFrameShadow(QFrame::Plain);
        graphic_label->setTextFormat(Qt::AutoText);
        graphic_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(graphic_label, 0, 0, 1, 1);

        customPlot = new QCustomPlot(third_frame);
        customPlot->setObjectName("customPlot");
        customPlot->setMinimumSize(QSize(644, 295));

        gridLayout_2->addWidget(customPlot, 1, 0, 2, 1);


        gridLayout_6->addWidget(third_frame, 1, 1, 1, 1);

        second_frame = new QFrame(WorkplaceForm);
        second_frame->setObjectName("second_frame");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(second_frame->sizePolicy().hasHeightForWidth());
        second_frame->setSizePolicy(sizePolicy4);
        second_frame->setMinimumSize(QSize(998, 431));
        second_frame->setFrameShape(QFrame::WinPanel);
        second_frame->setFrameShadow(QFrame::Sunken);
        gridLayout_4 = new QGridLayout(second_frame);
        gridLayout_4->setObjectName("gridLayout_4");
        calc_label = new QLabel(second_frame);
        calc_label->setObjectName("calc_label");
        calc_label->setMinimumSize(QSize(972, 31));
        calc_label->setMaximumSize(QSize(16777215, 31));
        calc_label->setFont(font);
        calc_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(120, 120, 120);      /* \320\274\321\217\320\263\320\272\320\270\320\271 \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(240, 240, 240);                /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \321\205\320\276\321\200\320\276\321\210\320\265\320\263\320\276 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(160, 160, 160);     /* \321\202\320\276\320\275\320\272\320\260\321\217, \320\274\321\217\320\263\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                        /* \320\275\320\265\320\261\320\276\320\273\321\214\321\210\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 \321\203\320\263\320\273\320\276\320\262 */\n"
" "
                        "   padding: 4px 8px;                          /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;       /* \320\276\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-weight: 500;                           /* \321\203\320\274\320\265\321\200\320\265\320\275\320\275\320\276 \320\266\320\270\321\200\320\275\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 10pt;                            /* \321\207\320\270\321\202\320\260\320\265\320\274\321\213\320\271 \321\200\320\260\320\267\320\274\320\265\321\200 */\n"
"    font-style: italic;   \n"
"}\n"
""));
        calc_label->setFrameShape(QFrame::WinPanel);
        calc_label->setFrameShadow(QFrame::Plain);
        calc_label->setTextFormat(Qt::AutoText);
        calc_label->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(calc_label, 0, 0, 1, 3);

        forecast_label = new QLabel(second_frame);
        forecast_label->setObjectName("forecast_label");
        forecast_label->setMaximumSize(QSize(16777215, 31));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Segoe UI")});
        font4.setPointSize(9);
        font4.setBold(false);
        font4.setItalic(true);
        forecast_label->setFont(font4);
        forecast_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(180, 180, 180);     /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271, \320\274\321\217\320\263\321\207\320\265 \321\207\320\265\320\274 \321\203 \320\267\320\260\320\263\320\276\320\273\320\276\320\262\320\272\320\260 */\n"
"    color: rgb(40, 40, 40);                  /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(200, 200, 200);    /* \321\202\320\276\320\275\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260, \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 */\n"
"    border-radius: 3px;                      /* \320\260\320\272\320\272\321\203\321\200\320\260\321\202\320\275\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320"
                        "\265 */\n"
"    padding: 3px 6px;                        /* \320\275\320\265\320\274\320\275\320\276\320\263\320\276 \320\274\320\265\320\275\321\214\321\210\320\265 \320\276\321\202\321\201\321\202\321\203\320\277 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-weight: 400;                        /* \320\275\320\276\321\200\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \321\202\320\276\320\273\321\211\320\270\320\275\320\260 */\n"
"    font-size: 9pt;                          /* \321\207\321\203\321\202\321\214 \320\274\320\265\320\275\321\214\321\210\320\265 \321\207\320\265\320\274 \321\203 \320\267\320\260\320\263\320\276\320\273\320\276\320\262\320\272\320\260 */\n"
"}\n"
""));
        forecast_label->setFrameShape(QFrame::WinPanel);
        forecast_label->setFrameShadow(QFrame::Plain);
        forecast_label->setTextFormat(Qt::AutoText);
        forecast_label->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(forecast_label, 1, 0, 1, 1);

        equation_label = new QLabel(second_frame);
        equation_label->setObjectName("equation_label");
        sizePolicy1.setHeightForWidth(equation_label->sizePolicy().hasHeightForWidth());
        equation_label->setSizePolicy(sizePolicy1);
        equation_label->setMinimumSize(QSize(581, 31));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Segoe UI")});
        font5.setPointSize(9);
        font5.setWeight(QFont::Medium);
        font5.setItalic(true);
        equation_label->setFont(font5);
        equation_label->setCursor(QCursor(Qt::IBeamCursor));
        equation_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(140, 210, 180);   /* \321\207\321\203\321\202\321\214 \320\261\320\276\320\273\320\265\320\265 \321\202\321\221\320\274\320\275\321\213\320\271 \320\274\321\217\321\202\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: #1C1C1C;                         /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(110, 180, 150);   /* \321\200\320\260\320\274\320\272\320\260 \321\202\320\265\320\274\320\275\320\265\320\265 */\n"
"    border-radius: 12px;                    /* \321\201\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    padding: 4px 12px;                      /* \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 9pt;                         /* \320\275\320\265\320\274\320\275\320\276\320\263\320\276 \320\272\321\200\321\203\320\277\320"
                        "\275\320\265\320\265 \320\264\320\273\321\217 \321\207\320\270\321\202\320\260\320\265\320\274\320\276\321\201\321\202\320\270 */\n"
"    qproperty-alignment: 'AlignCenter';     /* \320\262\321\213\321\200\320\260\320\262\320\275\320\270\320\262\320\260\320\275\320\270\320\265 \321\202\320\265\320\272\321\201\321\202\320\260 \320\277\320\276 \321\206\320\265\320\275\321\202\321\200\321\203 */\n"
"}\n"
""));
        equation_label->setFrameShape(QFrame::WinPanel);
        equation_label->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(equation_label, 1, 1, 1, 2);

        calculate_tableView = new QTableView(second_frame);
        calculate_tableView->setObjectName("calculate_tableView");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(calculate_tableView->sizePolicy().hasHeightForWidth());
        calculate_tableView->setSizePolicy(sizePolicy5);
        calculate_tableView->setMaximumSize(QSize(16777215, 329));
        calculate_tableView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        calculate_tableView->setFocusPolicy(Qt::StrongFocus);
        calculate_tableView->setStyleSheet(QString::fromUtf8("/* ------------------- QTableView ------------------- */\n"
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
        calculate_tableView->setFrameShape(QFrame::WinPanel);
        calculate_tableView->setFrameShadow(QFrame::Sunken);
        calculate_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        calculate_tableView->setGridStyle(Qt::DashDotLine);

        gridLayout_4->addWidget(calculate_tableView, 2, 0, 2, 1);

        correlation_textEdit = new QTextEdit(second_frame);
        correlation_textEdit->setObjectName("correlation_textEdit");
        sizePolicy1.setHeightForWidth(correlation_textEdit->sizePolicy().hasHeightForWidth());
        correlation_textEdit->setSizePolicy(sizePolicy1);
        correlation_textEdit->setMinimumSize(QSize(285, 81));
        correlation_textEdit->setMaximumSize(QSize(289, 81));
        correlation_textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    background-color: rgb(220, 220, 220);  /* \320\275\320\265\320\271\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271 \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(180, 180, 180);\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 8pt;\n"
"}\n"
""));
        correlation_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        correlation_textEdit->setReadOnly(true);

        gridLayout_4->addWidget(correlation_textEdit, 2, 1, 1, 1);

        coefficients_textEdit = new QTextEdit(second_frame);
        coefficients_textEdit->setObjectName("coefficients_textEdit");
        sizePolicy1.setHeightForWidth(coefficients_textEdit->sizePolicy().hasHeightForWidth());
        coefficients_textEdit->setSizePolicy(sizePolicy1);
        coefficients_textEdit->setMinimumSize(QSize(285, 81));
        coefficients_textEdit->setMaximumSize(QSize(288, 81));
        coefficients_textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"    background-color: rgb(200, 225, 245);  /* \320\275\320\265\320\266\320\275\321\213\320\271 \320\263\320\276\320\273\321\203\320\261\320\276\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(150, 180, 200);\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 8pt;\n"
"}\n"
""));
        coefficients_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);

        gridLayout_4->addWidget(coefficients_textEdit, 2, 2, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        n_label = new QLabel(second_frame);
        n_label->setObjectName("n_label");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(n_label->sizePolicy().hasHeightForWidth());
        n_label->setSizePolicy(sizePolicy6);
        n_label->setMinimumSize(QSize(140, 28));
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Times New Roman")});
        font6.setPointSize(10);
        n_label->setFont(font6);
        n_label->setCursor(QCursor(Qt::IBeamCursor));
        n_label->setFrameShape(QFrame::WinPanel);
        n_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(n_label, 0, 0, 1, 1);

        Sx2_label = new QLabel(second_frame);
        Sx2_label->setObjectName("Sx2_label");
        QSizePolicy sizePolicy7(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(Sx2_label->sizePolicy().hasHeightForWidth());
        Sx2_label->setSizePolicy(sizePolicy7);
        Sx2_label->setMinimumSize(QSize(140, 28));
        QFont font7;
        font7.setFamilies({QString::fromUtf8("Segoe UI")});
        font7.setPointSize(10);
        Sx2_label->setFont(font7);
        Sx2_label->setCursor(QCursor(Qt::IBeamCursor));
        Sx2_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(250, 200, 220);   /* \320\275\320\265\320\266\320\275\320\276-\321\200\320\276\320\267\320\276\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(220, 150, 180);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        Sx2_label->setFrameShape(QFrame::WinPanel);
        Sx2_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(Sx2_label, 0, 1, 1, 1);

        Sy2_label = new QLabel(second_frame);
        Sy2_label->setObjectName("Sy2_label");
        sizePolicy7.setHeightForWidth(Sy2_label->sizePolicy().hasHeightForWidth());
        Sy2_label->setSizePolicy(sizePolicy7);
        Sy2_label->setMinimumSize(QSize(140, 28));
        Sy2_label->setFont(font7);
        Sy2_label->setCursor(QCursor(Qt::IBeamCursor));
        Sy2_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(250, 200, 220);   /* \320\275\320\265\320\266\320\275\320\276-\321\200\320\276\320\267\320\276\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(220, 150, 180);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        Sy2_label->setFrameShape(QFrame::WinPanel);
        Sy2_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(Sy2_label, 0, 2, 1, 2);

        Sregr_label = new QLabel(second_frame);
        Sregr_label->setObjectName("Sregr_label");
        QSizePolicy sizePolicy8(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(Sregr_label->sizePolicy().hasHeightForWidth());
        Sregr_label->setSizePolicy(sizePolicy8);
        Sregr_label->setMinimumSize(QSize(140, 28));
        Sregr_label->setFont(font7);
        Sregr_label->setCursor(QCursor(Qt::IBeamCursor));
        Sregr_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 255, 200);   /* \321\201\320\262\320\265\321\202\320\273\320\276-\320\266\321\221\320\273\321\202\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(210, 210, 150);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        Sregr_label->setFrameShape(QFrame::WinPanel);
        Sregr_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(Sregr_label, 0, 4, 1, 1);

        MSE_label = new QLabel(second_frame);
        MSE_label->setObjectName("MSE_label");
        sizePolicy6.setHeightForWidth(MSE_label->sizePolicy().hasHeightForWidth());
        MSE_label->setSizePolicy(sizePolicy6);
        MSE_label->setMinimumSize(QSize(140, 28));
        MSE_label->setFont(font7);
        MSE_label->setCursor(QCursor(Qt::IBeamCursor));
        MSE_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(230, 210, 250);   /* \320\275\320\265\320\266\320\275\320\276-\321\204\320\270\320\276\320\273\320\265\321\202\320\276\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(180, 160, 210);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        MSE_label->setFrameShape(QFrame::WinPanel);
        MSE_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(MSE_label, 1, 0, 1, 1);

        meanSx_label = new QLabel(second_frame);
        meanSx_label->setObjectName("meanSx_label");
        QSizePolicy sizePolicy9(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(meanSx_label->sizePolicy().hasHeightForWidth());
        meanSx_label->setSizePolicy(sizePolicy9);
        meanSx_label->setMinimumSize(QSize(140, 28));
        meanSx_label->setFont(font7);
        meanSx_label->setCursor(QCursor(Qt::IBeamCursor));
        meanSx_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(250, 200, 220);   /* \320\275\320\265\320\266\320\275\320\276-\321\200\320\276\320\267\320\276\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(220, 150, 180);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        meanSx_label->setFrameShape(QFrame::WinPanel);
        meanSx_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(meanSx_label, 1, 1, 1, 1);

        meanSy_label = new QLabel(second_frame);
        meanSy_label->setObjectName("meanSy_label");
        sizePolicy7.setHeightForWidth(meanSy_label->sizePolicy().hasHeightForWidth());
        meanSy_label->setSizePolicy(sizePolicy7);
        meanSy_label->setMinimumSize(QSize(140, 28));
        meanSy_label->setFont(font7);
        meanSy_label->setCursor(QCursor(Qt::IBeamCursor));
        meanSy_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(250, 200, 220);   /* \320\275\320\265\320\266\320\275\320\276-\321\200\320\276\320\267\320\276\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(220, 150, 180);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        meanSy_label->setFrameShape(QFrame::WinPanel);
        meanSy_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(meanSy_label, 1, 2, 1, 2);

        Sost_label = new QLabel(second_frame);
        Sost_label->setObjectName("Sost_label");
        sizePolicy8.setHeightForWidth(Sost_label->sizePolicy().hasHeightForWidth());
        Sost_label->setSizePolicy(sizePolicy8);
        Sost_label->setMinimumSize(QSize(140, 28));
        Sost_label->setFont(font7);
        Sost_label->setCursor(QCursor(Qt::IBeamCursor));
        Sost_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 255, 200);   /* \321\201\320\262\320\265\321\202\320\273\320\276-\320\266\321\221\320\273\321\202\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(210, 210, 150);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        Sost_label->setFrameShape(QFrame::WinPanel);
        Sost_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(Sost_label, 1, 4, 1, 1);

        R2_label = new QLabel(second_frame);
        R2_label->setObjectName("R2_label");
        sizePolicy6.setHeightForWidth(R2_label->sizePolicy().hasHeightForWidth());
        R2_label->setSizePolicy(sizePolicy6);
        R2_label->setMinimumSize(QSize(140, 28));
        R2_label->setFont(font7);
        R2_label->setCursor(QCursor(Qt::IBeamCursor));
        R2_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 220, 180);   /* \321\201\320\262\320\265\321\202\320\273\320\276-\320\276\321\200\320\260\320\275\320\266\320\265\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(210, 170, 130);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        R2_label->setFrameShape(QFrame::WinPanel);
        R2_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(R2_label, 2, 0, 1, 1);

        Sfull_label = new QLabel(second_frame);
        Sfull_label->setObjectName("Sfull_label");
        sizePolicy8.setHeightForWidth(Sfull_label->sizePolicy().hasHeightForWidth());
        Sfull_label->setSizePolicy(sizePolicy8);
        Sfull_label->setMinimumSize(QSize(140, 28));
        Sfull_label->setFont(font7);
        Sfull_label->setCursor(QCursor(Qt::IBeamCursor));
        Sfull_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 255, 200);   /* \321\201\320\262\320\265\321\202\320\273\320\276-\320\266\321\221\320\273\321\202\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(210, 210, 150);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        Sfull_label->setFrameShape(QFrame::WinPanel);
        Sfull_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(Sfull_label, 2, 4, 1, 1);

        descr_R2_label = new QLabel(second_frame);
        descr_R2_label->setObjectName("descr_R2_label");
        sizePolicy8.setHeightForWidth(descr_R2_label->sizePolicy().hasHeightForWidth());
        descr_R2_label->setSizePolicy(sizePolicy8);
        descr_R2_label->setMinimumSize(QSize(581, 28));
        descr_R2_label->setFont(font7);
        descr_R2_label->setCursor(QCursor(Qt::IBeamCursor));
        descr_R2_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 220, 180);   /* \321\201\320\262\320\265\321\202\320\273\320\276-\320\276\321\200\320\260\320\275\320\266\320\265\320\262\321\213\320\271 */\n"
"    color: #1C1C1C;\n"
"    border: 1px solid rgb(210, 170, 130);\n"
"    border-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 10pt;\n"
"}\n"
""));
        descr_R2_label->setFrameShape(QFrame::WinPanel);
        descr_R2_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(descr_R2_label, 3, 0, 1, 5);

        selectDate_label = new QLabel(second_frame);
        selectDate_label->setObjectName("selectDate_label");
        selectDate_label->setMinimumSize(QSize(581, 28));
        selectDate_label->setFont(font4);
        selectDate_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(180, 180, 180);     /* \321\201\320\262\320\265\321\202\320\273\320\276-\321\201\320\265\321\200\321\213\320\271, \320\274\321\217\320\263\321\207\320\265 \321\207\320\265\320\274 \321\203 \320\267\320\260\320\263\320\276\320\273\320\276\320\262\320\272\320\260 */\n"
"    color: rgb(40, 40, 40);                  /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\265\321\200\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\260 */\n"
"    border: 1px solid rgb(200, 200, 200);    /* \321\202\320\276\320\275\320\272\320\260\321\217 \321\200\320\260\320\274\320\272\320\260, \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 */\n"
"    border-radius: 3px;                      /* \320\260\320\272\320\272\321\203\321\200\320\260\321\202\320\275\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320"
                        "\265 */\n"
"    padding: 3px 6px;                        /* \320\275\320\265\320\274\320\275\320\276\320\263\320\276 \320\274\320\265\320\275\321\214\321\210\320\265 \320\276\321\202\321\201\321\202\321\203\320\277 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-weight: 400;                        /* \320\275\320\276\321\200\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \321\202\320\276\320\273\321\211\320\270\320\275\320\260 */\n"
"    font-size: 9pt;                          /* \321\207\321\203\321\202\321\214 \320\274\320\265\320\275\321\214\321\210\320\265 \321\207\320\265\320\274 \321\203 \320\267\320\260\320\263\320\276\320\273\320\276\320\262\320\272\320\260 */\n"
"}\n"
""));
        selectDate_label->setFrameShape(QFrame::WinPanel);
        selectDate_label->setFrameShadow(QFrame::Plain);
        selectDate_label->setTextFormat(Qt::AutoText);
        selectDate_label->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(selectDate_label, 4, 0, 1, 5);

        selectDate_dateEdit = new QDateEdit(second_frame);
        selectDate_dateEdit->setObjectName("selectDate_dateEdit");
        selectDate_dateEdit->setMinimumSize(QSize(141, 29));
        selectDate_dateEdit->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    border: 1px solid rgb(0, 32, 96);   /* \321\201\321\202\321\200\320\276\320\263\320\260\321\217 \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\321\217\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 4px;                 /* \320\273\321\221\320\263\320\272\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 */\n"
"    padding: 4px 6px;                   /* \320\262\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    background-color: white;            /* \320\261\320\265\320\273\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(0, 32, 96);              /* \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\320\270\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font: 8pt \"Segoe UI\";              /* \321\201\321\202\321\200\320\276\320\263\320\270\320\271 \321\210\321\200\320\270"
                        "\321\204\321\202 */\n"
"}\n"
"\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 20px;\n"
"    border-left: 1px solid rgb(0, 32, 96);\n"
"    background-color: rgb(146, 205, 220);  /* \320\272\320\276\321\200\320\277\320\276\321\200\320\260\321\202\320\270\320\262\320\275\321\213\320\271 \320\263\320\276\320\273\321\203\320\261\320\276\320\271 */\n"
"}\n"
"\n"
"QDateEdit:focus {\n"
"    border: 1px solid rgb(146, 205, 220);  /* \320\263\320\276\320\273\321\203\320\261\320\260\321\217 \320\277\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\321\200\320\270 \321\204\320\276\320\272\321\203\321\201\320\265 */\n"
"    background-color: rgb(240, 248, 255);  /* \320\273\321\221\320\263\320\272\320\270\320\271 \320\260\320\272\321\206\320\265\320\275\321\202 \321\204\320\276\320\275\320\260 */\n"
"}\n"
""));

        gridLayout_3->addWidget(selectDate_dateEdit, 5, 0, 1, 1);

        calculate_pushButton = new QPushButton(second_frame);
        calculate_pushButton->setObjectName("calculate_pushButton");
        calculate_pushButton->setMinimumSize(QSize(201, 31));
        calculate_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(0, 32, 96);     /* \321\201\321\202\321\200\320\276\320\263\320\270\320\271 \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\320\270\320\271 */\n"
"    color: white;                         /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(0, 32, 96);     /* \321\200\320\260\320\274\320\272\320\260 \320\262 \321\202\320\276\320\275 */\n"
"    border-radius: 6px;                   /* \320\273\321\221\320\263\320\272\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 */\n"
"    padding: 6px 12px;                    /* \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font: 8pt \"Segoe UI\";                /* \321\201\321\202\321\200\320\276\320\263\320\270\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(0, 45, 135);    /* \321\201\320\262\320\265\321\202\320"
                        "\273\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(0, 20, 64);     /* \321\202\320\265\320\274\320\275\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200); /* \321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(120, 120, 120);            /* \321\202\321\203\321\201\320\272\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(160, 160, 160); /* \320\261\320\273\320\265\320\264\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        gridLayout_3->addWidget(calculate_pushButton, 5, 1, 1, 2);

        clean_pushButton = new QPushButton(second_frame);
        clean_pushButton->setObjectName("clean_pushButton");
        clean_pushButton->setMinimumSize(QSize(201, 31));
        clean_pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(0, 32, 96);     /* \321\201\321\202\321\200\320\276\320\263\320\270\320\271 \321\202\321\221\320\274\320\275\320\276-\321\201\320\270\320\275\320\270\320\271 */\n"
"    color: white;                         /* \320\261\320\265\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(0, 32, 96);     /* \321\200\320\260\320\274\320\272\320\260 \320\262 \321\202\320\276\320\275 */\n"
"    border-radius: 6px;                   /* \320\273\321\221\320\263\320\272\320\276\320\265 \321\201\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\270\320\265 */\n"
"    padding: 6px 12px;                    /* \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font: 8pt \"Segoe UI\";                /* \321\201\321\202\321\200\320\276\320\263\320\270\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(0, 45, 135);    /* \321\201\320\262\320\265\321\202\320"
                        "\273\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(0, 20, 64);     /* \321\202\320\265\320\274\320\275\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200); /* \321\201\320\265\321\200\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: rgb(120, 120, 120);            /* \321\202\321\203\321\201\320\272\320\273\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    border: 1px solid rgb(160, 160, 160); /* \320\261\320\273\320\265\320\264\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
""));

        gridLayout_3->addWidget(clean_pushButton, 5, 3, 1, 2);

        forecastCurs_label = new QLabel(second_frame);
        forecastCurs_label->setObjectName("forecastCurs_label");
        sizePolicy8.setHeightForWidth(forecastCurs_label->sizePolicy().hasHeightForWidth());
        forecastCurs_label->setSizePolicy(sizePolicy8);
        forecastCurs_label->setMinimumSize(QSize(581, 27));
        QFont font8;
        font8.setFamilies({QString::fromUtf8("Segoe UI")});
        font8.setPointSize(8);
        forecastCurs_label->setFont(font8);
        forecastCurs_label->setCursor(QCursor(Qt::IBeamCursor));
        forecastCurs_label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(170, 240, 210);   /* \320\274\321\217\321\202\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
"    color: #1C1C1C;                         /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 \320\264\320\273\321\217 \321\207\320\270\321\202\320\260\320\265\320\274\320\276\321\201\321\202\320\270 */\n"
"    border: 1px solid rgb(140, 210, 180);   /* \321\207\321\203\321\202\321\214 \320\261\320\276\320\273\320\265\320\265 \321\202\321\221\320\274\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    border-radius: 12px;                    /* \321\201\320\272\321\200\321\203\320\263\320\273\321\221\320\275\320\275\321\213\320\265 \321\203\320\263\320\273\321\213 */\n"
"    padding: 4px 12px;                      /* \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"    font-size: 8pt;\n"
"}\n"
""));
        forecastCurs_label->setFrameShape(QFrame::WinPanel);
        forecastCurs_label->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(forecastCurs_label, 6, 0, 1, 5);


        gridLayout_4->addLayout(gridLayout_3, 3, 1, 1, 2);


        gridLayout_6->addWidget(second_frame, 0, 1, 1, 1);


        retranslateUi(WorkplaceForm);

        QMetaObject::connectSlotsByName(WorkplaceForm);
    } // setupUi

    void retranslateUi(QWidget *WorkplaceForm)
    {
        WorkplaceForm->setWindowTitle(QCoreApplication::translate("WorkplaceForm", "Form", nullptr));
        total_label->setText(QCoreApplication::translate("WorkplaceForm", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260 \320\270 \321\200\320\260\321\201\321\207\320\265\321\202\321\213", nullptr));
        meanX_label->setText(QString());
        meanY_label->setText(QString());
        pushButton_back->setText(QCoreApplication::translate("WorkplaceForm", "\320\222\320\265\321\200\320\275\321\203\321\202\321\214\321\201\321\217", nullptr));
        graphic_label->setText(QCoreApplication::translate("WorkplaceForm", "\320\223\321\200\320\260\321\204\320\270\320\272", nullptr));
        calc_label->setText(QCoreApplication::translate("WorkplaceForm", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \320\270 \320\272\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\321\213", nullptr));
        forecast_label->setText(QCoreApplication::translate("WorkplaceForm", "\320\237\321\200\320\265\320\264\321\201\320\272\320\260\320\267\320\260\320\275\320\270\321\217 \320\277\320\276 \320\274\320\276\320\264\320\265\320\273\320\270 \320\270 \321\200\320\260\321\201\321\207\320\265\321\202\321\213", nullptr));
        equation_label->setText(QString());
        n_label->setText(QString());
        Sx2_label->setText(QString());
        Sy2_label->setText(QString());
        Sregr_label->setText(QString());
        MSE_label->setText(QString());
        meanSx_label->setText(QString());
        meanSy_label->setText(QString());
        Sost_label->setText(QString());
        R2_label->setText(QString());
        Sfull_label->setText(QString());
        descr_R2_label->setText(QString());
        selectDate_label->setText(QCoreApplication::translate("WorkplaceForm", "\320\243\320\272\320\260\320\267\320\260\320\275\320\270\320\265 \320\261\321\203\320\264\321\203\321\211\320\265\320\271 \320\264\320\260\321\202\321\213 \320\270 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        calculate_pushButton->setText(QCoreApplication::translate("WorkplaceForm", "\320\240\320\260\321\201\321\201\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
        clean_pushButton->setText(QCoreApplication::translate("WorkplaceForm", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        forecastCurs_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WorkplaceForm: public Ui_WorkplaceForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKPLACEFORM_H
