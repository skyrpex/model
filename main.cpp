#include <QApplication>
#include "model/JointModel.h"
#include "model/AnimModel.h"
#include "model/Joint.h"
#include "model/Anim.h"
#include "widgets/time/JointProxyModel.h"
#include "widgets/time/KeyFramesEditor.h"
#include "widgets/TimeWidget.h"
#include "widgets/AnimsWidget.h"
#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QListView>
#include <QTableView>
#include <QSplitter>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "widgets/graphics/GraphicsJoint.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    KeyFrames k;
//    KeyFramesEditor e;
//    e.setData(k.data, 20, 4);
//    e.show();

    JointModel m;

    TimeWidget *w = new TimeWidget(&m);
    w->show();

    AnimsWidget *aw = new AnimsWidget(m.animModel());
    aw->show();

    aw->connect(aw, SIGNAL(currentAnimChanged(int)), w, SLOT(setCurrentAnim(int)));

    new Joint("Rack", m.root());
    m.animModel()->addAnim(new Anim("Ist", 10, 10));
    Joint *j = new Joint("Neck", m.root());
    Anim *jah = new Anim("Jah", 10, 10);
    m.animModel()->addAnim(jah);
    new Joint("Head", j);
    m.animModel()->addAnim(new Anim("Ber", 10, 10));

    jah->setFrameCount(100);

    QGraphicsView *gv = new QGraphicsView(new QGraphicsScene);
    gv->scene()->addItem(new GraphicsJoint(m.root()));
    gv->show();

    return a.exec();
}
