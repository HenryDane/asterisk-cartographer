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
