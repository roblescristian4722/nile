#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableLoginPB()
{
    regex regEmail("[A-Za-z0-9-_.]*");
    if (ui->usernameLE->text().length() && ui->passwordLE->text().length())
    {
        if (regex_match(ui->usernameLE->text().toStdString(), regEmail))
            ui->loginPB->setEnabled(true);
        else
            ui->loginPB->setEnabled(false);
    }
    else
        ui->loginPB->setEnabled(false);
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableLoginPB();
}

void MainWindow::on_usernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableLoginPB();
}

void MainWindow::on_newUsernameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::on_newEmailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableCreatePB();
}

void MainWindow::enableCreatePB()
{
    ui->createPB->setEnabled(false);
    if (ui->newPasswordLE->text().length() && ui->newEmailLE->text().length()
        && ui->newUsernameLE->text().length())
        ui->createPB->setEnabled(true);
}

void MainWindow::on_createPB_clicked()
{
    bool create = false;
    unsigned int i;
    regex regEmail("[a-zA-Z0-9-_.]+@[a-zA-Z0-9]+.+[a-zA-Z]");
    regex regUser("[A-Za-z0-9-_.]*");
    User userTmp;
    userTmp.setEmail(ui->newEmailLE->text());
    userTmp.setPassword(ui->newPasswordLE->text());
    userTmp.setUsername(ui->newUsernameLE->text());

    if (regex_match(userTmp.getEmail().toStdString(), regEmail))
    {
        if (regex_match(userTmp.getUsername().toStdString(), regUser))
        {
            for (i = 0; i < m_users.size(); i++)
            {
                if (userTmp.getEmail() == m_users.at(i).getEmail())
                    QMessageBox::warning(this, "invalid email",
                                         "email is already in use");
                else if (userTmp.getUsername() == m_users.at(i).getUsername())
                    QMessageBox::warning(this, "invalid username",
                                         "username is already in use");
                else
                {
                    create = true;
                    QMessageBox::about(this, "User created", "User created succesfully");
                }
            }
            if (!i)
                create = true;
        }
        else
            QMessageBox::warning(this, "invalid username",
                                 "username needs an alphanumeric format,"
                                 "perdiods, dash and/or underscore");
    }
    else
        QMessageBox::warning(this, "invalid email",
                             "email needs an alphanumeric format, period, dash and/or underscore "
                             "followed by \"@<domain>.<extension>\"");
    if (create)
    {
        m_users.push_back(userTmp);
        QMessageBox::about(this, "User created", "User created succesfully");
    }

    ui->newEmailLE->clear();
    ui->newPasswordLE->clear();
    ui->newUsernameLE->clear();
}

void MainWindow::on_loginPB_clicked()
{
    unsigned int i;
    for (i = 0; i < m_users.size(); i++)
    {
        if (m_users.at(i).getUsername() == ui->usernameLE->text())
        {
            if (m_users.at(i).getPassword() == ui->passwordLE->text())
            {
                ui->viewSW->setCurrentIndex(1);
                QMessageBox::about(this, "Welcome", "Welcome to Nile!!");
            }
            else
            {
                QMessageBox::warning(this, "Invalid user/password", "Invalid user/password");
            }
            break;
        }
    }
    if (i == m_users.size())
        QMessageBox::warning(this, "Invalid user/password", "Invalid user/password");
    ui->usernameLE->clear();
    ui->passwordLE->clear();
}
