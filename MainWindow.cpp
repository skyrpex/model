#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "model/JointModel.h"
#include "widgets/AnimsWidget.h"
#include "widgets/TimeWidget.h"
#include "widgets/graphics/GraphicsJoint.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(new JointModel(this))
{
    ui->setupUi(this);

    TimeWidget *tw = new TimeWidget(m_model);
    AnimsWidget *aw = new AnimsWidget(m_model->animModel());
    QGraphicsView *gv = new QGraphicsView(new QGraphicsScene);
    gv->scene()->addItem(new GraphicsJoint(m_model->root()));

    connect(aw, SIGNAL(currentAnimChanged(int)), tw, SLOT(setCurrentAnim(int)));

    QSplitter *hs = new QSplitter(this);
    hs->addWidget(aw);
    hs->addWidget(gv);
    QSplitter *vs = new QSplitter(Qt::Vertical, this);
    vs->addWidget(hs);
    vs->addWidget(tw);

    QWidget *w = new QWidget(this);
    setCentralWidget(w);
    QVBoxLayout *l = new QVBoxLayout(w);
    l->addWidget(vs);
}

MainWindow::~MainWindow()
{
    delete ui;
}
