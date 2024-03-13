#include <sys/fcntl.h>
#include <math.h>
#include "showwidget.h"
struct Points
{
    int x;
    int y;
};
static int fd;
struct Tree *tree = NULL;
static GtkWidget *area = NULL;
static void sighander(int s)
{
    switch (s)
    {
    case SIGUSR1:
    case SIGUSR2:
        if (fd != 0)
        {
            char buf[1024] = { '\0' };
            if (read(fd, buf, sizeof(buf) - 1) > 0)
            {
                if (s == SIGUSR1 ? insert(&tree, strtol(buf, NULL, 10)) : delete(&tree, strtol(buf, NULL, 10)))
                    gtk_widget_queue_draw(area);
            }
        }
        break;
    default:
        break;
    }
    return;
}
static void drawNode(struct Tree *node, cairo_t *cr, struct Points *prevPoints, int left, int right, int y, int index)
{
    if (!node || node->num == 0)
        return;
    int curX = left + (right - left) / 2;
    struct Points points = { curX, index * 25 };
    drawNode(node->left, cr, &points, left, curX, y, index + 2);
    drawNode(node->right, cr, &points, curX, right, y, index + 2);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_arc(cr, curX, index * 25, 20, 0, 2 * M_PI);
    if (prevPoints)
    {
        cairo_move_to(cr, prevPoints->x, prevPoints->y);
        cairo_line_to(cr, points.x, points.y);
    }
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    cairo_move_to(cr, curX - 15 / 2, index * 25);
    char text[5] = { '\0' };
    sprintf(text, "%d", node->num);
    cairo_set_font_size(cr, 15);
    cairo_text_path(cr, text);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
    return;
}
static void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
{
    int index = 1;
    drawNode(tree, cr, NULL, 0, width, height, index);
    return;
}
void on_activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "二叉树显示窗口");
    area = gtk_drawing_area_new();
    gtk_window_set_child(GTK_WINDOW(window), area);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), on_draw, NULL, NULL);
    gtk_widget_show(window);
    fd = open("tmpName", O_RDONLY);
    if (fd == -1)
    {
        GtkWidget *msgDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "通信管道打开失败:%s", strerror(errno));
        gtk_widget_show(msgDialog);
        gtk_window_destroy(GTK_WINDOW(msgDialog));
    }
    signal(SIGUSR1, sighander);
    signal(SIGUSR2, sighander);
    return;
}