#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mainWindowInit()
{
    openSetting = new appSetting();
    openAuthorization = new authorization();
    db = QSqlDatabase::addDatabase("QSQLITE");


    // после покупки хостинга фикстить
    db.setDatabaseName("/Users/andrii/Desktop/Gradify/src/dataBase.db ");
    QMessageBox::information(this,"",QDir::currentPath() + "dataBase.db");

    if(!db.open())
    {
        QMessageBox::information(this,"","База даних не відкрилась");
    }


    query = new QSqlQuery(db);

    model = new QSqlTableModel(this,db);
    model->setTable("loginPassTable");
    model->select();

    ui->tableView->setModel(model);
    //ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();

    setWindowTitle("Gradify");
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);

    connect(this, &MainWindow::setThemeSettingsUI, openSetting, &appSetting::setThemeSettingUI);
    connect(this, &MainWindow::setThemeSettingsUI, openAuthorization, &authorization::setThemeAuthorUI);

    configRead();
    configInit();

    connect(openSetting, &appSetting::changeThemeApp, this, &MainWindow::setThemeUI);
    connect(openSetting, &appSetting::changeThemeApp, openAuthorization, &authorization::setThemeAuthorUI);

    ui->authorizationButton->setFocus();
}

void MainWindow::configDefault()
{
    config["theme"] = "black";
    configWrite();
}

void MainWindow::configRead()
{
    cfgFile.setFileName(QDir::currentPath() + "/cfg");
    if (!cfgFile.open(QIODevice::ReadOnly))
    {
        configDefault();
        return;
    }

    while (!cfgFile.atEnd())
    {
        QString cfgData = cfgFile.readLine();
        if (cfgData.isEmpty()) continue;
        QStringList list = cfgData.split("=");
        list[1].remove("\n");
        config[list[0]] = list[1];
    }

    cfgFile.close();
}

void MainWindow::configInit()
{
    if (config["theme"] == "white")
    {
        setWhiteUI();
    }
    else if (config["theme"] == "black")
    {
        setBlackUI();
    }

    emit setThemeSettingsUI(config["theme"]);
}

void MainWindow::configWrite()
{
    cfgFile.setFileName(QDir::currentPath() + "/cfg");
    if (!cfgFile.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&cfgFile);

    for (auto const& cfg : config.keys())
    {
        stream << cfg + "=" + config.value(cfg) + "\n";
    }

    cfgFile.close();
}

void MainWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::ActivationChange)
    {
        if (isActiveWindow())
        {
            setWindowOpacity(1);
        }
        else
        {
            setWindowOpacity(0.97);
        }
    }
}

void MainWindow::on_studentsTableButton_clicked()
{
    /*
     *
     * Код реализации открытия таблицы студентов
     *
    */

    clearStyleButtonTable();
    ui->studentsTableButton->setStyleSheet(selectButtonTableStyle);

    if (config["theme"] == "white")
    {
        ui->studentsTableButton->setIcon(QIcon(":/img/whiteMenuIcon/studentsIco.png"));
    }
    else if (config["theme"] == "black")
    {
        ui->studentsTableButton->setIcon(QIcon(":/img/blackMenuIcon/studenstIco.png"));
    }

    setWindowTitle("Gradify - (Студенти)");
}

void MainWindow::on_teachersTableButton_clicked()
{
    /*
     *
     * Код реализации открытия таблицы преподавателей
     *
    */
    clearStyleButtonTable();
    ui->teachersTableButton->setStyleSheet(selectButtonTableStyle);

    if (config["theme"] == "white")
    {
        ui->teachersTableButton->setIcon(QIcon(":/img/whiteMenuIcon/teachersIco.png"));
    }
    else if (config["theme"] == "black")
    {
        ui->teachersTableButton->setIcon(QIcon(":/img/blackMenuIcon/teachersIco.png"));
    }

    setWindowTitle("Gradify - (Викладачі)");
}

