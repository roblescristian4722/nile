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
    void changePrice(float data);
    void changeImage(QString data);

    float getPrice() const;
    QString getName();

    bool operator < (const Producto& other);
    bool operator > (const Producto& other);

private:
    Ui::Producto *ui;
    float m_price;
};

#endif // PRODUCTO_H
