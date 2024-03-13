#include <math.h>
#include "showwidget.h"
struct Points
{
    int x;
    int y;
};
struct Tree *tree = NULL;
static GtkWidget *area = NULL;
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
static void sigaction_callback(int signum, siginfo_t *s_t, void *p)
{
    switch (signum)
    {
    case SIGUSR1:
        insert(&tree, s_t->si_value.sival_int);
        break;
    case SIGUSR2:
        delete(&tree, s_t->si_value.sival_int);
        break;
    default:
        return;
    }
    gtk_widget_queue_draw(area);
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
    struct sigaction sc;
    sc.sa_sigaction = sigaction_callback;
    sc.sa_flags = SA_SIGINFO;
    sigemptyset(&sc.sa_mask);
    sigaction(SIGUSR1, &sc, NULL);
    sigaction(SIGUSR2, &sc, NULL);
    return;
}