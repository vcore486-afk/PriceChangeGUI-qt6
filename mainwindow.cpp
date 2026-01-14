#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDoubleValidator>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *validator = new QDoubleValidator(0, 1e15, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->oldPriceEdit->setValidator(validator);
    ui->newPriceEdit->setValidator(validator);

    ui->resultLabel->setStyleSheet("font-size: 16px;");

    connect(ui->calcButton, &QPushButton::clicked,
            this, &MainWindow::calculate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculate()
{
    // --- как в bash: разрешаем , и .
    QString oldText = ui->oldPriceEdit->text().replace(',', '.');
    QString newText = ui->newPriceEdit->text().replace(',', '.');

    bool okOld, okNew;
    double oldPrice = oldText.toDouble(&okOld);
    double newPrice = newText.toDouble(&okNew);

    // --- Проверка чисел
    if (!okOld || !okNew || oldPrice < 0 || newPrice < 0) {
        ui->resultLabel->setText("Ошибка: передавай только положительные числа!");
        ui->resultLabel->setStyleSheet("color: red;");
        return;
    }

    // --- old == 0 → +∞ %
    if (oldPrice == 0.0) {
        ui->resultLabel->setText("▲ Изменение : +∞ %");
        ui->resultLabel->setStyleSheet("color: green; font-size: 18px;");
        return;
    }

    // --- Формула как в bash
    double change = (newPrice - oldPrice) * 100.0 / oldPrice;
    QString changeStr = QString::number(change, 'f', 2);

    QString sign, arrow, color;

    if (change >= 0) {
        sign  = "+";
        arrow = "▲";
        color = "green";
    } else {
        sign  = "";
        arrow = "▼";
        color = "red";
    }

    ui->resultLabel->setText(
        QString("%1 Изменение : %2%3%")
            .arg(arrow)
            .arg(sign)
            .arg(changeStr)
    );

    ui->resultLabel->setStyleSheet(
        QString("color: %1; font-size: 18px;").arg(color)
    );
}
