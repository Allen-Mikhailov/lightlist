#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cjson/cJSON.h"
#include <dirent.h> 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct Color3 {
        unsigned short r;
        unsigned short g;
        unsigned short b;
} C3;


#define TEXT_MENU_BUTTON_TYPE 1
typedef struct TextMenuButtonStruct {
	int type;
	int id;
	char* text;
	void (*onclick) (int id);	
} TextMenuButton;


typedef struct TextMenuStruct {
	int item_count;
	int hovered_item;
	void **menu_items;	
	int allocated_items;
} TextMenu;

TextMenuButton* create_text_menu_button(char* text, int id, void (*onclick) (int id))
{
	TextMenuButton *button = malloc(sizeof(TextMenuButton));
	button->type = TEXT_MENU_BUTTON_TYPE;
	button->id = id;
	button->onclick = onclick;

	// Assuming text gets cleared when we close
	char *text2 = malloc(strlen(text)+1);
	strcpy(name, text);
	button->text = text2;

	return button;
}

TextMenuButton* attach_text_menu_button(TextMenu *menu, TextMenuButton *button)
{
	menu->item_count++;
	if (menu->item_count > menu->allocated_items)
	{
		if (menu->allocated_items == 0) {
			menu->allocated_items = 5;
			menu->menu_items = malloc(sizeof(void*) * menu->allocated_items);
		} else {
			menu->allocated_items *= 2;
			menu->menu_items = realloc(menu->menu_items, sizeof(void*) * menu->allocated_items);
		}
	}
	
	TextMenuButton **menu_items = (TextMenuButton**) menu->menu_items;
	menu_items[menu->item_count - 1] = button;

	// Idk why I decided to return
	return button;
}


TextMenuButton* add_text_menu_button(TextMenu *menu, char* text, int id, void (*onclick) (int id))
{
	TextMenuButton *button = create_text_menu_button(text, id, onclick);
	attach_text_menu_button(menu, button);
	return button;
}

void writeNumCharToStr(char* str, unsigned char val)
{
	*str = '0' + (val / 100);
	str++;
	*str = '0' + ((val%100) / 10);
	str++;
	*str = '0' + (val%10);
}

char COLOR_STR[] = "\033[38;2;000;000;000m";
const int COLOR_STR_SIZE = sizeof(COLOR_STR) - 1;

void set_print_color(C3 *color)
{
	writeNumCharToStr(COLOR_STR+7,  MIN(color->r, 255));
	writeNumCharToStr(COLOR_STR+11, MIN(color->g, 255));
	writeNumCharToStr(COLOR_STR+15, MIN(color->b, 255));
	printf("%s", COLOR_STR);
}


void print_text_menu(TextMenu* menu)
{
	for (int i = 0; i < menu->item_count; i++)
	{
		// Item type will be the first item in the struct for all types
		void *menu_item = menu->menu_items[i];
		int item_type = *( (int*) menu_item );
		switch(item_type)
		{
			case TEXT_MENU_BUTTON_TYPE:
				TextMenuButton *button = (TextMenuButton*) menu_item;
				printf("%s\n", button->text);
				break;
		}
	}
}


void sleep_ms(int milliseconds)
{
    // Convert milliseconds to microseconds
    usleep(milliseconds * 1000);
}

char **read_directory(char *path)
{
	DIR *d;
	struct dirent *dir;

	int count = 0;

	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			printf("%s\n", dir->d_name);
			count++;
		}
		closedir(d);
	} else {
		return NULL;
	}

	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			printf("%s\n", dir->d_name);
			count++;
		}
		closedir(d);
	} else {
		return NULL;
	}
}

char *read_text_file(char *path)
{
	// Character that store the read
    // character

    // Opening file in reading mode
    FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Error reading file \"%s\"", path);
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
		printf("Error allocating %ld bytes of memory for file allocation", file_size);
		return NULL;
	}

	// Reading the bytes
	size_t bytes_read = fread(str, 1, file_size, file);
    if (bytes_read != file_size) {
        // Handle error: not all bytes were read
        fprintf(stderr, "Error reading file: expected %ld bytes, read %zu\n", file_size, bytes_read);
        free(str);
        fclose(file);
        return NULL;
    }

	// Closing the file
    fclose(file);

	return str;
}


TextMenu directory_menu;

void empty_menu_button(int id)
{ }

void pick_list()
{	
	DIR *d;
	struct dirent *dir;

	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			add_text_menu_button(directory_menu, dir->dname, i, empty_menu_button);
		}
		closedir(d);
	} else {
		return NULL;
	}


}


int main()
{
	return 0;
}
