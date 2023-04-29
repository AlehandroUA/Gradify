#include "groupwindow.h"
#include "ui_groupwindow.h"

#include <QFile>
#include <QMessageBox>

groupWindow::groupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::groupWindow)
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


groupWindow::~groupWindow()
{
    delete ui;
}


void groupWindow::setBlackUI()
{
    ui->mainImage->setPixmap(QPixmap(":/img/whiteMenuIcon/groupIco.png"));
    QFile file(":/styles/black/recordsForms/recordsForms.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}


void groupWindow::setWhiteUI()
{
    ui->mainImage->setPixmap(QPixmap(":/img/blackMenuIcon/groupIco.png"));
    QFile file(":/styles/white/recordsForms/recordsForms.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}


void groupWindow::setSystemUI()
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


void groupWindow::setData(QString titleName, QStringList listData)
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

    ui->okLabel->setVisible(false);
}


void groupWindow::setDataCuratorComboBox(QStringList list)
{
    ui->curatorComboBox->clear();
    ui->curatorComboBox->addItem("Оберіть куратора");
    ui->curatorComboBox->insertSeparator(1);
    ui->curatorComboBox->addItems(list);
}


void groupWindow::setDataHeadManComboBox(QStringList list)
{
    ui->headManComboBox->clear();
    ui->headManComboBox->addItem("Оберіть старосту");
    ui->headManComboBox->insertSeparator(1);
    ui->headManComboBox->addItems(list);
}


void groupWindow::setTheme(const QString style)
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


void groupWindow::on_cancelButton_clicked()
{
    this->close();
}


void groupWindow::on_saveButton_clicked()
{
    if (!ui->nameLineEdit->text().isEmpty() and
        ui->specialComboBox->currentIndex() != 0 and
        ui->curatorComboBox->currentIndex() != 0 and
        ui->headManComboBox->currentIndex() != 0)
    {
        ui->okLabel->setVisible(true);
        emit sendData(getCurrentData());
    }
    else if (ui->nameLineEdit->text().isEmpty())
    {
        ui->nameLineEdit->setFocus();
        QMessageBox::critical(this,"","Введіть назву групи");
    }
    else if (ui->specialComboBox->currentIndex() == 0)
    {
        ui->specialComboBox->setFocus();
        QMessageBox::critical(this,"","Оберіть спеціальність групи");
    }
    else if (ui->curatorComboBox->currentIndex() == 0)
    {
        ui->curatorComboBox->setFocus();
        QMessageBox::critical(this,"","Оберіть куратора групи");
    }
    else if (ui->headManComboBox->currentIndex() == 0)
    {
        ui->headManComboBox->setFocus();
        QMessageBox::critical(this,"","Оберіть старосту групи");
    }
}


void groupWindow::on_startStudySpinBox_valueChanged(int arg1)
{
    ui->endStudySpinBox->setMinimum(arg1 + 1);
}


QStringList groupWindow::getCurrentData()
{
    QStringList dataList;

    dataList << QString::number(idRowEdit);
    dataList << ui->nameLineEdit->text();
    dataList << ui->specialComboBox->currentText();
    dataList << ui->startStudySpinBox->text();
    dataList << ui->endStudySpinBox->text();
    dataList << ui->curatorComboBox->currentText();
    dataList << ui->headManComboBox->currentText();

    return dataList;
}
