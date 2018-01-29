#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include "main.hpp"
#include "display.hpp"
#include "map.hpp"

sf::RenderTexture windowTexture; // texture handle for the screen
sf::Font font; // main font for use

int state = 0; // state handle
\
// vectors of various stuff
std::vector<map_t> map_vector;
std::vector<npc_t> npc_vector;
std::vector<item_t> item_vector;

int main(){
    srand( time ( NULL ) ); // initalize random

    sf::RenderWindow window(sf::VideoMode(800, 600), "Asterisk Cartographer v0"); // create window

    // prepare window texture
    if(!windowTexture.create(800, 600)){
        printf("Failed to build main window texture");
        return -5;
    }

    if (!font.loadFromFile("res/telegrama_raw.ttf")) return -1;

    while (window.isOpen()) { // game loop
        sf::Event event;
        while (window.pollEvent(event)){ // event loop
            if (event.type == sf::Event::Closed) { // check for close request
                window.close(); // close window
            } else if (event.type == sf::Event::KeyReleased){ // check keypresses (release only because why not hack)
                switch (event.key.code){
                    case sf::Keyboard::Escape: return 0; // exit
                    default:
                        printf(".");
                }
            }
        }

        switch (state){
            case 0:
                // main menu
                draw_main_menu();
                break;
        }

        windowTexture.display(); // finish up windowTexture
        const sf::Texture& texture = windowTexture.getTexture(); // build texture object
        sf::Sprite sprite(texture); // build sprite
        sprite.setPosition(0,0); // place sprite

        window.clear();
        window.draw(sprite); // draw window sprite
        window.display();
    }

    return 0;
}

// reads in the gamefile
bool read_gamefile(){
    std::ifstream gamefile ("gamefile.dat");

    // read in game data

    // finish
    return 0;
}

// Writes out the gamefile
bool write_gamefile(){
    std::ofstream gamefile ("gamefile.dat");

    // write version info
    gamefile << "! VERSION 0 START \n";
    gamefile << "# P 0 0 \n";
    gamefile << "# V 3 \n";
    gamefile << "! VERSION END \n";

    // write each level quadrant . . .
    gamefile << "! LEVEL BEGIN \n";

    // if no level data exists, declare this
    gamefile << "# EMPTY \n";

    // write LM data

    // write LD data

    // write TD data
    gamefile << "! LEVEL END \n";

    // write all item definitions
    gamefile << "! ITEM BEGIN \n";
    for (item_t item : item_vector){
        gamefile << "# ITEM " << item.id << " BEGIN \n";
        gamefile << "$ ITEM_TYPE " << item.type << " \n";
        gamefile << "$ ITEM_USEABLE " << !item.unuseable << " \n";
        gamefile << "$ ITEM_DATA " << item.data << " \n";
        gamefile << "$ ITEM_DATA_LEN " << item.data_len << " \n";
        gamefile << "# ITEM " << item.id << " END \n";
    }

    // write quest data

    // write NPC data

    // write Map data

    // write cutscene data
    // (not yet supported in the actual game)
    // here would be information for animations and music and text for each cutscene

    // write bitmap data
    // (not yet supported in actual game)
    // here would be information for textures, images, etc.

    gamefile.close();
    return true;
}

//Writes out function prototypes and functions themselves from several provided samples in order to allow quests to change things, and for the npcfunciton lists
bool write_header_file(){
    std::ofstream npc_header ("action_functions.h");

    return true;
}
