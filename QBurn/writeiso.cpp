#include "writeiso.h"
#include "ui_writeiso.h"

WriteISO::WriteISO(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteISO)
{
    burn.initialize();
    ui->setupUi(this);
}

WriteISO::~WriteISO()
{
    delete ui;
}

void WriteISO::on_choose_clicked()
{
    path_to_file = QFileDialog::getOpenFileName(this, tr("Выбрать ISO"), "/home", tr("ISO 9660 Files (*.iso)"));
    ui->iso_name->setText(path_to_file);

    if(!path_to_file.isEmpty())
        ui->burn->setEnabled(true);
    else
        ui->burn->setEnabled(false);
}
