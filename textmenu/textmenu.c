#include "textmenu.h"
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>



TextMenuItem* create_text_menu_button(char* text, int id, void (*onclick) (int id))
{
	TextMenuItem *button = malloc(sizeof(TextMenuItem));
	button->type = TEXT_MENU_BUTTON_TYPE;
	button->id = id;
	button->onclick = onclick;
	button->selectable = 1;

	// Assuming text gets cleared when we close
	char *text2 = malloc(strlen(text)+1);
	strcpy(text2, text);
	button->text = text2;

	return button;
}

TextMenuItem *create_text_menu_label(char* text)
{
	TextMenuItem *label = malloc(sizeof(TextMenuItem));
	label->type = TEXT_MENU_LABEL_TYPE;
	label->selectable = 0;

	// Assuming text gets cleared when we close
	char *text2 = malloc(strlen(text)+1);
	strcpy(text2, text);
	label->text = text2;


	return label;
}

TextMenuItem* attach_text_menu_item(TextMenu *menu, TextMenuItem *button)
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
	
	menu->menu_items[menu->item_count - 1] = button;

	// Idk why I decided to return
	return button;
}


TextMenuItem* add_text_menu_button(TextMenu *menu, char* text, int id, void (*onclick) (int id))
{
	TextMenuItem *button = create_text_menu_button(text, id, onclick);
	attach_text_menu_item(menu, button);
	return button;
}

TextMenuItem *add_text_menu_label(TextMenu *menu, char *text)
{
	TextMenuItem *label = create_text_menu_label(text);
	attach_text_menu_item(menu, label);
	return label;

}

void free_text_menu_item(TextMenuItem *item)
{
	free(item->text);
	free(item);
}

void free_text_menu(TextMenu *menu)
{

	for (int i = 0; i < menu->item_count; i++)
	{
		TextMenuItem *item = menu->menu_items[i];
		free_text_menu_item(item);
	}
	free(menu->menu_items);
}

void print_text_menu(TextMenu* menu)
{
	// Setting ncurses settings
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);

	for (int i = 0; i < menu->item_count; i++)
	{
		TextMenuItem *menu_item = (TextMenuItem*) menu->menu_items[i];

		switch(menu_item->type)
		{
			case TEXT_MENU_BUTTON_TYPE:;
				if (menu->hovered_item == i)
					attrset(COLOR_PAIR(2));
				else
					attrset(COLOR_PAIR(1));

				printw("%s\n", menu_item->text);
				break;

			case TEXT_MENU_LABEL_TYPE: ;
				printw("%s\n", menu_item->text);
				break;
		}
	}
	attrset(COLOR_PAIR(1));
}

void text_menu_move(TextMenu *menu, int dir)
{
	int move_attempts = 0;
	int new_item = menu->hovered_item;
	while (move_attempts < menu->item_count)
	{
		move_attempts++;

		new_item = (new_item + menu->item_count + dir) % menu->item_count;

		TextMenuItem *menu_item = (TextMenuItem*) menu->menu_items[new_item];
		if (menu_item->selectable)
		{
			menu->hovered_item = new_item;
			return;
		}
	}
}
