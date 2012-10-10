#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int argc, char **argv);
};

#endif // APPLICATION_H
