#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Nile");
    this->setMinimumSize(700, 600);
    ui->productosSA->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->productosSA->setWidgetResizable(true);

    m_layoutRow = 0;
    m_layoutColumn = 0;
    m_layout = new QGridLayout;
    m_fileMenu = ui->menubar->addMenu(tr("&File"));
    m_openFileAction = new QAction(tr("Open File"), this);
    // Conectar evento y comportamiento
    // connect(Quién envía, Qué envía, Quién responde, Con qué responde)
    connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    m_fileMenu->addAction(m_openFileAction);
}

MainWindow::~MainWindow()
{
    saveDB();
    while (m_layout->takeAt(0) != nullptr)
        delete m_layout->takeAt(0);
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

void MainWindow::on_passwordLE_textChanged(const QString &index)
{
    Q_UNUSED(index)
    enableLoginPB();
}

void MainWindow::on_newUsernameLE_textChanged(const QString &index)
{
    Q_UNUSED(index)
    enableCreatePB();
}

void MainWindow::on_newEmailLE_textChanged(const QString &index)
{
    Q_UNUSED(index)
    enableCreatePB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &index)
{
    Q_UNUSED(index)
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
                {
                    QMessageBox::warning(this, "invalid email",
                                         "email is already in use");
                    break;
                }
                else if (userTmp.getUsername() == m_users.at(i).getUsername())
                {
                    QMessageBox::warning(this, "invalid username",
                                         "username is already in use");
                    break;
                }
            }
            if (i == m_users.size())
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
                break;
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
        on_categoriaCB_currentIndexChanged(CATEGORIA_TODOS);
}

void MainWindow::saveDB()
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;

    jsonObj["users"] = m_database;
    jsonObj["products"] = m_productDb;
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

void MainWindow::on_emailLE_textChanged(const QString &index)
{
    Q_UNUSED(index)
    enableLoginPB();
}

void MainWindow::showProducts(regex category, regex search)
{
    QJsonObject auxJson;
    Producto* tmp;
    QString img;
    QString name;
    float floatTmp;

    // Se eliminan los datos anteriores
    removeLayoutW();

    // Se ingresan los nuevos datos al layout
    for (int i = 0; i < m_productDb.size(); ++i)
    {
        tmp = new Producto;
        // Se obtiene el primer objeto de la tabla "products"
        // de la base de datos JSON
        auxJson = m_productDb[i].toObject();

        // Se obtiene el nombre de dicho objeto y se c
        tmp->setMaximumHeight(300);
        tmp->setMaximumWidth(200);

        img = auxJson["id"].toString();

        name = auxJson["name"].toString();
        for (auto &x: name)
            x = x.toLower();

        if (regex_match(img.toStdString(), category) && regex_match(name.toStdString(), search))
        {
            // Se asigna el nombre al widget
            tmp->changeName(name);

            // Se asigna el precio al widget
            floatTmp = auxJson["price"].toDouble();
            tmp->changePrice(floatTmp);

            // Se asigna la imagen al widget
            img = auxJson["id"].toString();
            img += ".jpg";
            tmp->changeImage(img);

            // Se guarda el puntero del widget en el vector
            m_products.push_back(tmp);
        }
    }
    if (m_products.size())
        on_filtroCB_currentIndexChanged(ui->filtroCB->currentIndex());
    else
    {
        QMessageBox::warning(this, "No matches", "The search didn't gave any results");
        ui->categoriaCB->setCurrentIndex(CATEGORIA_TODOS);
        showProducts(regex(".+"));
    }
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::addToGrid(Producto* producto)
{
    m_layout->addWidget(producto, m_layoutRow, m_layoutColumn);
    if (m_layoutColumn == MAX_ROWS - 1)
        ++m_layoutRow;
    ++m_layoutColumn;
    m_layoutColumn %= MAX_ROWS;
}

void MainWindow::removeLayoutW()
{
    for (size_t i = 0; i < m_products.size(); ++i)
        delete m_products[i];
    m_layoutRow = 0;
    m_layoutColumn = 0;
    m_products.clear();
}

void MainWindow::on_categoriaCB_currentIndexChanged(int index, regex search)
{
    this->setCursor(Qt::WaitCursor);
    m_layoutRow = 0;
    m_layoutColumn = 0;
    if (index == CATEGORIA_TODOS)
        showProducts(regex(".+"), search);
    else if (index == CATEGORIA_AB)
        showProducts(regex("AB\\w+"), search);
    else if (index == CATEGORIA_L)
        showProducts(regex("L\\w+"), search);
    else if (index == CATEGORIA_E)
        showProducts(regex("E\\w+"), search);
    else if (index == CATEGORIA_HC)
        showProducts(regex("HC\\w+"), search);
    else if (index == CATEGORIA_D)
        showProducts(regex("D\\w+"), search);
}

void MainWindow::on_filtroCB_currentIndexChanged(int index)
{
    m_layoutRow = 0;
    m_layoutColumn = 0;
    if (index == FILTRO_MENOR_PRECIO)
        sort(m_products.begin(), m_products.end(), [](Producto* prod1, Producto* prod2)
                                                    { return *prod1 < *prod2; });
    else if (index == FILTRO_MAYOR_PRECIO)
        sort(m_products.begin(), m_products.end(), [](Producto* prod1, Producto* prod2)
                                                    { return *prod1 > *prod2; });
    for (auto &x: m_products)
        addToGrid(x);
    ui->tmpW->setLayout(m_layout);
}

void MainWindow::on_buscarL_clicked()
{
    string expression = ".*";
    expression += ui->buscarLE->text().toStdString();
    expression += ".*";

    on_categoriaCB_currentIndexChanged(ui->categoriaCB->currentIndex(), regex(expression));
}
