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

void Producto::changePrice(float data)
{
    m_price = data;
    QString tmp = "$";
    tmp += QString::number(data);
    ui->priceL->setText(tmp);
    ui->priceL->setMaximumHeight(50);
}

void Producto::changeImage(QString data)
{
    QString tmp = "imgs/";
    tmp += data;
    QPixmap pix(tmp);

    ui->imageL->setPixmap(pix.scaled(this->width(), this->height(), Qt::KeepAspectRatio));
    ui->imageL->setAlignment(Qt::AlignCenter);
}

float Producto::getPrice() const
{
    return m_price;
}

QString Producto::getName()
{
    return ui->nameL->text();
}

bool Producto::operator < (const Producto &other)
{
    return this->getPrice() < other.getPrice();
}

bool Producto::operator >(const Producto &other)
{
    return this->getPrice() > other.getPrice();
}