void MainWindow::on_gradesTableButton_clicked()
{
    /*
     *
     * Код реализации открытия таблицы оценок
     *
    */
    clearStyleButtonTable();
    ui->gradesTableButton->setStyleSheet(selectButtonTableStyle);

    if (config["theme"] == "white")
    {
        ui->gradesTableButton->setIcon(QIcon(":/img/whiteMenuIcon/raitingIco.png"));
    }
    else if (config["theme"] == "black")
    {
        ui->gradesTableButton->setIcon(QIcon(":/img/blackMenuIcon/raitingIco.png"));
    }

    setWindowTitle("Gradify - (Оцінки)");
}

void MainWindow::on_groupsTableButton_clicked()
{
    /*
     *
     * Код реализации открытия таблицы групп
     *
    */
    clearStyleButtonTable();
    ui->groupsTableButton->setStyleSheet(selectButtonTableStyle);
    if (config["theme"] == "white")
    {
        ui->groupsTableButton->setIcon(QIcon(":/img/whiteMenuIcon/groupIco.png"));
    }
    else if (config["theme"] == "black")
    {
        ui->groupsTableButton->setIcon(QIcon(":/img/blackMenuIcon/groupIco.png"));
    }

    setWindowTitle("Gradify - (Групи)");
}

void MainWindow::clearStyleButtonTable()
{
    ui->studentsTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->teachersTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->gradesTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->groupsTableButton->setStyleSheet(defaultButtonTableStyle);

    ui->studentsTableButton->setIcon(QIcon(":/img/pinkMenuicon/studentsIco.png"));
    ui->teachersTableButton->setIcon(QIcon(":/img/pinkMenuicon/teachersIco.png"));
    ui->gradesTableButton->setIcon(QIcon(":/img/pinkMenuicon/raitingIco.png"));
    ui->groupsTableButton->setIcon(QIcon(":/img/pinkMenuicon/groupIco.png"));
}

void MainWindow::setBlackUI()
{
    // код для задания черного интерфейса

    setWindowTitle("Gradify");
    clearStyleButtonTable();
    defaultButtonTableStyle = "QPushButton { border-radius:  6px; background-color:  "
                              "rgb(41,45,48); color: rgb(255, 255, 255); } "
                              "QPushButton:hover { background-color:  rgb(98, 98, 98); }";

    selectButtonTableStyle = "QPushButton{border-radius:  6px;background-color:  rgb(172, 72, 70);"
                             "color:  rgb(255, 255, 255);} "
                             "QPushButton:hover{background-color: rgb(172, 72, 70);color:  "
                             "rgb(255, 255, 255);}";

    defaultSettingButtonStyle = "QPushButton { border-radius:  6px; "
                                "background-color:  rgb(41, 45, 48); color:  rgb(255, 255, 255); }";

    ui->studentsTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->teachersTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->gradesTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->groupsTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportStudentsButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportTeachersButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportGradesButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportGroupsButton->setStyleSheet(defaultButtonTableStyle);
    ui->settingsButton->setStyleSheet(defaultSettingButtonStyle);
    ui->authorizationButton->setStyleSheet(defaultButtonTableStyle);
    ui->deleteRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->addRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->editRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->controlTableFrame->setStyleSheet("border-radius:  6px;color: white;background-color: rgb(61,65,68);");
    ui->leftMenuFrame->setStyleSheet("background-color: rgb(41,45,48);");
    ui->upMenuFrame->setStyleSheet("border: 0px");
    ui->mainTableFrame->setStyleSheet("color: white;background-color: rgb(41,45,48); border: 0px; border-radius: 16px;");
    setStyleSheet("MainWindow{background-color: rgb(29, 31, 32);}border: 0px;");
    ui->tableView->setStyleSheet("QHeaderView::section{"
                                 "size: 12px;"
                                 "color: white;"
                                 "background-color: rgb(61,65,68);"
                                 "padding-bottom:5px;"
                                 "padding-top:5px;}"
                                 "QTableView{"
                                 "background-color: rgb(61,65,68);"
                                 "color: white;}"
                                 "QTableView::item{"
                                 "color: white;"
                                 "text-align: center;"
                                 "border-style: 1px solid rgb(41,45,48);}"
                                 "QTableView::item:selected{color: white;background-color: rgb(232, 118, 123);}"
                                 "QScrollBar:vertical {\n"
                                 "border: none;\n"
                                 "background: rgb(61,65,68);;\n"
                                 "width: 14px;\n"
                                 "margin: 15px 0 15px 0;\n"
                                 "border-radius: 0px;\n}\n"
                                 "QScrollBar::handle:vertical {\n"
                                 "background-color: rgb(132,132,132);\n"
                                 "min-height: 20px;\n"
                                 "border-radius: 7px;\n}\n"
                                 "QScrollBar::handle:vertical:hover { \n"
                                 "background-color: rgb(152,152,152);\n}\n"
                                 "QScrollBar::handle:vertical:pressed {\n"
                                 "background-color: rgb(91,95,98);\n}\n"
                                 "QScrollBar::sub-line:vertical {\n"
                                 "border: none;\n"
                                 "background-color: rgb(91,95,98);\n"
                                 "height: 15px;\n"
                                 "border-top-left-radius: 7px;\n"
                                 "border-top-right-radius: 7px;\n"
                                 "subcontrol-position: top;\n"
                                 "subcontrol-origin: margin;\n}\n"
                                 "QScrollBar::sub-line:vertical:hover {\n"
                                 "background-color: rgb(111,115,118);\n}\n"
                                 "QScrollBar::sub-line:vertical:pressed { \n"
                                 "background-color: rgb(111,115,118);\n}\n"
                                 "QScrollBar::add-line:vertical {\n"
                                 "border: none;\n"
                                 "background-color: rgb(91,95,98);\n"
                                 "height: 15px;\n"
                                 "border-bottom-left-radius: 7px;\n"
                                 "border-bottom-right-radius: 7px;\n"
                                 "subcontrol-position: bottom;\n"
                                 "subcontrol-origin: margin;\n}\n"
                                 "QScrollBar::add-line:vertical:hover {\n"
                                 "background-color: rgb(111,115,118);\n}\n"
                                 "QScrollBar::add-line:vertical:pressed { \n"
                                 "background-color: rgb(111,115,118);\n}\n\n"
                                 "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {\n"
                                 "background: none;\n\n}\n"
                                 "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
                                 "background: none;\n");
}

