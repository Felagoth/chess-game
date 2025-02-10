
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "types.h"
#include "gtkgame.h"
#include "chess_logic.h"

static board_state *board_s;
static int mode;
static char color;
static bool is_selected = false;
static coords init_co;
// create the chess board
GtkWidget *board_widgets[8][8];

void on_activate(GtkApplication *app)
{
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *start_button = gtk_button_new_with_label("Click to start");
    gtk_window_set_title(GTK_WINDOW(window), "Chess");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    gtk_window_set_child(GTK_WINDOW(window), start_button);
    g_signal_connect(start_button, "clicked", G_CALLBACK(menugtk), window);
    gtk_widget_show(window);
}

void menugtk(GtkApplication *app, GtkWidget *window)
{
    GtkWidget *pvp_button = gtk_button_new_with_label("play pvp");
    GtkWidget *pva_button = gtk_button_new_with_label("play pva");
    GtkWidget *semi_free_button = gtk_button_new_with_label("play semi-free");
    GtkWidget *free_mode_button = gtk_button_new_with_label("play free mode");
    GtkWidget *settings_button = gtk_button_new_with_label("settings");
    // Create a new grid
    GtkWidget *grid = gtk_grid_new();
    // Set the grid spacing
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    // Add the grid to the window
    gtk_window_set_child(GTK_WINDOW(window), grid);
    // Attach the buttons to the grid
    gtk_grid_attach(GTK_GRID(grid), pvp_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), pva_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), semi_free_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), free_mode_button, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), settings_button, 0, 4, 1, 1);
    // center the menu
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    // Connect the buttons to the callback functions
    g_signal_connect(pvp_button, "clicked", G_CALLBACK(play1), window);
    g_signal_connect(pva_button, "clicked", G_CALLBACK(pva), NULL);
    g_signal_connect(semi_free_button, "clicked", G_CALLBACK(semi_free), NULL);
    g_signal_connect(free_mode_button, "clicked", G_CALLBACK(free_mode), NULL);
    g_signal_connect(settings_button, "clicked", G_CALLBACK(settings), NULL);
    // Show the window
    gtk_widget_show(window);
}

char *get_label_piece(piece p)
{
    char *label_piece = malloc(2 * sizeof(char));
    label_piece[0] = p.name;
    if (p.color == 'b')
    {
        label_piece[0] = label_piece[0] + 32;
    }
    label_piece[1] = '\0';
    return label_piece;
}

void draw_board()
{
    piece selected_piece = get_piece(board_s->board, init_co);
    char *label_piece;
    coords dest_co;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            dest_co.x = i;
            dest_co.y = j;
            label_piece = get_label_piece(board_s->board[i][j]);
            gtk_label_set_text(GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(board_widgets[7 - i][j]))), label_piece);
            if (can_move_heuristic(board_s, selected_piece, init_co, dest_co, true))
            {
                // set widget color to yellow
                gtk_widget_set_name(board_widgets[7 - i][j], "yellow");
            }
            else if (init_co.x == i && init_co.y == j && is_selected && !is_empty_coords(init_co))
            {
                // set widget color to blue
                gtk_widget_set_name(board_widgets[7 - i][j], "blue");
            }
            else
            {
                // set widget color to the original color
                if ((i + j) % 2 == 0)
                {
                    gtk_widget_set_name(board_widgets[7 - i][j], "lightbrown");
                }
                else
                {
                    gtk_widget_set_name(board_widgets[7 - i][j], "darkbrown");
                }
            }
        }
    }
}

// on_square_clicked function that returns the column and line of the clicked square
void on_square_clicked(GtkGestureClick *gesture, GtkButton *event, GtkWidget *eventbox)
{
    int left, top, width, height;
    // GtkWidget *label = gtk_widget_get_first_child(eventbox);
    GtkWidget *grid = gtk_widget_get_parent(eventbox);
    gtk_grid_query_child(GTK_GRID(grid), eventbox,
                         &left, &top, &width, &height);
    int row, column;
    row = top;
    column = left - 1;
    printf("clicked on square %c%d\n", 'a' + column, 8 - row);
    coords co;
    co.x = 7 - row;
    co.y = column;
    printf("cos: %d, %d\n", co.x, co.y);
    if (is_selected)
    {
        if (can_move(board_s, board_s->board[init_co.x][init_co.y], init_co, co, true))
        {
            board_s = move_piece(board_s, init_co, co);
            is_selected = false;
            if (mode < 3)
            {
                color = (color == 'w') ? 'b' : 'w';
            }
        }
        else if (!is_empty(board_s->board[co.x][co.y]) && (board_s->board[co.x][co.y].color == color || mode > 3))
        {
            init_co = co;
            is_selected = true;
        }
        else
        {
            is_selected = false;
            init_co = empty_coords();
        }
    }
    else
    {
        if (!is_empty(board_s->board[co.x][co.y]) && (board_s->board[co.x][co.y].color == color || mode > 3))
        {
            init_co = co;
            is_selected = true;
        }
    }
    draw_board();
    if (!is_selected)
    {
        init_co = empty_coords();
    }
}

