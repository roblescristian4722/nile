#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Nile");
    this->setMinimumSize(600, 600);
    ui->productosSA->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->productosSA->setWidgetResizable(true);

    m_fileMenu = ui->menubar->addMenu(tr("&File"));
    m_openFileAction = new QAction(tr("Open File"), this);
    // Conectar evento y comportamiento
    // connect(Quién envía, Qué envía, Quién responde, Con qué responde)
    connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    m_fileMenu->addAction(m_openFileAction);
}

MainWindow::~MainWindow()
{
    //saveDB();
    for (int i = 0; i < m_products.size(); ++i)
        delete m_products[i];
    m_products.clear();
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if (ui->emailLE->text().length() && ui->passwordLE->text().length())
    {
        ui->loginPB->setEnabled(true);
    }
    else
        ui->loginPB->setEnabled(false);
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
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

void MainWindow::openFile()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this, "Nile Database",
                                            "", "JSON files (*.json)");
    if (fileName.length() > 0)
    {
        m_dbFile.setFileName(fileName);
        loadDB();
        ui->loginFrame->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->loginGB->setEnabled(true);
    }
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
    QJsonObject jsonObj;
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
            if (m_users.empty())
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
        jsonObj["name"] = userTmp.getUsername();
        jsonObj["email"] = userTmp.getEmail();
        jsonObj["password"] = userTmp.getPassword();
        m_database.append(jsonObj);
        QMessageBox::about(this, "User created", "User created succesfully");
    }

    ui->newEmailLE->clear();
    ui->newPasswordLE->clear();
    ui->newUsernameLE->clear();
}

void MainWindow::on_loginPB_clicked()
{
    bool success = false;
    unsigned int i;
    for (i = 0; i < m_users.size(); i++)
    {
        if (m_users.at(i).getEmail() == ui->emailLE->text())
        {
            if (m_users.at(i).getPassword() == ui->passwordLE->text())
            {
                ui->viewSW->setCurrentIndex(1);
                QMessageBox::about(this, "Welcome", "Welcome to Nile!!");

                success = true;
            }
            else
                QMessageBox::warning(this, "Invalid user/password", "Invalid user/password");
            break;
        }
    }
    if (i == m_users.size())
        QMessageBox::warning(this, "Invalid user/password", "Invalid user/password");
    ui->emailLE->clear();
    ui->passwordLE->clear();

    if (success)
        showProducts();
}

void MainWindow::saveDB()
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    jsonObj["users"] = m_database;
    jsonDoc = QJsonDocument(jsonObj);

    m_dbFile.open(QIODevice::WriteOnly);
    m_dbFile.write(jsonDoc.toJson());
    m_dbFile.close();
}

void MainWindow::loadDB()
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    QByteArray data;

    QJsonObject jsonAux;
    User userAux;

    m_dbFile.open(QIODevice::ReadOnly);
    data = m_dbFile.readAll();
    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    m_database = jsonObj["users"].toArray();
    m_productDb = jsonObj["products"].toArray();

    for (int i = 0; i < m_database.size(); i++)
    {
        jsonAux = m_database[i].toObject();
        userAux.setEmail(jsonAux["email"].toString());
        userAux.setUsername(jsonAux["name"].toString());
        userAux.setPassword(jsonAux["password"].toString());
        m_users.push_back(userAux);
    }

    m_dbFile.close();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    enableLoginPB();
}

void MainWindow::showProducts()
{
    QJsonObject auxJson;
    Producto* tmp;
    QGridLayout *layout = new QGridLayout;
    QString img;

    for (int i = 0; i < m_productDb.size(); ++i)
    {
        tmp = new Producto;
        // Se obtiene el primer objeto de la tabla "products"
        // de la base de datos JSON
        auxJson = m_productDb[i].toObject();

        // Se obtiene el nombre de dicho objeto y se c
        tmp->setMinimumSize(100, 100);
        tmp->setMaximumWidth(500);
        tmp->changeName(auxJson["name"].toString());
        tmp->changePrice(auxJson["price"].toString());
        img = auxJson["id"].toString();
        img += ".jpg";
        tmp->changeImage(img);
        addToGrid(layout, tmp);
        m_products.push_back(tmp);
    }
    ui->tmpW->setLayout(layout);
}

void MainWindow::addToGrid(QGridLayout *layout, Producto* producto)
{
    static int column = 0;
    static int row = 0;
    layout->addWidget(producto, row, column);
    if (column == MAX_ROWS - 1)
        ++row;
    ++column;
    column %= MAX_ROWS;
}
