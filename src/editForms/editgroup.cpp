#include "editgroup.h"
#include "ui_editgroup.h"

#include <QFile>
#include <QMessageBox>

editGroup::editGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editGroup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("Редагування групи (%groupName%)");
    setFixedSize(width(), height());
    idRowEdit = -1;

    ui->curatorComboBox->insertSeparator(1);
    ui->specialComboBox->insertSeparator(1);
    ui->headManComboBox->insertSeparator(1);
}


editGroup::~editGroup()
{
    delete ui;
}


void editGroup::setBlackUI()
{
    ui->mainImage->setPixmap(QPixmap(":/img/whiteMenuIcon/groupIco.png"));
    QFile file(":/styles/black/editForms/editForms.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}


void editGroup::setWhiteUI()
{
    ui->mainImage->setPixmap(QPixmap(":/img/blackMenuIcon/groupIco.png"));
    QFile file(":/styles/white/editForms/editForms.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}


void editGroup::setSystemUI()
{
    QPalette basePalette;
    QColor baseColor = basePalette.base().color();
    QColor newBase = QColor::fromRgbF(1 - baseColor.redF(), 1 - baseColor.greenF(), 1 - baseColor.blueF());

    if (newBase.name() == "#000000")
    {
        setWhiteUI();
    }
    else
    {
        setBlackUI();
    }
}


void editGroup::setData(QString titleName, QStringList listData)
{
    idRowEdit = listData[0].toInt();
    titleName.remove(0, titleName.indexOf('.') + 2);
    setWindowTitle("Редагування групи (" + titleName +")");

    ui->nameLineEdit->setFocus();

    ui->nameLineEdit->setText(listData[1]);
    ui->specialComboBox->setCurrentText(listData[2]);
    ui->startStudySpinBox->setValue(listData[3].toInt());
    ui->endStudySpinBox->setValue(listData[4].toInt());
    ui->curatorComboBox->setCurrentText(listData[5]);
    ui->headManComboBox->setCurrentText(listData[6]);
}


void editGroup::setDataCuratorComboBox(QStringList list)
{
    ui->curatorComboBox->clear();
    ui->curatorComboBox->addItem("Оберіть куратора");
    ui->curatorComboBox->insertSeparator(1);
    ui->curatorComboBox->addItems(list);
}


void editGroup::setDataHeadManComboBox(QStringList list)
{
    ui->headManComboBox->clear();
    ui->headManComboBox->addItem("Оберіть старосту");
    ui->headManComboBox->insertSeparator(1);
    ui->headManComboBox->addItems(list);
}


void editGroup::setTheme(const QString style)
{
    if (style == "black")
    {
        setBlackUI();
    }
    else if (style == "white")
    {
        setWhiteUI();
    }
    else
    {
        setSystemUI();
    }
}


void editGroup::on_cancelButton_clicked()
{
    this->close();
}


void editGroup::on_saveButton_clicked()
{
    // проверка на заполненость данных
}


void editGroup::on_startStudySpinBox_valueChanged(int arg1)
{
    ui->endStudySpinBox->setMinimum(arg1 + 1);
}

