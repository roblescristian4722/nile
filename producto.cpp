#include "producto.h"
#include "ui_producto.h"

Producto::Producto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Producto)
{
    ui->setupUi(this);
}

Producto::~Producto()
{
    delete ui;
}

void Producto::changeName(QString data)
{
    ui->nameL->setText(data);
    ui->nameL->setWordWrap(true);
    ui->nameL->setMaximumHeight(60);
}

void Producto::changePrice(QString data)
{
    QString tmp = "$";
    tmp += data;
    ui->priceL->setText(tmp);
    ui->priceL->setMaximumHeight(50);
}

void Producto::changeImage(QString data)
{
    QString tmp = "imgs/";
    tmp += data;
    QPixmap pix(tmp);
    pix.scaled(ui->imageL->width(), ui->imageL->height());
    ui->imageL->setPixmap(pix);
}
