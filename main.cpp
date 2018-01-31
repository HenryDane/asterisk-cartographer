#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include "main.hpp"
#include "display.hpp"
#include "map.hpp"

void quest_create_menu();

sf::RenderTexture windowTexture; // texture handle for the screen
sf::Font font; // main font for use

int state = 0; // state handle
\
// vectors of various stuff
std::vector<map_t> map_vector;
std::vector<npc_t> npc_vector;
std::vector<item_t> item_vector;

// target ids (all ids are unique)
int id_current = 0;

int id_next = 1;

sf::Texture textures[NUM_TEXTURES];

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

void getText(char *message, char *variable, int size){
    printf("\n %s: ", message);
    fgets(variable, sizeof(char) * size, stdin);
    sscanf(variable, "%[^\n]", variable);
}

int main(){
    srand( time ( NULL ) ); // initalize random

    sf::RenderWindow window(sf::VideoMode(S_WIDTH, S_HEIGHT), "Asterisk Cartographer v0"); // create window

    if(!windowTexture.create(S_WIDTH, S_HEIGHT)){ // prepare window texture
        printf("Failed to build main window texture \n");
        return -5;
    }

    printf("Reading %d texture files \n", NUM_TEXTURES_DEFINED);
    for (int i = 0; i < NUM_TEXTURES_DEFINED; i++){ // loop through and load textures
        char tmp[80];
        sprintf(tmp, "res/%d.png" , i);
        if (!textures[i].loadFromFile(tmp)){
            printf("-%d-", i);
        } else {
            printf("[%d]", i);
        }
    }
    printf("\n");

    printf("Loading font . . . \n");
    if (!font.loadFromFile("res/telegrama_raw.ttf")) return -1; // set up font

    item_vector.push_back((item_t) {12, 3, false, "Flower", 6, 12}); // add a dummy item

    char tmp[80]; // temp data

    while (true){
        int choice = 0; // choice
        int edit_mode = 0; // 1 - map, 2 - quest, 3 - item
        char str[80];

        switch (state){
            case 0: // main menu
                printf("Main menu: \n 1 - View all textures \n 2 - Select a map to view \n 3 - Select a quest to edit \n 4 - Select an item to edit \n 5 - View all items \n 6 - Hide window \n 0 - Save menu \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0: // exit
                        printf("Save to file? [Y/N]: ");
                        scanf("%2s", str);
                        if (str[0] == 'Y' || str[0] == 'y'){
                            printf("Saving . . . \n");
                            printf("Exiting . . . \n");
                            return 0;
                        } else if (str[0] == 'N' || str[0] == 'n'){
                            printf("Exiting . . . \n");
                            return 0;
                        } else {
                            state = 0;
                        }
                        break;
                    case 1: // view textures
                        state = 1;
                        break;
                    case 2: // select map
                        state = 2;
                        break;
                    case 3: // select quest
                        state = 3;
                        break;
                    case 4: // select item
                        state = 4;
                        break;
                    case 5: // view all items
                        state = 5;
                        break;
                    case 6:
                        window.setVisible(false);
                        break;
                    default: // something else
                        printf("Unknown Option \n");
                }
                break;
            case 1: // texture menu
                printf("Texture menu: \n 1 - Edit a texture \n 0 - Go back \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0:
                        state = 0;
                        break;
                    case 1:
                        state = 6;
                        break;
                }
                break;
            case 2: // map select menu
                printf("Map select menu: \n 1 - Enter map id \n 0 - Go back \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0:
                        state = 0;
                        break;
                    case 1:
                        scanf("%d", &id_current);
                        state = 7;
                        break;
                }
                break;
            case 3:
                printf("Quest edit menu: \n 1 - Create new quest \n 2 - Edit an existing quest \n 0 - Go back \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0:
                        state = 0;
                        break;
                    case 1:
                        state = 8;
                        break;
                }
                break;
            case 4:
                printf("Item edit menu: \n 1 - Create new item \n 2 - Edit an existing item \n 0 - Go back \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0:
                        state = 0;
                        break;
                    case 1:
                        state = 9;
                        break;
                    case 2:
                        state = 8;
                        break;
                }
                break;
            case 7:
                printf("Map edit menu: \n 0 - Go back \n");
                scanf("%d", &choice);
                switch (choice){
                    case 0:
                        state = 0;
                        break;
                }
                break;
            case 8:
                quest_create_menu();
                state = 0;
                break;
            default:
                printf("Error condition of state [%d] \n", state);
                system("pause");
        }

        // process window events
        sf::Event event;
        while (window.pollEvent(event)){ // event loop
            if (event.type == sf::Event::Closed) { // check for close request
                window.close(); // close window
            }
        }

        windowTexture.display(); // finish up windowTexture
        const sf::Texture& texture = windowTexture.getTexture(); // build texture object
        sf::Sprite sprite(texture); // build sprite
        sprite.setPosition(0,0); // place sprite

        window.clear();
        window.draw(sprite); // draw window sprite
        window.display();
        windowTexture.clear();
    }

    return 0;
}

void quest_create_menu(){
    char tmp[80]; // buffer
    int n; // buffer

    quest_t quest; // create temp quest

    printf("Quest creation menu \n[Spaces must be dashes] \nEnter quest name: ");
    scanf("%31s", tmp);
    for (int i = 0; i < 32; i++){
        if (tmp[i] == '-'){
            quest.title[i] = ' ';
        } else {
            quest.title[i] = tmp[i];
        }
    }

    printf("Enter quest issuer's name: ");
    scanf("%31s", tmp);
    for (int i = 0; i < 32; i++){
        if (tmp[i] == '-'){
            quest.issuer[i] = ' ';
        } else {
            quest.issuer[i] = tmp[i];
        }
    }

    printf("Number of stages: ");
    scanf("%d", &n);
    quest.num_stages = n;

    printf("Quest so far: \n  Title: %s \n  Issuer: %s \n   Number of Stages: %d \n", quest.title, quest.issuer, quest.num_stages);

    system("PAUSE");
}
