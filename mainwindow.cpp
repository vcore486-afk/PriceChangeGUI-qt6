#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegularExpressionValidator>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- Разрешаем цифры + , или . + до 6 знаков
    QRegularExpression re(R"(^\d*([.,]\d{0,6})?$)");
    auto *validator = new QRegularExpressionValidator(re, this);

    ui->oldPriceEdit->setValidator(validator);
    ui->newPriceEdit->setValidator(validator);

    // --- Локаль с запятой в UI (по желанию)
    ui->oldPriceEdit->setLocale(QLocale::Russian);
    ui->newPriceEdit->setLocale(QLocale::Russian);

    connect(ui->calcButton, &QPushButton::clicked,
            this, &MainWindow::calculate);

    ui->resultLabel->setStyleSheet("font-size: 16px;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculate()
{
    // --- Нормализация ТОЛЬКО для логики
    QString oldText = ui->oldPriceEdit->text().replace(',', '.');
    QString newText = ui->newPriceEdit->text().replace(',', '.');

    bool okOld, okNew;
    double oldPrice = oldText.toDouble(&okOld);
    double newPrice = newText.toDouble(&okNew);

    if (!okOld || !okNew || oldPrice < 0 || newPrice < 0) {
        ui->resultLabel->setText("Ошибка: введите корректные числа");
        ui->resultLabel->setStyleSheet("color: red;");
        return;
    }

    if (oldPrice == 0.0) {
        ui->resultLabel->setText("▲ Изменение: +∞ %");
        ui->resultLabel->setStyleSheet("color: green; font-size: 18px;");
        return;
    }

    double change = (newPrice - oldPrice) * 100.0 / oldPrice;
    QString changeStr = QString::number(change, 'f', 2);

    QString arrow = (change >= 0) ? "▲" : "▼";
    QString sign  = (change >= 0) ? "+" : "";
    QString color = (change >= 0) ? "green" : "red";

    ui->resultLabel->setText(
        QString("%1 Изменение: %2%3%")
            .arg(arrow)
            .arg(sign)
            .arg(changeStr)
    );

    ui->resultLabel->setStyleSheet(
        QString("color: %1; font-size: 18px;").arg(color)
    );
}
