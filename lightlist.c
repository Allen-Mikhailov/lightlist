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

int in_command = 0;
int command_length = 0;
char command_line_input[1000];


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

void create_new_list_button(int id)
{

}

char **parse_command(char* str)
{
	int length = strlen(str);

	int allocated_arguments = 2;
	int arguments = 0;

	int buffer_allocated = length+2;
	int buffer_used = 0;

	char **commands = malloc(sizeof(char*) * 2);
	char *command_buffer = malloc(sizeof(char) * buffer_allocated);

	// Parse input
	int in_string = 0;
	int in_escape_character;
	int was_space_last = 0;
	for (int i = 0; i < length; i++)
	{
		char c = command_line_input[i];
		if (in_escape_character)
		{
			was_space_last = 0;
			continue;
		}

		if (c == ' ' && !was_space_last) {
			was_space_last = 1;
			arguments++;

			if (arguments > allocated_arguments)
				commands = realloc(commands, sizeof(char*) * arguments);

			continue;
		}

		if (c == '\\')
			in_escape_character = 1;

		else if (c == '"')
			in_string = !in_string;
		else {

		}

		was_space_last = 0;

	}

	return commands;


}

void free_command_args(char **args)
{
	free(args[0]);
	free(args);
}

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
		add_text_menu_label(&directory_menu, "Unable to open directory");
	}


	add_text_menu_button(&directory_menu, "New List", 0, &create_new_list_button);

	

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

		if (in_command)
		{
			printw("Command Line:\n%s", command_line_input);
		}


		// Writes to the screen
		refresh();

		// pause till input
		last_input = getch();

		// Handling input
		if (last_input == KEY_UP || last_input == 'k')
			text_menu_move(&directory_menu, -1);

		if (last_input == KEY_DOWN)
			text_menu_move(&directory_menu, 1 || last_input == 'j');

		if (last_input == 27) // ESC Key
			 exit = 1;

		if (last_input == 10) // Enter Key
		{
						

			// Clearing the string
			command_line_input[0] = '\0';
			in_command = 0;
		}
		
		// open command line
		if (last_input == ':' && !in_command)
		{
			in_command = 1;
			command_line_input[0] = '\0';
			command_length = 0;
			// The colon will get added by the next if statement
		}

		if (last_input >= 33 && last_input <= 126 && in_command)
		{
			command_line_input[command_length] = (char) last_input;
			command_line_input[command_length+1] = '\0';
			command_length++;
		}

		if (last_input == 263 && command_length > 0) // Backspace
		{
			command_line_input[command_length-1] = '\0';
			command_length--;
		}




		frame++;
	}

	free_text_menu(&directory_menu);


}


int main()
{
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	// raw();
	set_escdelay(25);	

	if (start_color() != OK)
		return 1;

	pick_list();

	endwin();
	return 0;
}
