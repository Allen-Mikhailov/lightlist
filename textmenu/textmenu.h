#ifndef TEXT_MENU_H
#define TEXT_MENU_H

#define TEXT_MENU_BUTTON_TYPE 1
#define TEXT_MENU_LABEL_TYPE 2

typedef struct TextMenuItemStruct {
	int type;
	int selectable;
	int id;
	char* text;
	void (*onclick) (int id);
} TextMenuItem;

typedef struct TextMenuStruct {
	int item_count;
	int hovered_item;
	TextMenuItem **menu_items;	
	int allocated_items;
} TextMenu;

// Menu functions
TextMenuItem* attach_text_menu_item(TextMenu *menu, TextMenuItem *button);

TextMenuItem* create_text_menu_button(char* text, int id, void (*onclick) (int id));
TextMenuItem* add_text_menu_button(TextMenu *menu, char* text, int id, void (*onclick) (int id));

TextMenuItem *add_text_menu_label(TextMenu *menu, char *text);
TextMenuItem *create_text_menu_label(char* text);

// Cleanup
void free_text_menu(TextMenu *menu);
void free_text_menu_item(TextMenuItem *item);



// Usage functions
void print_text_menu(TextMenu* menu);
void text_menu_move(TextMenu *menu, int dir);


#endif // TEXT_MENU_H
