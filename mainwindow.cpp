#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Nile");
    this->setMinimumSize(WINDOW_MIN_W, WINDOW_MIN_H);
    ui->productosSA->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->productosSA->setWidgetResizable(true);

    m_layoutRow = 0;
    m_layoutColumn = 0;
    m_recomSize = 0;
    m_layout = new QGridLayout(this);
    m_layoutRecom = new QGridLayout(this);
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
    removeLayoutW();
    remove_layout_recom();
    delete ui;
}

void MainWindow::enableLoginPB()
{
    if (ui->emailLE->text().length() && ui->passwordLE->text().length())
        ui->loginPB->setEnabled(true);
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
    size_t i;
    regex regEmail("[a-zA-Z0-9-_.]+@[a-zA-Z0-9]+.+[a-zA-Z]");
    regex regUser("[A-Za-z0-9-_.]*");
    User userTmp;
    userTmp.setEmail(ui->newEmailLE->text());
    userTmp.setPassword(ui->newPasswordLE->text());
    userTmp.setUsername(ui->newUsernameLE->text());

    // Se valida que tanto el email como el username estén
    // bien escritos conforme a las expresiones regulares
    if (regex_match(userTmp.getEmail().toStdString(), regEmail)){
        if (regex_match(userTmp.getUsername().toStdString(), regUser)){

            // Se comparan los datos con los de todos los usuarios
            // en el vector
            for (i = 0; i < m_users.size(); i++){
                if (userTmp.getEmail() == m_users.at(i).getEmail()){
                    QMessageBox::warning(this, "invalid email",
                                         "email is already in use");
                    break;
                }
                else if (userTmp.getUsername() == m_users.at(i).getUsername()){
                    QMessageBox::warning(this, "invalid username",
                                         "username is already in use");
                    break;
                }
            }
            if (i == m_users.size()){
                m_users.push_back(userTmp);
                jsonObj["name"] = userTmp.getUsername();
                jsonObj["email"] = userTmp.getEmail();
                jsonObj["password"] = userTmp.getPassword();
                jsonObj["purchase"];
                m_userDb.append(jsonObj);
                QMessageBox::about(this, "User created", "User created succesfully");
            }
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

    ui->newEmailLE->clear();
    ui->newPasswordLE->clear();
    ui->newUsernameLE->clear();
}

void MainWindow::on_loginPB_clicked()
{
    bool success = false;
    size_t i;
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString dateTime = date.toString() + " " + time.toString();
    QJsonObject userTmp;

    // Se comparan los usuarios de la base de datos con el recién ingresado
    for (i = 0; i < m_users.size(); i++){
        if (m_users[i].getEmail() == ui->emailLE->text()){
            if (m_users[i].getPassword() == ui->passwordLE->text()){
                // Si hay una coincidencia se busca ese mismo usuario en QJsonArray de
                // usuarios para añadir la nueva sesión
                m_dateSession = dateTime;
                m_currentUser = ui->emailLE->text();
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

    if (success){
        m_users.clear();
        on_categoriaCB_currentIndexChanged(CATEGORIA_TODOS);
        random_elements();
    }
}

void MainWindow::saveDB()
{
    QJsonDocument jsonDoc;
    QJsonObject jsonDocToObj;
    QJsonObject jsonObj;
    QJsonObject purchaseObj;
    QJsonObject aux;
    QJsonObject timeDateObj;
    QJsonArray purchaseArr;
    QJsonArray timeDateArr;
    map<QString, int>::iterator it;
    int sold = 0;

    if (m_shoppingCart.size()){
        // Se itera por la base de datos del usuario hasta encontrar el usuario
        // de la sesión actual
        for (int i = 0; i < m_userDb.size(); ++i)
            if (m_userDb[i].toObject()["email"].toString() == m_currentUser){
                // Objeto usuario
                jsonObj = m_userDb[i].toObject();
                // Array purchase - Se guarda para no perder los datos
                // previamente almacenados
                purchaseArr = jsonObj["purchase"].toArray();
                for (it = m_shoppingCart.begin(); it != m_shoppingCart.end(); it++){
                    aux["id"] = it->first;
                    timeDateArr.append(aux);
                }
                // Se guarda el array timeDate dentro de un objeto que tiene
                // la fecha y hora de la sesión como llave
                timeDateObj[m_dateSession] = timeDateArr;
                // Luego se almacena el objeto completo en el array de compras
                purchaseArr.append(timeDateObj);
                // Después se sustituyen los datos en purchase por la versión
                // actualizada del array
                jsonObj["purchase"] = purchaseArr;
                // Y finalmente se cambia todo purchase del objeto usuario
                m_userDb[i] = jsonObj;
            }

        for (int i = 0; i < m_productDb.size(); ++i){
            jsonObj = m_productDb[i].toObject();
            // Se busca el producto de la base de datos en el carrito de compras
            it = m_shoppingCart.find(jsonObj["id"].toString());
            // Si el producto fue vendido esta sesión se cambia su valor
            if (it != m_shoppingCart.end()){
                sold = jsonObj["sold"].toInt();
                sold += it->second;
                jsonObj["sold"] = sold;
                m_productDb[i] = jsonObj;
            }
        }
    }

    jsonDocToObj["users"] = m_userDb;
    jsonDocToObj["products"] = m_productDb;
    jsonDoc = QJsonDocument(jsonDocToObj);

    m_dbFile.open(QIODevice::WriteOnly);
    m_dbFile.write(jsonDoc.toJson());

    m_dbFile.close();
}

void MainWindow::loadDB()
{
    QJsonDocument jsonDoc;
    QByteArray data;
    User userAux;
    QJsonObject jsonObj;
    QJsonObject jsonAux;
    QJsonArray purchases;
    QJsonArray nestedArray;
    QJsonObject nestedItem;
    QString item, item2;
    int cost = 0;

    m_dbFile.open(QIODevice::ReadOnly);
    data = m_dbFile.readAll();
    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    m_userDb = jsonObj["users"].toArray();
    m_productDb = jsonObj["products"].toArray();
    for (int i = 0; i < m_userDb.size(); i++){
        jsonAux = m_userDb[i].toObject();
        userAux.setEmail(jsonAux["email"].toString());
        userAux.setUsername(jsonAux["name"].toString());
        userAux.setPassword(jsonAux["password"].toString());
        m_users.push_back(userAux);
    }

    // Se guardan los datos en el grafo y se calculan sus pesos
    //
    // Itera por los usuarios disponibles
    for (int i = 0; i < m_userDb.size(); ++i){
        // usuario
        jsonObj =  m_userDb[i].toObject();
        // todo el objeto de compras se almacena en un array
        // para iterar sobre este
        purchases = jsonObj["purchase"].toArray();
        // Se itera por dicho array y se guarda cada sesión
        // en otro array
        for (int j = 0; j < purchases.size(); ++j){
            jsonAux = purchases[j].toObject();
            nestedArray = jsonAux.begin()->toArray();
            // Itera por cada identificador
            for (int k = 0; k < nestedArray.size() - 1; ++k)
                for (int l = k + 1; l < nestedArray.size(); ++l){
                    nestedItem = nestedArray[k].toObject();
                    item = nestedItem["id"].toString();
                    nestedItem = nestedArray[l].toObject();
                    item2 = nestedItem["id"].toString();
                    if (m_graph.isEdge(item.toStdString(), item2.toStdString()))
                        cost = m_graph.getCost(item.toStdString(), item2.toStdString());
                    ++cost;
                    m_graph.createEdge(item.toStdString(), item2.toStdString(), cost);
                    cost = 0;
            }
        }
    }
    m_graph.printData();
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
    double floatTmp;

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
        tmp->setMinimumSize(WIDGET_MIN_W, WIDGET_MIN_H);
        tmp->setMaximumSize(WIDGET_MAX_W, WIDGET_MAX_H);

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
            tmp->changeImage(img);

            // Se guarda el puntero del widget en el vector
            m_products.push_back(tmp);
            connect (m_products.back(), &Producto::add_to_purchase, this, &MainWindow::added);
        }
    }
    if (m_products.size())
        on_filtroCB_currentIndexChanged(ui->filtroCB->currentIndex());
    else
    {
        QMessageBox::warning(this, "No matches found", "The search didn't give any result");
        ui->categoriaCB->setCurrentIndex(CATEGORIA_TODOS);
        showProducts(regex(".+"));
    }
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::addToGrid(Producto* producto)
{
    int rows;
    m_layout->addWidget(producto, m_layoutRow, m_layoutColumn);
    if (this->height() > WINDOW_MIN_H && this->width() > WINDOW_MIN_W)
        rows = MAX_ROWS_MAX_SIZE;
    else
        rows = MAX_ROWS_MIN_SIZE;

    if (m_layoutColumn == rows - 1)
        ++m_layoutRow;
    ++m_layoutColumn;
    m_layoutColumn %= rows;

}

void MainWindow::removeLayoutW()
{
    for (size_t i = 0; i < m_products.size(); ++i){
        m_layout->removeWidget(m_products[i]);
        delete m_products[i];
    }
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

void MainWindow::added(QString id, int total, bool recomm)
{
    QJsonObject userTmp;
    map<string, int> mapTmp;
    int cont = TOTAL_RECOM;

    // Se añade el dato al carrito
    if (m_shoppingCart.find(id) == m_shoppingCart.end())
        m_shoppingCart[id] = total;
    // Si ya existe el dato entonces solo se aumenta la cantidad de productos
    else
        m_shoppingCart[id] += total;

    // Si el objeto en el carrito se encuentra dentro del grafo de recomendaciones
    // entonces obtenemos sus vecinos y luego guardamos máximo 5 de esos vecinos en
    // una cola de prioridad que ordena los datos de mayor a menor (conforme a lo fuerte que sea
    // la relación entre el dato comprado y el vecino).
    if (m_graph.contains(id.toStdString())){
        mapTmp = m_graph.getNeighbors(id.toStdString());
        for (map<string, int>::iterator it = mapTmp.begin(); it != mapTmp.end() && cont; ++it){
            // HashMap para evitar repeticiones en la cola de prioridad
            if (m_recomAdded.find(it->first) == m_recomAdded.end()){
                m_recomQueue.push(*it);
                // Se indica en el map que el producto se ha recomendado
                // pero no se ha comprado
                m_recomAdded[it->first] = false;
            }
            --cont;
        }
        update_recommendations(id, recomm);
    }
}

void MainWindow::remove_layout_recom()
{
    for (size_t i = 0; i < m_recom.size(); ++i){
        m_layoutRecom->removeWidget(m_recom[i]);
        delete m_recom[i];
    }
    m_recom.clear();
}

void MainWindow::update_recommendations(QString id, bool recomm)
{
    Producto* prod;
    QString name;
    QString img;
    size_t pos;
    double price;

    if (m_recomSize == TOTAL_RECOM){
        remove_layout_recom();
        m_recomSize = 0;
    }

    cout << m_recomQueue.size() << endl;
    if (recomm){
        for (pos = 0; pos < m_recom.size(); ++pos)
            if (id == m_recom[pos]->getId())
                break;
        if (pos < m_recom.size()){
            m_layoutRecom->removeWidget(m_recom[pos]);
            delete m_recom[pos];
            m_recom.erase(m_recom.begin() + pos);
            m_posTmp.push(int(pos));
            --m_recomSize;
        }
    }
    // Se desencolan los datos y se crean los widgets
    while(m_recomQueue.size() && m_recomSize < TOTAL_RECOM){
        img = QString::fromUtf8(m_recomQueue.top().first.c_str());
        for (int i = 0; i < m_productDb.size(); ++i){
            if (img == m_productDb[i].toObject()["id"].toString()){
                prod = new Producto(true);
                name = m_productDb[i].toObject()["name"].toString();
                price = m_productDb[i].toObject()["price"].toDouble();
                prod->changeName(name);
                prod->changeImage(img);
                prod->changePrice(price);
                m_recom.push_back(prod);
                connect (m_recom.back(), &Producto::add_to_purchase, this, &MainWindow::added);
                if (m_posTmp.size()){
                    m_layoutRecom->addWidget(prod, 0, m_posTmp.back());
                    m_posTmp.pop();
                }
                else
                    m_layoutRecom->addWidget(prod, 0, m_recomSize);
                break;
            }
        }
        // Si es la primera vez que se compra el producto se aumenta el contador
        // de las recomendaciones, esto para evitar que se haya cambios en las
        // recomendaciones en caso de que se vuelva a comprar un elemento
        if (m_recomAdded.find(id.toStdString()) == m_recomAdded.end())
            m_recomAdded[id.toStdString()] = true;
        ++m_recomSize;
        m_recomQueue.pop();
    }
    ui->tmpRecomW->setLayout(m_layoutRecom);
    prod = nullptr;
}

void MainWindow::random_elements()
{
    int pos;
    QJsonObject tmp;
    map<int, bool> positions;
    PAIR par;

    srand(time(NULL));
    for (int i = 0; i < TOTAL_RECOM; ++i){
        do{
            pos = rand() % m_productDb.size();
        }while(positions.find(pos) != positions.end());
        positions[pos] = true;
        tmp = m_productDb[pos].toObject();
        par.first = tmp["id"].toString().toStdString();
        par.second = pos;
        m_recomQueue.push(par);
    }
    update_recommendations("", false);
}
