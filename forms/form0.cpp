#include "form0.h"
#include "ui_form0.h"

Form0::Form0(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form0)
{
    ui->setupUi(this);
}

Form0::~Form0()
{
    delete ui;
}
