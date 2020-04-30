#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAX_ROWS_MAX_SIZE 3
#define MAX_ROWS_MIN_SIZE 2
#define WINDOW_MAX_W 500
#define WINDOW_MAX_H 400
#define WINDOW_MIN_W 700
#define WINDOW_MIN_H 600
#define WIDGET_MIN_H 200
#define WIDGET_MIN_W 300
#define WIDGET_MAX_H 400
#define WIDGET_MAX_W 500

#include <iostream>
#include "user.h"
#include "producto.h"
#include "graph.h"
#include <vector>
#include <regex>
#include <map>
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QDateTime>

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
    void on_newUsernameLE_textChanged(const QString &arg1);
    void on_newEmailLE_textChanged(const QString &arg1);
    void on_newPasswordLE_textChanged(const QString &arg1);
    void on_categoriaCB_currentIndexChanged(int index, regex search = regex(".+"));
    void on_filtroCB_currentIndexChanged(int index);
    void on_emailLE_textChanged(const QString &arg1);
    void on_createPB_clicked();
    void on_loginPB_clicked();
    void on_buscarL_clicked();

    void openFile();

    void enableLoginPB();
    void enableCreatePB();

    void saveDB();
    void loadDB();

    void showProducts(regex category, regex search = regex(".+"));
    void addToGrid(Producto* producto);
    void removeLayoutW();
    void added(QString id, int total);

private:
    Ui::MainWindow *ui;

    // Objetos para abrir correctamente el
    // archivo de datos
    QAction* m_openFileAction;
    QMenu* m_fileMenu;
    QFile m_dbFile;

    // Data Bases
    QJsonArray m_userDb;
    QJsonArray m_productDb;
    vector<User> m_users;

    // Para mostrar los productos de manera
    // correcta
    vector<Producto*> m_products;
    QGridLayout *m_layout;

    QString m_dateSession;
    QString m_currentUser;
    map<QString, int> m_shoppingCart;

    // Grafo que se utilizará a futuro para
    // las recomendaciones de productos
    Graph<string> m_graph;

    int m_layoutRow;
    int m_layoutColumn;

    enum indiceCategoria
    {
        CATEGORIA_TODOS = 0,
        CATEGORIA_AB,
        CATEGORIA_L,
        CATEGORIA_E,
        CATEGORIA_HC,
        CATEGORIA_D
    };

    enum indiceFiltro
    {
        FILTRO_MENOR_PRECIO = 0,
        FILTRO_MAYOR_PRECIO,
    };
};
#endif // MAINWINDOW_H
