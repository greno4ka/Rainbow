#include "form3.h"
#include "ui_form3.h"

extern float *rnd; // array for random values in 3rd scene

Form3::Form3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form3)
{
    ui->setupUi(this);
}

Form3::~Form3()
{
    delete ui;
    free(rnd); // I hope now you're free!
}
