#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <vector>
#include <regex>
#include "user.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_passwordLE_textChanged(const QString &arg1);
    void on_usernameLE_textChanged(const QString &arg1);
    void on_newUsernameLE_textChanged(const QString &arg1);
    void on_newEmailLE_textChanged(const QString &arg1);
    void on_newPasswordLE_textChanged(const QString &arg1);

    void enableLoginPB();
    void enableCreatePB();

    void on_createPB_clicked();

    void on_loginPB_clicked();

private:
    Ui::MainWindow *ui;
    vector<User> m_users;
};
#endif // MAINWINDOW_H
