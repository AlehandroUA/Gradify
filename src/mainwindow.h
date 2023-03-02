#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QEvent>
#include <QFile>
#include <QMap>

#include <appsetting.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeEvent(QEvent *event) override;

private slots:
    void on_studentsTableButton_clicked();

    void on_teachersTableButton_clicked();

    void on_gradesTableButton_clicked();

    void on_groupsTableButton_clicked();

    void clearStyleButtonTable();

    void on_settingsButton_clicked();

    void setBlackUI();

    void setWhiteUI();

    void mainWindowInit();

    void configDefault();

    void configRead();

    void configInit();

    void configWrite();

private:
    Ui::MainWindow *ui;
    appSetting *openSetting;

    QString defaultButtonTableStyle;
    QString selectButtonTableStyle;
    QString defaultSettingButtonStyle;

    QMap<QString, QString> config;

    QFile cfgFile;

public slots:
    void setThemeUI(QString style);

signals:
    void setThemeSettingsUI(QString);
};
#endif // MAINWINDOW_H
