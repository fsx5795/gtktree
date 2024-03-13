#include "startwidget.h"
static void on_pressed(GtkButton *button, gpointer data)
{
    const gchar *label = gtk_button_get_label(button);
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(data));
    static pid_t pid;
    if (strcmp(label, "启动显示窗口") == 0)
    {
        if (pid > 0 && kill(pid, 0) == 0)
        {
            GtkWidget *msgDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "该显示窗口进程已存在");
            gtk_widget_show(msgDialog);
            gtk_window_destroy(GTK_WINDOW(msgDialog));
            return;
        }
        pid = fork();
        if (pid < 0)
        {
            GtkWidget *msgDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "显示窗口进程创建失败:%s", strerror(errno));
            gtk_widget_show(msgDialog);
            gtk_window_destroy(GTK_WINDOW(msgDialog));
            return;
        }
        else if (pid == 0)
        {
            char buf[BUFSIZ] = { '\0' };
            ssize_t n = readlink("/proc/self/exe", buf, sizeof buf);
            if (n > 0 && n < BUFSIZ)
            {
                strcpy(strrchr(buf, '/'), "\0");
                strcat(buf, "/show");
                puts(buf);
                execl(buf, "show", NULL);
            }
        }
        return;
    }
    union sigval sv;
    sv.sival_int = atoi(text);
    if (sigqueue(pid, strcmp(label, "创建节点") == 0 ? SIGUSR1 : SIGUSR2, sv) == -1)
    {
        GtkWidget *msgDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "节点操作失败:%s", strerror(errno));
        gtk_widget_show(msgDialog);
        gtk_window_destroy(GTK_WINDOW(msgDialog));
    }
    return;
}
void on_activate(GtkApplication *app, gpointer data)
{
    signal(SIGCHLD, SIG_IGN);
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "控制台");
    GtkWidget *entry = gtk_entry_new();
    GtkWidget *createBtn = gtk_button_new_with_label("创建节点");
    GtkWidget *deleteBtn = gtk_button_new_with_label("删除节点");
    GtkWidget *showBtn = gtk_button_new_with_label("启动显示窗口");
    GtkWidget *entryBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(entryBox), entry);
    gtk_box_append(GTK_BOX(hbox), createBtn);
    gtk_box_append(GTK_BOX(hbox), deleteBtn);
    gtk_box_append(GTK_BOX(hbox), showBtn);
    gtk_box_append(GTK_BOX(vbox), entryBox);
    gtk_box_append(GTK_BOX(vbox), hbox);
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    g_signal_connect(createBtn, "clicked", G_CALLBACK(on_pressed), entry);
    g_signal_connect(deleteBtn, "clicked", G_CALLBACK(on_pressed), entry);
    g_signal_connect(showBtn, "clicked", G_CALLBACK(on_pressed), entry);
    gtk_widget_show(window);
    return;
}