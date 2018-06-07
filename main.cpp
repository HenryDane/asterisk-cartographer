#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>

#include "main.hpp"
#include "parse.hpp"

#include <SFML/System.hpp>
#include <cstdlib>

unsigned int DEBUG_LEVEL = 0;

using namespace std;

sf::Color colors[32];

//vectors of various stuff
std::vector<map_t> map_vector;
std::vector<npc_t> npc_vector;
std::vector<crate_t> crate_vector;
std::vector<coord_t> start_vector;
std::vector<enemy_t> enemy_vector;
std::vector<portal_t> portal_vector;

// quest stuff
std::vector<std::string> quest_text_vector;
std::vector<mquest_t> quest_vector;

std::string map_dir_name = " ";

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

// for splitting multiple images into one (untested)
void compile_multiple(sf::Image image, std::string prefix){
    vector<sf::Image> images;

    int width = image.getSize().x;
    int height = image.getSize().y;

    int n_tile_x = ceil(width / 16);
    int n_tile_y = ceil(height / 16);

    for (int i = 0; i < n_tile_x; i++){
        for (int j = 0; j < n_tile_y; j++){
            sf::Image timage;
            timage.create(16, 16, sf::Color::Black);
            timage.copy(image, i * 16, j * 16);
            images.push_back(timage);

            char tmp[80];
            sprintf(tmp, "%s_%d.png", prefix, i * n_tile_x + j);
            printf(tmp); printf("\n");
            timage.saveToFile(tmp);
        }
    }
}

int main(int argc, char *argv[]){
    // handle arguments
    cout << "=== Asterisk Cartographer Utility V1 ===" << endl;
    if (argc == 2){
        cout << "[ ] Map directory is \"" << argv[1] << "\"" << endl;
        map_dir_name = argv[1];
    } else if (argc == 1){
        cout << "[ ] Assuming map directory is \"map_0\\\"" << endl;
        map_dir_name = "map_2"; // temporary modification
    } else if (argc == 3){
        cout << "[ ] Map directory is \"" << argv[1] << "\"" << endl;
        map_dir_name = argv[1];

        DEBUG_LEVEL = atoi(argv[2]);
    } else {
        cout << "ERROR: I can't understand your arguments" << endl;
        return -20;
    }

    // debug
    DEBUG_LEVEL = 1000;
    map_dir_name = "map_3";

    parse();
}
