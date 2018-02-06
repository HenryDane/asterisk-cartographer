#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include "main.hpp"

#include <cstdlib>

using namespace std;

sf::Color colors[32];

//vectors of various stuff
std::vector<map_t> map_vector;
std::vector<npc_t> npc_vector;
std::vector<crate_t> crate_vector;
std::vector<coord_t> start_vector;
std::vector<enemy_t> enemy_vector;

std::string map_dir_name = " ";

int global_map_index = 0;

// fixes MinGW bug
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

int main(int argc, char *argv[]){
    // map image
    sf::Image map_image;

    // txt files
    ifstream map_txt;
    ifstream object_txt;
    ifstream quest_txt;

    string prefix = " ";
    string prefix_alt = " ";

    int num_maps;

    bool build_header = false;

    int num_fire_tex = 0;
    int num_vegetation_tex = 0;
    int num_machinery_tex = 0;
    int num_water_tex = 0;
    int num_o_machinery = 0;

    int num_objects;

    bool quests_ok = false;

    // open map_txt
    cout << "=== Asterisk Cartographer Utility V1 ===" << endl;
    if (argc == 2){
        cout << "[ ] Map directory is \"" << argv[1] << "\"" << endl;
        map_dir_name = argv[1];
    } else if (argc == 1){
        cout << "[ ] Assuming map to directory is \"map_0\\\"" << endl;
        map_dir_name = "map_1";
    } else {
        cout << "ERROR: I can't understand your arguments" << endl;
        return -20;
    }

    cout << "[ ] Opening maps.txt" << endl;
    map_txt.open("artifact\\" + map_dir_name + "\\maps.txt");
    if (!map_txt.is_open()) {
        cout << "Failed to load map.txt, exiting" << endl;
        return -1;
    }

    // first line starts with '!' ?
    char tmp;
    map_txt >> tmp;
    if (tmp != '!'){
        cout << "ERROR: Failed to find proper header [" << tmp << "]" << endl;
        return -2;
    }

    // 'maps.txt' follows '!' ?
    map_txt >> prefix;
    if (prefix != "maps.txt") {
        cout << "ERROR: Failed to find proper header data [" << prefix << "]" << endl;
        return -3;
    }

    // read number of maps
    map_txt >> num_maps;
    cout << "[M] Expecting " << num_maps << " maps" << endl;

    // read prefix
    map_txt >> prefix;
    cout << "[M] Expecting prefix " << prefix << endl;

    // read header boolean
    map_txt >> build_header;
    cout << "[M] Build header file: " << ((build_header > 0) ? "true" : "false") << endl;

    cout << "[ ] Finished parsing maps.txt" << endl;
    map_txt.close();

    cout << "[ ] Opening objects.txt" << endl;
    object_txt.open("artifact\\" + map_dir_name + "\\objects.txt");
    if (!object_txt.is_open()) {
        cout << "ERROR: Failed to load objects.txt, exiting" << endl;
        return -4;
    }

    // first line starts with '!' ?
    object_txt >> tmp;
    if (tmp != '!'){
        cout << "ERROR: Failed to find proper header [" << tmp << "]" << endl;
        return -5;
    }

    // 'objects.txt' follows '!' ?
    object_txt >> prefix_alt;
    if (prefix_alt != "objects.txt") {
        cout << "ERROR: Failed to find proper header data [" << prefix_alt << "]" << endl;
        return -6;
    }

    object_txt >> tmp;
    if (tmp != '!'){
        cout << "ERROR: Failed to find proper quest header [" << tmp << "]" << endl;
        return -7;
    }

    object_txt >> prefix_alt;
    cout << "[O] Quest mode is " << prefix_alt << endl;
    if (prefix_alt == "quests_on"){
        quests_ok = true;
    } else if (prefix_alt == "quests_off"){
        quests_ok = false;
    } else {
        cout << "ERROR: Quest mode is invalid " << endl;
        return -8;
    }

    object_txt >> prefix_alt;
    cout << "[O] Found prefix " << prefix_alt << endl;

    if (prefix_alt != prefix){
        cout << "ERROR: Prefixes differ [" << prefix << "] [" << prefix_alt << "] " << endl;
        return -9;
    }

    object_txt >> num_fire_tex;
    object_txt >> num_vegetation_tex;
    object_txt >> num_machinery_tex;
    object_txt >> num_water_tex;
    object_txt >> num_o_machinery;
    cout << "[O] Expecting textures " << num_fire_tex << " " << num_vegetation_tex << " " << num_machinery_tex << " " << num_water_tex << " " << num_o_machinery << endl;

    object_txt >> num_objects;
    cout << "[O] Expecting " << num_objects << " objects" << endl;

    bool done = false;
    if (num_objects > 0) {
        cout << "[O] Beginning object parsing" << endl;
        for (int h = 0; h < num_objects; h++){
            if (done) break;
            int x, y, type, num_data;
            object_txt >> x;
            object_txt >> y;
            object_txt >> type;
            object_txt >> num_data;
            if (x == -1){
                cout << "[O] Found object with X of " << x << ", moving on to next map . . . " << endl;
                global_map_index++;
                object_txt >> num_data; // consume next integer
            } else if (x == -2) {
                cout << "[O] Found object with X of " << x << ", objects are disabled . . . " << endl;
                object_txt >> num_data; // consume next integer
                done = true;
                break;
            }
            int a;
            item_t item;
            npc_item_t nitem;
            crate_t crate;
            npc_t npc;
            enemy_t enemy;
            portal_t portal;

            //system("PAUSE");
            switch (type){
                case 0: // chest
                    cout << "    Found chest (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    object_txt >> a; // read object number
                    crate.x = x;
                    crate.y = y;
                    crate.num_items = a;
                    for (int i = 0; i < a; i++){
                        object_txt >> a;
                        item.id = a;
                        object_txt >> a;
                        item.type = a;
                        object_txt >> a;
                        item.unuseable = (a > 0) ? false : true;
                        for (int z = 0; z < 10; z++){
                            item.data[z] = ' ';
                        }
                        item.data_len = 1;
                        item.tex_id = 0;
                        crate.inventory.push_back(item);
                    }
                    crate_vector.push_back(crate);
                    break;
                case 1: // NPC
                    cout << "    Found NPC (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    object_txt >> npc.id;
                    object_txt >> npc.health;
                    object_txt >> npc.type;
                    object_txt >> npc.inventory_size;
                    if (npc.inventory_size > 16) {
                        cout << "WARN: Inventory of NPC: " << npc.id << " is too large (" << npc.inventory_size << ")" << endl;
                    }
                    for (int i = 0; i < npc.inventory_size; i++){
                        for (int z = 0; z < 10; z++){
                            item.data[z] = ' ';
                        }
                        nitem.data_len = 0;
                        object_txt >> nitem.id;
                        object_txt >> nitem.type;
                        object_txt >> nitem.cost;
                        npc.inventory[i] = nitem;
                    }
                    object_txt >> npc.is_merchant;
                    object_txt >> npc.quest_id;
                    object_txt >> npc.x;
                    object_txt >> npc.y;
                    npc.is_alive = true;
                    npc.is_ablaze = false;
                    npc.map_index = global_map_index;
                    npc_vector.push_back(npc);
                    break;
                case 2: // enemy
                    cout << "    Found enemy (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    cout << "WARN: Enemies are unsupported" << endl;
                    object_txt >> enemy.id;
                    object_txt >> enemy.health;
                    object_txt >> enemy.type;
                    object_txt >> enemy.ammunition;
                    object_txt >> enemy.immortal;
                    enemy.x = x;
                    enemy.y = y;
                    enemy.map_index = global_map_index;
                    enemy_vector.push_back(enemy);
                    break;
                case 3: // portal
                    cout << "    Found type 2 portal (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    cout << "WARN: Type 2 portals are unsupported" << endl;
                    object_txt >> portal.mapid_target;
                    object_txt >> portal.x_target;
                    object_txt >> portal.y_target;
                    portal.x = x;
                    portal.y = y;
                    portal.mapid = global_map_index;
                    break;
                case 4: // undefined
                    cout << "    Found undefined object (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    cout << "WARN: Undefined object encountered " << endl;
                    break;
                default:
                    cout << "ERROR: Found bad object type: " << type << endl;
                    return -10;

            }
        }
        object_txt.clear();
        object_txt.ignore(numeric_limits<streamsize>::max(), '\n');
        object_txt.ignore(1000);
    } else {
        cout << "[O] Object lookup disabled, continuing to map parsing" << endl;
    }

    cout << "[ ] Finished parsing of objects.txt " << endl;
    object_txt.close();

    // set up header file for parsing, or set up hex file
    ofstream header_file;
    cout << "[ ] Exporting data to C file" << endl;
    if (!build_header){
        cout << "WARN: Binary file output not yet supported, using .c file instead" << endl;
        build_header = true;
        // return -11;
    }

    cout << "[H] Building map_data.h" << endl;
    header_file.open("map.c");
    header_file << "// Autogenerated C file from asterisk-cartographer utility" << endl;
    header_file << "// Built on " << currentDateTime() << endl << endl;

    header_file << "#include \"main.h\"" << endl;

    // header_file << "#undef NUM_MAPS" << endl;
    header_file << "#define NUM_MAPS " << num_maps << endl << endl; // write macro

    header_file << "#define NUM_WATER_TEX " << num_water_tex << endl; // write macro
    header_file << "#define NUM_VEGETATION_TEX " << num_vegetation_tex << endl; // write macro
    header_file << "#define NUM_MACHINERY_TEX " << num_machinery_tex << endl; // write macro
    header_file << "#define NUM_OVER_MACHINERY_TEX " << num_o_machinery << endl; // write macro
    header_file << "#define NUM_FIRE_TEX " << num_fire_tex << endl; // write macro

    // add null entity list
    header_file << endl << "entity_t null_entities_list[4] = { {0, 7, 7, 2}, {0, 7, 8, 3}, {0, 7, 9, 4}, {0, 7, 10, 1} };" << endl;

    // read map key
    cout << "[ ] Reading map key " << endl;
    if (!map_image.loadFromFile("artifact//map_key.png")){
        cout << "ERROR: Failed to load map_key.png" << endl;
        return -12;
    }

    sf::Vector2u size_map = map_image.getSize();
    cout << "[K] Read map key of size x: " << size_map.x << " y: " << size_map.y << endl;
    if (size_map.x != 32 || size_map.y != 1){
        cout << "ERROR: Map key is wrong size" << endl;
    }

    for(int i = 0; i < 32; i++){
        colors[i] = map_image.getPixel(i, 0);
    }
    cout << "[ ] Finished reading map_key" << endl;

    cout << "[ ] Beginning parsing of " << num_maps << " maps " << endl;
    for (int i = 0; i < num_maps; i++){
        prefix_alt = "artifact/" + map_dir_name + "/";
        prefix_alt.append(prefix);
        prefix_alt += patch::to_string(i);
        prefix_alt.append(".png");
        if(!map_image.loadFromFile(prefix_alt)){
            cout << "ERROR: Failed to load map image " << prefix_alt << endl;
            return -13;
        }

        size_map = map_image.getSize();

        // set up header for export
        header_file << endl << "// GENERATED MAP " << prefix << i << endl;

        header_file << "int " << prefix << i << "_map_data[] = {";

        cout << "[M] Loaded map " << prefix_alt << " with width " << size_map.x << " and height " << size_map.y << endl;

        cout << "    Writing tile data . . . " << endl;
        //for (int j = 0; j < size_map.x ; j++){
        //    for (int k = 0; k < size_map.y; k++){
        for (int j = size_map.x - 1; j > 0; j--){
            for (int k = size_map.y -1; k > 0; k--){
                sf::Color color_tmp = map_image.getPixel(j, k);

                // search
                int m = 0;
                for (; m < 32; m++){
                    if (colors[m] == color_tmp){
                        break; // break if color match found
                    }
                }

                if (build_header){
                    header_file << m << ", "; // write tile type
                }

            }
            header_file << endl; // prettify output
        }
        header_file << "0};" << endl;
        header_file << "map_t " << prefix << i << "_map = {" << size_map.x << ", " << size_map.y << "," << prefix << i << "_map_data ";
        header_file << ", 0, null_entities_list};" << endl;

        cout << "    Writing starting coordinates" << endl;
        header_file << "coord_t " << prefix << i << "_start = {0, 0};" << endl;

        cout << "    Writing NPC definitions" << endl;
        header_file << "npc_t " << prefix << i << "_npc ( unsigned int x, unsigned int y ){" << endl; // define function def
        header_file << "    npc_t npc_null = {-1, 0, 0, 0, {-1, 0, ' ', 0}, false, false, false, 0, -1, -1 };" << endl;

        // define npc_t instances
        for (int i = 0; i < npc_vector.size(); i++){
            npc_t localnpc = npc_vector.at(i);
            header_file << "    npc_t npc_" << i << " = {" << localnpc.id << ", " << localnpc.health << ", " << localnpc.type << ", ";
            header_file << localnpc.inventory_size << ", {";

            // write out inventory
            for (int j = 0; j < localnpc.inventory_size; j++){
                // start npc_item_t definition
                header_file << "{" << localnpc.inventory[j].id << ", " << localnpc.inventory[j].type << ", \"";

                // copy over string data
                for (int v = 0; v < localnpc.inventory[j].data_len; v++){
                    header_file << localnpc.inventory[j].data[v];
                }

                // empty string
                if (localnpc.inventory[j].data_len == 0){
                    header_file << " ";
                }

                // end npc_item_t definition
                header_file << "\", " << localnpc.inventory[j].data_len << ", " << localnpc.inventory[j].cost << "}";
                if (j != localnpc.inventory_size -1 ){
                    header_file << ", ";
                }
            }

            // close inventory definition
            header_file << "}, " << ((localnpc.is_merchant) ? "true" : "false") << ", " << ((localnpc.is_ablaze) ? "true" : "false") << ", " << ((localnpc.is_alive) ? "true" : "false") << ", " << localnpc.quest_id << ", " << localnpc.x << ", " << localnpc.y << "};" << endl;
        }

        header_file << endl;

        for(int i = 0; i < npc_vector.size(); i++){
            npc_t localnpc = npc_vector.at(i);
            header_file << "    if( x == " << localnpc.x << " && y == " << localnpc.y << ") return npc_" << i << ";" << endl;
        }
        header_file << endl << "    return npc_null;" << endl;
        header_file << "}" << endl << endl;// close map definition
    }

    // write NPCs.
    cout << "[M] Generating registry" << endl;
    header_file << "int master_index = 0;" << endl;
    header_file << "map_t cached_map;" << endl;
    header_file << "map_master rogue_map_master[NUM_MAPS];" << endl;
    header_file << "npc_function_ft rogue_npc_master[NUM_MAPS];" << endl;
    header_file << endl;

    cout << "[M] Generating startup procedure" << endl;
    header_file << "void init_maps(){" << endl;
    for (int i = 0; i < num_maps; i++){
        header_file << "    rogue_map_master[" << i <<"] = (map_master) { " << prefix << i << "_map, " << prefix << i << "_start };" << endl;
        header_file << "    rogue_npc_master[" << i <<"] = "<< prefix << i << "_npc;" << endl;
    }

    header_file << "}" << endl << endl;

    cout << "[M] Adding on util function" << endl;

    header_file << "void load_map(int index){" << endl;
    header_file << "    master_index = index;" << endl;
    header_file << "    cached_map = rogue_map_master[index].mapdat;" << endl;
    header_file << "    character_x = rogue_map_master[index].coord.x;" << endl;
    header_file << "    character_y = rogue_map_master[index].coord.y;" << endl;
    header_file << "    cached_map.num_entities = rogue_map_master[index].mapdat.num_entities;" << endl;
    header_file << "    num_entities = rogue_map_master[index].mapdat.num_entities;" << endl;
    header_file << "    printf(\"Loading map %d with start of (%d, %d) and %d entities \\n\", index, rogue_map_master[index].coord.x, rogue_map_master[index].coord.y, rogue_map_master[index].mapdat.num_entities);" << endl;
    header_file << "    for (int i = 0; i < rogue_map_master[index].mapdat.num_entities; i++){" << endl;
    header_file << "        entities[i] = rogue_map_master[index].mapdat.entities[i];" << endl;
    header_file << "    }" << endl;
    header_file << "}" << endl;

    header_file.close();

    return 0;
}
