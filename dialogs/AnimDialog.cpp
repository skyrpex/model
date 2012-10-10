#include "AnimDialog.h"
#include "ui_AnimDialog.h"
#include <QPushButton>

AnimDialog::AnimDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimDialog)
{
    ui->setupUi(this);
    connect(ui->name, SIGNAL(textChanged(QString)), SLOT(onNameChanged(QString)));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    setWindowTitle(tr("Create Animation"));
}

AnimDialog::AnimDialog(const QString &name, int frameCount, int fps, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimDialog)
{
    ui->setupUi(this);
    connect(ui->name, SIGNAL(textChanged(QString)), SLOT(onNameChanged(QString)));
    ui->name->setText(name);
    ui->frameCount->setValue(frameCount);
    ui->fps->setValue(fps);

    setWindowTitle(tr("Edit Animation"));
}

AnimDialog::~AnimDialog()
{
    delete ui;
}

QString AnimDialog::name() const
{
    return ui->name->text();
}

int AnimDialog::frameCount() const
{
    return ui->frameCount->value();
}

int AnimDialog::fps() const
{
    return ui->fps->value();
}

void AnimDialog::onNameChanged(const QString &text)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(text.trimmed().isEmpty());
}
