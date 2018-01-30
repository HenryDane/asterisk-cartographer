#include <SFML/Graphics.hpp>
#include "main.hpp"

void put_text(char * str, int x, int y){
    sf::Text text;
    text.setCharacterSize(16);
    text.setColor(sf::Color::White);
    text.setFont(font);
    text.setString(str);
    text.setPosition(x, y);
    windowTexture.draw(text);
}

void put_tile(int tex_id, int x, int y){
    sf::RectangleShape r(sf::Vector2f(16, 16));
    r.setTexture(&textures[tex_id]);
    r.setPosition(x, y);
    windowTexture.draw(r);
}

void draw_main_menu(){
    put_text("MAIN MENU:" , 0, 0);
    put_text("Q - View all items", 16, 16);
    put_text("W - Select a map", 16, 32);
    put_text("E - Select a level", 16, 48);
    put_text("R - Select an NPC", 16, 64);
    put_text("T - View textures and associations", 16, 80);
    put_text("Y - Select a quest", 16, 96);
    put_text("U - Write to file", 16, 112);
    put_text("I - Read to file", 16, 128);
}

void draw_item_view_menu(){
    put_text("ITEMS: " , 0, 0);
    int y = 16;
    char tmp[80];
    for (item_t item : item_vector){
        put_tile(item.tex_id, 32, y );
        sprintf(tmp, "Item [ID: %d] [TYPE: %d] %s", item.id, item.type, item.data);
        put_text(tmp, 48, y);
        y += 16;
        if (y > S_HEIGHT) break;
    }

    put_text("Q - Select item to edit", 608, 16 );
    put_text("W - Scroll up", 608, 32);
    put_text("E - Scroll down", 608, 48);
    put_text("R - Go back", 608, 64);
}

void draw_select_item(){
    put_text("Enter numeric ID in console window and press [ENTER]", 0, 0);
}

void draw_item_edit_menu(){
    put_text("Q - Enter full item data in console", 0, 0);
}
