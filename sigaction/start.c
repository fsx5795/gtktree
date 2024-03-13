#include <gtk/gtk.h>
#include "startwidget.h"
int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("nook.inc", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int app_status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return app_status;
}