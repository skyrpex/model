#include "Application.h"

Application::Application(int argc, char **argv) :
    QApplication(argc, argv)
{
    setOrganizationName("VGAFIB");
    setApplicationName("Flatty");
    setApplicationVersion("0.0");
}
