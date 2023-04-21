#include "filterform.h"
#include "ui_filterform.h"

#include <QMessageBox>
#include <QRegularExpression>

filterForm::filterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filterForm)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setFixedSize(width(), height());

    ui->conditionLineEdit_2->setVisible(false);
    ui->conditionComboBox->installEventFilter(this);
    ui->tableComboBox->installEventFilter(this);

    currentTabelSelect = "NULL";
    oldColumnSelect = "NULL";
    ui->clearFilterPushButton->setEnabled(false);
    ui->conditionDataEdit->setVisible(false);
    close();
}


filterForm::~filterForm()
{
    delete ui;
}


bool filterForm::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        if (ui->conditionComboBox->hasFocus())
        {
            ui->conditionComboBox->setGraphicsEffect(paintDropRedShadowEffect());
        }
        else if (ui->tableComboBox->hasFocus())
        {
            ui->tableComboBox->setGraphicsEffect(paintDropRedShadowEffect());
        }
    }
    else if (event->type() == QEvent::FocusOut and !this->hasFocus())
    {
        ui->conditionComboBox->setGraphicsEffect(nullptr);
        ui->tableComboBox->setGraphicsEffect(nullptr);
    }

    return false;
}


void filterForm::setListTable(QString tableName, const QMap<QString, QString> tableNameType)
{
    if (currentTabelSelect != tableName)
    {
        ui->clearFilterPushButton->setEnabled(false);
        ui->conditionComboBox->setEnabled(false);
        QStringList columnList = tableNameType.keys();

        ui->tableComboBox->clear();
        ui->conditionLineEdit->clear();
        ui->conditionLineEdit_2->clear();

        ui->tableComboBox->addItem("Оберіть колонку", 0);
        ui->tableComboBox->insertSeparator(1);
        ui->tableComboBox->addItems(columnList);
        ui->conditionComboBox->setCurrentIndex(0);

        currentTabelSelect = tableName;
        typeColumnsMap = tableNameType;

        ui->conditionLineEdit->setPlaceholderText("Текст");
        ui->conditionLineEdit->setValidator(nullptr);
    }
}


void filterForm::on_filterPushButton_clicked()
{
    if (ui->tableComboBox->currentIndex() > 0
        and ui->conditionComboBox->currentIndex() > 0
        and !ui->conditionLineEdit->text().isEmpty()
        and currentPlaceHolderText != "Дата")
    {
        strSqlFilter = "SELECT `" + currentTabelSelect + "`.*";
        strSqlFilter += "\nFROM `" + currentTabelSelect + "`";
        strSqlFilter += "\nWHERE `" + currentTabelSelect +"`.`" + ui->tableComboBox->currentText() + "` ";

        if (currentPlaceHolderText == "Число")
        {
            if (ui->conditionComboBox->currentText() != "between")
            {
                strSqlFilter += ui->conditionComboBox->currentText() + " " + ui->conditionLineEdit->text();
            }
            else if (ui->conditionComboBox->currentText() == "between" and !ui->conditionLineEdit_2->text().isEmpty()
                     and ui->conditionLineEdit->text().toInt() < ui->conditionLineEdit_2->text().toInt())
            {
                strSqlFilter += "BETWEEN " + ui->conditionLineEdit->text() + " AND " + ui->conditionLineEdit_2->text();
            }
            else if (ui->conditionComboBox->currentText() == "between" and ui->conditionLineEdit_2->text().isEmpty())
            {
                QMessageBox::critical(this,"","Введіть значення умови фільтрування!");
                ui->conditionLineEdit_2->setFocus();
                return;
            }
            else if (ui->conditionComboBox->currentText() == "between"
                     and ui->conditionLineEdit->text().toInt() >= ui->conditionLineEdit_2->text().toInt())
            {
                QMessageBox::critical(this,"","Перше значення повине бути меншим за друге!");
                ui->conditionLineEdit->setFocus();
                return;
            }
        }
        else if (currentPlaceHolderText == "Текст")
        {
            if (ui->conditionComboBox->currentText() == "like%")
            {
                strSqlFilter += "LIKE '" + ui->conditionLineEdit->text() + "%'";
            }
            else if (ui->conditionComboBox->currentText() == "%like")
            {
                strSqlFilter += "LIKE '%" + ui->conditionLineEdit->text() + "'";
            }
            else if (ui->conditionComboBox->currentText() == "%like%")
            {
                strSqlFilter += "LIKE '%" + ui->conditionLineEdit->text() + "%'";
            }
        }

        emit sendFilter(strSqlFilter, ui->tableComboBox->currentText());
        ui->clearFilterPushButton->setEnabled(true);
    }

    else if (ui->tableComboBox->currentIndex() > 0
             and ui->conditionComboBox->currentIndex() > 0
             and currentPlaceHolderText == "Дата")
    {
        strSqlFilter = "SELECT `" + currentTabelSelect + "`.*";
        strSqlFilter += "\nFROM `" + currentTabelSelect + "`";
        strSqlFilter += "\nWHERE `" + currentTabelSelect +"`.`" +
                        ui->tableComboBox->currentText() + "` " + ui->conditionComboBox->currentText()
                        + " '" + QString::number(ui->conditionDataEdit->date().year()) + "-"
                        + QString::number(ui->conditionDataEdit->date().month()) + "-"
                        + QString::number(ui->conditionDataEdit->date().day()) + "'";

        emit sendFilter(strSqlFilter, ui->tableComboBox->currentText());
        ui->clearFilterPushButton->setEnabled(true);
        show();
    }
    else if (ui->tableComboBox->currentIndex() == 0)
    {
        QMessageBox::critical(this,"","Оберіть потрібну колонку!");
        ui->tableComboBox->setFocus();
        show();
    }
    else if (ui->conditionComboBox->currentIndex() == 0)
    {
        QMessageBox::critical(this,"","Оберіть умову!");
        ui->conditionComboBox->setFocus();
        show();
    }
    else
    {
        QMessageBox::critical(this,"","Введіть значення умови фільтрування");
        ui->conditionLineEdit->setFocus();
        show();
    }
}


