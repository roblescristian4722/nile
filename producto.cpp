#include "producto.h"
#include "ui_producto.h"

Producto::Producto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Producto)
{
    ui->setupUi(this);
    ui->imageL->setMinimumHeight(150);
}

Producto::~Producto()
{
    delete ui;
}

void Producto::changeName(QString data)
{
    ui->nameL->setText(data);
}

void Producto::changePrice(QString data)
{
    ui->priceL->setText(data);
}

void Producto::changeImage(QString data)
{
    QString tmp = "imgs/";
    tmp += data;
    QPixmap pix(tmp);
    ui->imageL->setPixmap(pix);
}
