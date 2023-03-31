#include "filterform.h"
#include "ui_filterform.h"
#include <QMessageBox>

filterForm::filterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filterForm)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setFixedSize(width(),height());
    close();
}

filterForm::~filterForm()
{
    delete ui;
}

void filterForm::leaveEvent(QEvent *event)
{
    close();
}
