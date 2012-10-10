#ifndef JOINTHEADERVIEW_H
#define JOINTHEADERVIEW_H

#include <QHeaderView>

class JointHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit JointHeaderView(bool showTimeLine, QWidget *parent = 0);

    int currentFrame() const;

    QSize sizeHint() const;

public slots:
    void setCurrentFrame(int frame);

signals:
    void currentFrameChanged(int frame);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void updateCurrentFrame(const QPoint &pos);

    int m_currentFrame;
    bool m_showTimeLine;
};

#endif // JOINTHEADERVIEW_H
