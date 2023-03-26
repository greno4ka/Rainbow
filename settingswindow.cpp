#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete translator;

    delete ui;
}

void SettingsWindow::setTranslator(QTranslator *newTranslator)
{
    translator = newTranslator;
}

void SettingsWindow::on_comboBox_activated(int value)
{
    switch (value) {
    case 0:
        if (! translator->load(QString("rainbow_") + QString("en")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    case 1:
        if (! translator->load(QString("rainbow_") + QString("ru")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    case 2:
        if (! translator->load(QString("rainbow_") + QString("fr")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    default:
        break;
    }
    ui->retranslateUi(this);
    emit language_change();
}