void filterForm::on_conditionComboBox_currentTextChanged(const QString &arg1)
{
    ui->conditionComboBox->clearFocus();

    if (ui->conditionComboBox->currentText() == "between" and currentPlaceHolderText != "Дата")
    {
        ui->conditionLineEdit_2->setVisible(true);

        ui->conditionLineEdit->setPlaceholderText("1");
        ui->conditionLineEdit_2->setPlaceholderText("2");

        ui->conditionComboBox->resize(QSize(95, 21));

        ui->conditionLineEdit->move(110, ui->conditionLineEdit->y());
        ui->conditionLineEdit->resize(ui->conditionLineEdit->height() + 30,
                                      ui->conditionLineEdit->height());

        ui->conditionLineEdit_2->resize(ui->conditionLineEdit->height() + 30,
                                        ui->conditionLineEdit->height());

        ui->conditionLineEdit_2->move(ui->conditionLineEdit->x() + 58,
                                      ui->conditionLineEdit->y());
    }
    else if (currentPlaceHolderText != "Дата")
    {
        ui->conditionComboBox->resize(QSize(140, 21));
        ui->conditionLineEdit->setPlaceholderText(currentPlaceHolderText);
        ui->conditionLineEdit->resize(64, ui->conditionLineEdit->height());
        ui->conditionLineEdit->move(156, ui->conditionLineEdit->y());
        ui->conditionLineEdit_2->setVisible(false);
        ui->conditionLineEdit_2->clear();
    }
}


void filterForm::on_conditionLineEdit_editingFinished()
{
    ui->conditionLineEdit->clearFocus();
}


void filterForm::on_tableComboBox_currentTextChanged(const QString &arg1)
{
    ui->tableComboBox->clearFocus();

    if (typeColumnsMap.value(arg1) == "text" and typeColumnsMap.value(oldColumnSelect) != typeColumnsMap.value(arg1))
    {
        setStringTypeComboBox();
    }
    else if (typeColumnsMap.value(arg1) == "int" and typeColumnsMap.value(oldColumnSelect) != typeColumnsMap.value(arg1))
    {
        setIntTypeComboBox();
    }
    else if (typeColumnsMap.value(arg1) == "date" and typeColumnsMap.value(oldColumnSelect) != typeColumnsMap.value(arg1))
    {
        setDateTypeComboBox();
    }
    else if (ui->tableComboBox->currentIndex() == 0)
    {
        setDisabledComboBox();
    }

    oldColumnSelect = arg1;
}


