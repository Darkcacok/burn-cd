#include "writeiso.h"
#include "ui_writeiso.h"

WriteISO::WriteISO(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteISO)
{
    burn.initialize();
    vector<string> *adr = burn.scan();
    ui->setupUi(this);

    for(int i =0; i < adr->size(); ++i)
        ui->choose_disc->addItem(QString::fromStdString((*adr)[i]));
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

void WriteISO::on_burn_clicked()
{
    burn.write_iso_image(ui->choose_disc->currentIndex(), path_to_file.toUtf8().data(), ui->progress_burn);
}
