#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cjson/cJSON.h"
#include <dirent.h> 
#include <ncurses.h>
#include "textmenu/textmenu.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct Color3 {
        unsigned short r;
        unsigned short g;
        unsigned short b;
} C3;





void sleep_ms(int milliseconds)
{
    // Convert milliseconds to microseconds
    usleep(milliseconds * 1000);
}

char *read_text_file(char *path)
{
	// Character that store the read
    // character

    // Opening file in reading mode
    FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		printw("Error reading file \"%s\"", path);
		return NULL;
	}
	
	// Getting the file size for reading
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file); // + 1 to account for \0
	rewind(file); // Or fseek(file, 0, SEEK_SET);

	// Allocating the memory
    char *str = malloc(file_size + 1);
	if (str == NULL)
	{
		printw("Error allocating %ld bytes of memory for file allocation", file_size);
		return NULL;
	}

	// Reading the bytes
	size_t bytes_read = fread(str, 1, file_size, file);
    if (bytes_read != file_size) {
        // Handle error: not all bytes were read
        printw("Error reading file: expected %ld bytes, read %zu\n", file_size, bytes_read);
        free(str);
        fclose(file);
        return NULL;
    }

	// Closing the file
    fclose(file);

	return str;
}

void empty_menu_button(int id)
{ }

// TODO: Make this call a function so we can free or something and shove in a function
TextMenu directory_menu;

void pick_list()
{

	add_text_menu_label(&directory_menu, "Files:");

	
	// Generating buttons for all the lists
	DIR *d;
	struct dirent *dir;


	d = opendir("./lists");
	if (d) {
		int i = 0;
		while ((dir = readdir(d)) != NULL) {

			// If file starts with . then ignore it
			if (dir->d_name[0] == '.')
			{
				i++;
				continue;
			}

			add_text_menu_button(&directory_menu, dir->d_name, i, empty_menu_button);
			i++;
		}
		closedir(d);
	} else {
		add_text_menu_button(&directory_menu, "Unable to open directory", 0, empty_menu_button);
	}

	

	// Control Loop
	int frame = 0;
	int last_input;
	int exit = 0;
	while (!exit)
	{
		erase();

		// Rendering the list
		print_text_menu(&directory_menu);
		printw("Frame: %d\n", frame);
		printw("%d\n", last_input);


		// Writes to the screen
		refresh();

		// pause till input
		last_input = getch();

		// Handling input
		switch (last_input)
		{
			case KEY_UP: ;
				text_menu_move(&directory_menu, -1);
				break;
			case KEY_DOWN: ;
				text_menu_move(&directory_menu, 1);
				break;
			case 27: ; // ESC key
				exit = 1;
				break;



		}

		frame++;
	}

	free_text_menu(&directory_menu);


}


int main()
{
	initscr();
	keypad(stdscr, TRUE);
	
	if (start_color() != OK)
		return 1;

	pick_list();

	endwin();
	return 0;
}
