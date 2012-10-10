#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QWidget>

class JointModel;
class Anim;
class QxGraphicsView;
class GraphicsJoint;

class GraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicsWidget(JointModel *model, QWidget *parent = 0);
    
signals:
    
public slots:
    void setCurrentAnim(int i);
    void setCurrentFrame(int frame);
    
private:
    JointModel *m_model;
    QxGraphicsView *m_view;
    GraphicsJoint *m_root;
    Anim *m_currentAnim;
    int m_currentFrame;
};

#endif // GRAPHICSWIDGET_H