void MainWindow::setWhiteUI()
{
    // код для задания белого интерфейса

    setWindowTitle("Gradify");
    clearStyleButtonTable();
    defaultButtonTableStyle = "QPushButton { border-radius:  6px; background-color:  "
                              "rgb(231,224,223); color: rgb(61, 60, 59); } "
                              "QPushButton:hover { background-color:  rgb(207, 201, 199); }";

    selectButtonTableStyle = "QPushButton{border-radius:  6px;background-color:  rgb(212, 112, 110);"
                             "color:  rgb(61, 60, 59);} "
                             "QPushButton:hover{background-color: rgb(232, 132, 130);color:  "
                             "rgb(61, 60, 59);}";

    defaultSettingButtonStyle = "QPushButton { border-radius:  6px; "
                                "background-color:  rgb(231, 224, 223); color:  rgb(61, 60, 59); }";

    ui->studentsTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->teachersTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->gradesTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->groupsTableButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportStudentsButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportTeachersButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportGradesButton->setStyleSheet(defaultButtonTableStyle);
    ui->reportGroupsButton->setStyleSheet(defaultButtonTableStyle);
    ui->settingsButton->setStyleSheet(defaultSettingButtonStyle);
    ui->authorizationButton->setStyleSheet(defaultButtonTableStyle);
    ui->deleteRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->addRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->editRowButton->setStyleSheet(defaultButtonTableStyle);
    ui->controlTableFrame->setStyleSheet("border-radius:  6px;color: white;background-color: white;");
    ui->leftMenuFrame->setStyleSheet("background-color: rgb(231,224,223);");
    ui->upMenuFrame->setStyleSheet("border: 0px");
    ui->mainTableFrame->setStyleSheet("color: black;background-color: rgb(231,224,223); border: 0px; border-radius: 16px;");
    setStyleSheet("MainWindow{background-color: rgb(255, 255, 255);}border: 0px;");
    ui->tableView->setStyleSheet("QHeaderView::section{"
                                 "size: 12px;"
                                 "color: black;"
                                 "background-color: white;"
                                 "padding-bottom:5px;"
                                 "padding-top:5px;}"
                                 "QTableView{"
                                 "background-color: white;"
                                 "color: black;}"
                                 "QTableView::item{"
                                 "color: black;"
                                 "text-align: center;"
                                 "border-style: 1px solid rgb(211,204,203);}"
                                 "QTableView::item:selected{color: black; background-color: rgb(232, 118, 123);}"
                                 "QScrollBar:vertical {\n"
                                 "border: none;\n"
                                 "background: rgb(245,245,245);\n"
                                 "width: 14px;\n"
                                 "margin: 15px 0 15px 0;\n"
                                 "border-radius: 0px;\n}\n"
                                 "QScrollBar::handle:vertical {\n"
                                 "background-color: rgb(212,212,212);\n"
                                 "min-height: 20px;\n"
                                 "border-radius: 7px;\n}\n"
                                 "QScrollBar::handle:vertical:hover { \n"
                                 "background-color: rgb(182,182,182);\n}\n"
                                 "QScrollBar::handle:vertical:pressed {\n"
                                 "background-color: rgb(182,182,182);\n}\n"
                                 "QScrollBar::sub-line:vertical {\n"
                                 "border: none;\n"
                                 "background-color: rgb(201,205,208);\n"
                                 "height: 15px;\n"
                                 "border-top-left-radius: 7px;\n"
                                 "border-top-right-radius: 7px;\n"
                                 "subcontrol-position: top;\n"
                                 "subcontrol-origin: margin;\n}\n"
                                 "QScrollBar::sub-line:vertical:hover {\n"
                                 "background-color: rgb(171,175,178);\n}\n"
                                 "QScrollBar::sub-line:vertical:pressed { \n"
                                 "background-color: rgb(171,175,178);\n}\n"
                                 "QScrollBar::add-line:vertical {\n"
                                 "border: none;\n"
                                 "background-color: rgb(201,205,208);\n"
                                 "height: 15px;\n"
                                 "border-bottom-left-radius: 7px;\n"
                                 "border-bottom-right-radius: 7px;\n"
                                 "subcontrol-position: bottom;\n"
                                 "subcontrol-origin: margin;\n}\n"
                                 "QScrollBar::add-line:vertical:hover {\n"
                                 "background-color: rgb(171,175,178);\n}\n"
                                 "QScrollBar::add-line:vertical:pressed { \n"
                                 "background-color: rgb(171,175,178);\n}\n\n"
                                 "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {\n"
                                 "background: none;\n\n}\n"
                                 "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
                                 "background: none;\n");
}

