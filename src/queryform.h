#ifndef QUERYFORM_H
#define QUERYFORM_H

#include <QWidget>

namespace Ui {
class queryForm;
}

class queryForm : public QWidget
{
    Q_OBJECT

public:
    explicit queryForm(QWidget *parent = nullptr);
    ~queryForm();

private slots:
    void on_mySQLPushButton_clicked();
    void on_succesStudentPushButton_clicked();
    void on_searchGradeStudentButton_clicked();

    QString getDateWithDialog();

    void on_avgScorePushButton_clicked();

private:
    Ui::queryForm *ui;

    int changedGradeTable;

public slots:
    void selectedGradeTable(int status);

signals:
    void sendQuery(const QString);
    void sendFilter(const QString, const QString);
    void clearFilter();
};

#endif // QUERYFORM_H
