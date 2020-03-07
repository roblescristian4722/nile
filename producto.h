#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <iostream>
using namespace std;

namespace Ui {
class Producto;
}

class Producto : public QWidget
{
    Q_OBJECT

public:
    explicit Producto(QWidget *parent = nullptr);
    ~Producto();

    void changeName(QString data);
    void changePrice(QString data);
    void changeImage(QString data);

private:
    Ui::Producto *ui;
};

#endif // PRODUCTO_H
