#include <hildon/hildon.h>
 
int main (int argc, char **argv)
{
    HildonProgram *program;
 
    GtkWidget *win;
    GtkWidget *label;
 
    hildon_gtk_init (&argc, &argv);
 
    program = hildon_program_get_instance ();
 
    /* Create the main window */
    win = hildon_stackable_window_new ();
    gtk_window_set_title ( GTK_WINDOW (win), "CPS2emu");
     
    //msg on screen...
    label =  gtk_label_new ("This application cant run on x86 machines!");
    gtk_container_add ( GTK_CONTAINER (win), label);
    
    //msg on shell...
    printf("This application cant run on x86 machines!\n");
    
    //Close application...
    g_signal_connect (win, "destroy", G_CALLBACK (gtk_main_quit), NULL);
 
    /* This call show the window and also add the window to the stack */
    gtk_widget_show_all (win);
    gtk_main();
 
    return 0;
}
