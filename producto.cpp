#include "producto.h"
#include "ui_producto.h"

Producto::Producto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Producto)
{
    ui->setupUi(this);
    ui->addPB->setEnabled(false);
}

Producto::~Producto()
{ delete ui; }

void Producto::changeName(QString data)
{
    ui->nameL->setText(data);
    ui->nameL->setWordWrap(true);
}

void Producto::changePrice(double data)
{
    m_price = data;
    QString tmp = "$";
    tmp += QString::number(data);
    ui->priceL->setText(tmp);
    ui->priceL->setMaximumHeight(50);
}

void Producto::changeImage(QString data)
{
    m_id = data;
    QString tmp = "imgs/";
    tmp += data + ".jpg";
    QPixmap pix(tmp);

    ui->imageL->setPixmap(pix.scaled(this->width(), this->height(), Qt::KeepAspectRatio));
    ui->imageL->setAlignment(Qt::AlignCenter);
}

double Producto::getPrice() const
{ return m_price; }

QString Producto::getName()
{ return ui->nameL->text(); }

bool Producto::operator < (const Producto &other)
{ return this->getPrice() < other.getPrice(); }

bool Producto::operator >(const Producto &other)
{ return this->getPrice() > other.getPrice(); }

void Producto::on_addPB_clicked()
{
    emit add_to_purchase(m_id, ui->numberProductsSB->text().toInt());
    ui->numberProductsSB->setValue(0);
}

void Producto::on_numberProductsSB_valueChanged(int arg1)
{
    if (arg1)
        ui->addPB->setEnabled(true);
    else
        ui->addPB->setEnabled(false);
}