void init_chess_window(GtkApplication *app, GtkWidget *window)
{
    GtkWidget *eventbox, *menu_button;
    GtkWidget *label;
    GtkGesture *gesture;
    // Set the window default size
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    // Create the main grid
    GtkWidget *main_grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), main_grid); // Add the main grid to the window
    gtk_grid_set_row_spacing(GTK_GRID(main_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(main_grid), 10);
    // Create the chess board grid
    GtkWidget *chess_grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(main_grid), chess_grid, 0, 0, 1, 1); // Attach the chess grid to the main grid
    // Create the menu grid
    GtkWidget *menu_grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(main_grid), menu_grid, 1, 0, 1, 1); // Attach the menu grid to the main grid
    gtk_grid_set_row_spacing(GTK_GRID(menu_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(menu_grid), 10);

    // Create a CSS provider and load the CSS data
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "style/style.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(css_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    for (int i = 0; i < 8; i++)
    {
        eventbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        char label_str[2] = {8 - i + '0', '\0'};
        label = gtk_label_new(label_str);
        gtk_widget_add_css_class(eventbox, "square");
        gtk_grid_attach(GTK_GRID(chess_grid), eventbox, 0, i, 1, 1);
        gtk_box_append(GTK_BOX(eventbox), label);
        gtk_widget_set_size_request(label, 30, 30);
        for (int j = 0; j < 8; j++)
        {
            gesture = gtk_gesture_click_new();
            eventbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
            board_widgets[i][j] = eventbox;
            gtk_grid_attach(GTK_GRID(chess_grid), board_widgets[i][j], j + 1, i, 1, 1);
            gtk_widget_add_controller(eventbox, GTK_EVENT_CONTROLLER(gesture));
            label = gtk_label_new(" ");
            // set label size
            gtk_widget_set_size_request(label, 70, 70);
            gtk_box_append(GTK_BOX(eventbox), label);
            gtk_widget_add_css_class(board_widgets[i][j], "square");
            // set square color
            if ((i + j) % 2 == 0)
            {
                gtk_widget_set_name(board_widgets[i][j], "lightbrown");
            }
            else
            {
                gtk_widget_set_name(board_widgets[i][j], "darkbrown");
            }
            g_signal_connect(gesture, "pressed", G_CALLBACK(on_square_clicked), eventbox);
        }
    }
    for (int j = 0; j < 8; j++)
    {
        eventbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        char label_str[2] = {j + 'a', '\0'};
        label = gtk_label_new(label_str);
        gtk_widget_add_css_class(eventbox, "square");
        gtk_grid_attach(GTK_GRID(chess_grid), eventbox, j + 1, 8, 1, 1);
        gtk_box_append(GTK_BOX(eventbox), label);
        gtk_widget_set_size_request(label, 30, 30);
    }
    // Create a new button for the menu
    menu_button = gtk_button_new_with_label("Menu");
    // Attach the button to the grid
    gtk_grid_attach(GTK_GRID(menu_grid), menu_button, 0, 4, 1, 1);
    // make the btton a normal size
    gtk_widget_set_size_request(menu_button, 60, 30);
    // Connect the button to the callback function
    g_signal_connect(menu_button, "clicked", G_CALLBACK(menugtk), window);

    // Init the chess board state
    board_s = init_board();
    init_co = empty_coords();
    draw_board(init_co);
    if (mode < 3)
    {
        color = 'w';
    }
    else
    {
        color = ' ';
    }
    // Show the window
    gtk_widget_show(window);
}

void play1(GtkApplication *app, GtkWidget *window)
{
    mode = 1;
    init_chess_window(app, window);
}

// launch the menu gtk window
void menu()
{
    // Create a new application
    GtkApplication *app = gtk_application_new("com.example.GtkApplication",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    // Run the application
    g_application_run(G_APPLICATION(app), 0, NULL);
}

void pvp(GtkApplication *app)
{
    printf("pvp available soon\n");
}

void pva(GtkApplication *app)
{
    printf("pva available soon\n");
}

void semi_free(GtkApplication *app)
{
    printf("semi free mode available soon\n");
}

void free_mode(GtkApplication *app)
{
    printf("free mode available soon\n");
}

void settings(GtkApplication *app)
{
    printf("settings available soon\n");
    menu();
}