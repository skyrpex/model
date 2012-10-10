#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QWidget>

class JointModel;
class QxGraphicsView;

class GraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicsWidget(JointModel *model, QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QxGraphicsView *m_view;
};

#endif // GRAPHICSWIDGET_H