void filterForm::setIntTypeComboBox()
{
    ui->conditionComboBox->setEnabled(true);
    ui->conditionComboBox->clear();
    ui->conditionComboBox->addItem("Оберіть умову", 0);
    ui->conditionComboBox->insertSeparator(1);
    ui->conditionComboBox->addItem(">", 2);
    ui->conditionComboBox->addItem("<", 3);
    ui->conditionComboBox->addItem("<=", 4);
    ui->conditionComboBox->addItem(">=", 5);
    ui->conditionComboBox->addItem("=", 6);
    ui->conditionComboBox->addItem("between", 7);

    ui->conditionLineEdit->setValidator(new QIntValidator(0, 10000, this));
    ui->conditionLineEdit_2->setValidator(new QIntValidator(0, 10000, this));

    currentPlaceHolderText = "Число";
    ui->conditionLineEdit->setPlaceholderText(currentPlaceHolderText);

    if (!ui->conditionLineEdit->text().toInt())
    {
        ui->conditionLineEdit->clear();
    }
    else if (!ui->conditionLineEdit_2->text().toInt())
    {
        ui->conditionLineEdit_2->clear();
    }

    ui->conditionLineEdit->setVisible(true);
    ui->conditionLineEdit_2->setVisible(false);
    ui->conditionDataEdit->setVisible(false);
    ui->conditionDataEdit->clear();

    ui->conditionComboBox->resize(QSize(140, 21));
}


void filterForm::setStringTypeComboBox()
{
    ui->conditionComboBox->setEnabled(true);
    ui->conditionComboBox->clear();
    ui->conditionComboBox->addItem("Оберіть умову", 0);
    ui->conditionComboBox->insertSeparator(1);
    ui->conditionComboBox->addItem("like%", 2);
    ui->conditionComboBox->addItem("%like", 3);
    ui->conditionComboBox->addItem("%like%", 4);

    ui->conditionLineEdit->setValidator(nullptr);
    ui->conditionLineEdit_2->setValidator(nullptr);

    currentPlaceHolderText = "Текст";
    ui->conditionLineEdit->setPlaceholderText(currentPlaceHolderText);

    ui->conditionLineEdit->setVisible(true);
    ui->conditionLineEdit_2->setVisible(false);
    ui->conditionDataEdit->setVisible(false);
    ui->conditionDataEdit->clear();

    ui->conditionComboBox->resize(QSize(140, 21));
}


void filterForm::setDateTypeComboBox()
{
    ui->conditionComboBox->setEnabled(true);
    ui->conditionComboBox->clear();
    ui->conditionComboBox->addItem("Оберіть умову", 0);
    ui->conditionComboBox->insertSeparator(1);
    ui->conditionComboBox->addItem(">", 2);
    ui->conditionComboBox->addItem("<", 3);
    ui->conditionComboBox->addItem("<=", 4);
    ui->conditionComboBox->addItem(">=", 5);
    ui->conditionComboBox->addItem("=", 6);
    //ui->conditionComboBox->addItem("between", 7);


    // нужно доделать регулярное выражение чтобы не вводить больше 31 дня и 12 месяцев
    //ui->conditionLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^(\\d\\d)\\.(\\d\\d)\\.(\\d\\d)$"), this));
    //ui->conditionLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^(\\d\\d)\\-(\\d\\d)\\-(\\d\\d)$"), this));
    currentPlaceHolderText = "Дата";
    //ui->conditionLineEdit->setPlaceholderText(currentPlaceHolderText);

    ui->conditionLineEdit->setVisible(false);
    ui->conditionLineEdit_2->setVisible(false);
    ui->conditionDataEdit->setVisible(true);
    ui->conditionLineEdit->clear();

    ui->conditionDataEdit->move(140, 50);
    ui->conditionComboBox->resize(QSize(130, 21));
}


void filterForm::setDisabledComboBox()
{
    ui->conditionComboBox->setEnabled(false);
    ui->conditionComboBox->setCurrentIndex(0);

    ui->conditionLineEdit->clear();
    ui->conditionLineEdit_2->clear();
}


QString filterForm::reverseDate(QString str)
{
    QString tempStr = str;

    tempStr[0] = str[str.length() - 2];
    tempStr[1] = str[str.length() - 1];

    tempStr[str.length() - 2] = str[0];
    tempStr[str.length() - 1] = str[1];

    return tempStr;
}


QGraphicsDropShadowEffect *filterForm::paintDropRedShadowEffect()
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(25);
    effect->setColor(QColor(196, 30, 58));
    effect->setOffset(QPointF(0, 0));
    return effect;

}

void filterForm::on_clearFilterPushButton_clicked()
{
    ui->clearFilterPushButton->setEnabled(false);
    emit clearFilter();
}

