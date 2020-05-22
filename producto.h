#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <iostream>
#include <QWidget>
#include <QDebug>
#include <QString>
using namespace std;

namespace Ui
{ class Producto; }

class Producto : public QWidget
{
    Q_OBJECT

public:
    explicit Producto(bool recomm = false, QWidget *parent = nullptr);
    ~Producto();

    void changeName(QString data);
    void changePrice(double data);
    void changeImage(QString data);

    double getPrice() const;
    QString getName();
    QString getId() const;

    bool operator < (const Producto& other);
    bool operator > (const Producto& other);

private slots:
    void on_addPB_clicked();
    void on_numberProductsSB_valueChanged(int arg1);

signals:
    void add_to_purchase(QString id, int total, bool recomm);

private:
    Ui::Producto *ui;
    QString m_id;
    double m_price;
    bool m_recomm;

};

#endif // PRODUCTO_H
