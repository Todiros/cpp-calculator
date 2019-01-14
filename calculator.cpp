#include "calculator.h"
#include "ui_calculator.h"

static double calcVal = 0.0;
static bool divTrigger = false;
static bool multTrigger = false;
static bool addTrigger = false;
static bool subTrigger = false;

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];

    for (int i=0; i < 10; i++) {
        QString buttonName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(buttonName);

        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearDisplay()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString buttVal = button->text();
    QString displayVal= ui->Display->text();

    if ((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) {
        ui->Display->setText(buttVal);
    } else {
        QString newVal = displayVal + buttVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed() {
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();

    QPushButton *button = (QPushButton *)sender();
    QString buttVal = button->text();

    if (QString::compare(buttVal, "/", Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    } else if (QString::compare(buttVal, "*", Qt::CaseInsensitive) == 0) {
        multTrigger = true;
    } else if (QString::compare(buttVal, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    } else if (QString::compare(buttVal, "-", Qt::CaseInsensitive) == 0) {
        subTrigger = true;
    }

    ui->Display->setText("");
}

void Calculator::EqualButtonPressed() {
    double result = 0.0;

    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if (addTrigger || subTrigger || multTrigger || divTrigger) {
        if (addTrigger) {
            result = calcVal + dblDisplayVal;
        } else if (subTrigger) {
            result = calcVal - dblDisplayVal;
        } else if (multTrigger) {
            result = calcVal * dblDisplayVal;
        } else {
            result = calcVal / dblDisplayVal;
        }
    }

    ui->Display->setText(QString::number(result));
}

void Calculator::ChangeNumberSign() {
    QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");

    if (reg.exactMatch(displayVal)) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = dblDisplayVal * (-1);

        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::ClearDisplay() {
    ui->Display->setText("");
}