void MainWindow::setThemeUI(const QString style)
{
    if(style == "black")
    {
        setBlackUI();
        config["theme"] = "black";
        configWrite();
    }
    else
    {
        setWhiteUI();
        config["theme"] = "white";
        configWrite();
    }
}

void MainWindow::on_settingsButton_clicked()
{
    openSetting->show();
    openAuthorization->close();
}

void MainWindow::on_authorizationButton_clicked()
{
    openAuthorization->show();
    openSetting->close();
}

void MainWindow::on_addRowButton_clicked()
{
    model->insertRow(model->rowCount());
    ui->tableView->scrollToBottom();
    ui->tableView->selectRow(model->rowCount() - 1);
}


void MainWindow::on_deleteRowButton_clicked()
{
    bool ok;
    int inputNum = QInputDialog::getInt(this, tr("Видалення запису по ключовому полю"),
                                              tr("Введіть номер ключового поля:"), row+1, 1, model->rowCount(), 1, &ok); // model->rowCount() - максимальное выбранное число (в нашем
                                              // случае максимальный выбор заканчивается на общем количестве записей)
                                              // первая 1 означает текущий выбор за умолчанием
                                              // вторая 1 означает минимальное значение выбор
                                              // третья 1 означает шаг с которым выбирается индекс
    if(ok)
    {
        model->removeRow(inputNum - 1);
        model->select();
    }
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}

