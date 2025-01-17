#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qpushbutton.h"
#include <QMainWindow>
#include <QString>
#include <QEvent>
#include <QFile>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QTableView>

#include <QTranslator>

#include <appsetting.h>
#include <authorizationform.h>
#include <filterform.h>
#include <queryform.h>
#include <aboutapp.h>

#include <recordsForms/gradewindow.h>
#include <recordsForms/groupwindow.h>
#include <recordsForms/studentwindow.h>
#include <recordsForms/subjectwindow.h>
#include <recordsForms/teacherwindow.h>

#include <statisticsForms/gradestatistics.h>
#include <statisticsForms/groupstatistics.h>
#include <statisticsForms/studentstatistics.h>
#include <statisticsForms/subjectstatistics.h>
#include <statisticsForms/teacherstatistics.h>

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
    void closeEvent(QCloseEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_studentsTableButton_clicked();
    void on_teachersTableButton_clicked();
    void on_gradesTableButton_clicked();
    void on_groupsTableButton_clicked();
    void clearStyleButtonTable();
    void on_settingsButton_clicked();

    void setBlackUI();
    void setWhiteUI();
    void setSystemUI();

    void setCurrentIconAction();
    void mainWindowInit();
    void configDefault();
    void configInit();
    void configWrite(const QString &key, const QVariant &value);
    void userLogout();

    void on_authorizationButton_clicked();
    void on_addRowButton_clicked();
    void on_deleteRowButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_subjectsTableButton_clicked();

    void clearSelectTable();
    void closeAllPopUpWindow();
    void closeAllEditForm();
    void closeAllStatisticsForm();

    void setEnabledButtons(const bool &status);
    void setEnabledActions(const bool &status);
    void setEnabledEditButton(const bool &status);

    void on_addRowAction_triggered();
    void on_deleteRowAction_triggered();
    void on_editRowAction_triggered();
    void on_editRowButton_clicked();
    void on_openStudTabAction_triggered();
    void on_openTeachTabAction_triggered();
    void on_openGradesTabAction_triggered();
    void on_openGroupTabAction_triggered();
    void on_openSubjTabAction_triggered();
    void on_studentsReportButton_clicked();
    void on_teachersReportButton_clicked();
    void on_gradesReportButton_clicked();
    void on_groupsReportButton_clicked();
    void on_subjectsReportButton_clicked();

    void on_openManual_triggered();

    void on_studentsReportAction_triggered();
    void on_teachersReportAction_triggered();
    void on_groupsReportAction_triggered();
    void on_gradesReportAction_triggered();
    void on_subjectsReportAction_triggered();
    void on_currentTableReportAction_triggered();
    void on_currentTableReportButton_clicked();

    void on_about_triggered();
    void on_filterButton_clicked();
    void on_queryButton_clicked();

    void printDocumentToPDF(const QString path, const QString html);
    void printDocumentToHTML(const QString path, const QString html);

    QString getHeaderHTML();
    void fillHTMLTable(QString& textHTML, QTableView* tableView);

    QGraphicsDropShadowEffect *paintDropShadowEffect();

    QMap<QString, QString> getColumnsNamesAndDatatypes(const QString &tableName);

    QStringList getCurrentItemTable();
    QStringList getRowData(const int &row);
    QStringList getGroupsNames();
    QStringList getGroupsSpecial();
    QStringList getStudentsNames();
    QStringList getTeachersNames();
    QStringList getSubjectsNames();
    QStringList getSubjectsTypes();
    QStringList getCategoryTeachers();

    void on_statisticsButton_clicked();

    QString modelDataToString(QAbstractItemModel* model);

    void on_actionCSV_triggered();
    void on_actionTXT_triggered();

    void on_actionEnglish_Translate_triggered();

    void on_actionUkrainian_Translate_triggered();

private:
    Ui::MainWindow *ui;

    QTranslator translator;

    appSetting *settingWindow;
    authorizationForm *authorizationWindow;
    filterForm *filterWindow;
    queryForm *queryWindow;
    aboutApp *aboutAppWindow;

    gradeWindow *gradeForm;
    groupWindow *groupForm;
    studentWindow *studentForm;
    subjectWindow *subjectForm;
    teacherWindow *teacherForm;

    gradeStatistics *gradeStat;
    groupStatistics *groupStat;
    studentStatistics *studentStat;
    subjectStatistics *subjectStat;
    teacherStatistics *teacherStat;

    QString selectButtonTableStyle;
    QString selectButtonAuthStyle;
    QString theme;

    QFile cfgFile;
    QFile styleF;

    QSqlDatabase db;
    QSqlQuery *query;
    QSqlQueryModel *queryModel;
    QSqlTableModel *model;

    QMessageBox logoutMessageBox;
    QPushButton *yesButton;

    QGraphicsDropShadowEffect *effect;

    int row;
    int currentSelectTable;

    bool isLogin;

public slots:
    void setTheme(const QString &style);
    void authorization(const QString &login);
    void setFilterForTable(const QString &filterQuery,
                           const QString &currentColumnFilter);
    void clearFilterForTable();
    void goSearch();
    void setDataToModel(QStringList dataList, bool isNewRow);
    void setQueryForTable(QString query);

signals:
    void setThemeSettingsUI(const QString);
    void statusAuthorization(const bool);
    void changedGradeTable(const int);
    void setTableForFilter(const QMap<QString, QString>);
    void setDataEditForm(QString, QStringList);
    void sendGroupsList(QStringList);
    void sendTeachersList(QStringList);
    void sendStudentsList(QStringList);
    void sendCurrentGroup(QString);
    void sendSubjectsList(QStringList);
    void createNewRow();
    void updateStatisticsSignal();
    void updateStatisticsComboBoxSignal();
};
#endif // MAINWINDOW_H
