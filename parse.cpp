#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>

#include "parse.hpp"
#include "main.hpp"
using namespace std;

int global_map_index = 0;

// map image
sf::Image map_image;
sf::Texture map_texture;

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

int parse(){
    int a = do_top();
    if (a < 0) return a;

    a = do_object();
    if (a < 0) return a;

    a = do_rogue_map();
    if (a < 0) return a;

    a = do_quests();
    if (a < 0) return a;

    cout << "[ ] Done" << endl;

    return 0;
}

int do_top(){
    // open maps.txt
    cout << "[ ] Now parsing maps.txt" << endl;
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
    if (DEBUG_LEVEL > 1) cout << "[M] Expecting " << num_maps << " maps" << endl;

    // read prefix
    map_txt >> prefix;
    if (DEBUG_LEVEL > 1) cout << "[M] Expecting prefix " << prefix << endl;

    // read header boolean
    map_txt >> build_header;
    if (DEBUG_LEVEL > 1) cout << "[M] Build header file: " << ((build_header > 0) ? "true" : "false") << endl;

    if (DEBUG_LEVEL > 1) cout << "[ ] Finished parsing maps.txt" << endl;
    map_txt.close();

    return 0;
}
    /*
    Read object definitions
    */
int do_object(){
    char tmp;

    cout << "[ ] Now parsing objects.txt" << endl;
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
    if (DEBUG_LEVEL > 1) cout << "[O] Quest mode is " << prefix_alt << endl;
    if (prefix_alt == "quests_on"){
        quests_ok = true;
    } else if (prefix_alt == "quests_off"){
        quests_ok = false;
    } else {
        cout << "ERROR: Quest mode is invalid " << endl;
        return -8;
    }

    object_txt >> prefix_alt;
    if (DEBUG_LEVEL > 1) cout << "[O] Found prefix " << prefix_alt << endl;

    if (prefix_alt != prefix){
        cout << "ERROR: Prefixes differ [" << prefix << "] [" << prefix_alt << "] " << endl;
        return -9;
    }

    object_txt >> num_fire_tex;
    object_txt >> num_vegetation_tex;
    object_txt >> num_machinery_tex;
    object_txt >> num_water_tex;
    object_txt >> num_o_machinery;
    if (DEBUG_LEVEL > 1) cout << "[O] Expecting textures " << num_fire_tex << " " << num_vegetation_tex << " " << num_machinery_tex << " " << num_water_tex << " " << num_o_machinery << endl;

    object_txt >> num_objects;
    if (DEBUG_LEVEL > 1) cout << "[O] Expecting " << num_objects << " objects" << endl;

    bool done = false;
    if (num_objects > 0) {
        if (DEBUG_LEVEL > 1) cout << "[O] Beginning object parsing" << endl;
        for (int h = 0; h < 1000; h++){
            if (done) break;
            int x, y, type, num_data, temp;
            object_txt >> x;
            object_txt >> y;
            object_txt >> type;
            object_txt >> num_data;
            if (x == -1){
                object_txt >> temp; // consume next integer
                if (DEBUG_LEVEL > 1) cout << "[O] Found map index increment indicator (" << x << "," << y << "," << type << "," << num_data << "," << temp << "), moving on to next map . . . " << endl;
                global_map_index++;
                continue;
            } else if (x == -2) {
                if (DEBUG_LEVEL > 1) cout << "[O] Found object with X of " << x << ", objects are disabled . . . " << endl;
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
                    if (DEBUG_LEVEL > 2) cout << "    Found chest (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
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
                    if (DEBUG_LEVEL > 2) cout << "    Found NPC (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    object_txt >> npc.id;
                    object_txt >> npc.health;
                    object_txt >> npc.type;
                    object_txt >> npc.inventory_size;
                    if (DEBUG_LEVEL > 2) cout << "    NPC ID: " << npc.id << " HEALTH: " << npc.health << " TYPE: " << npc.type << " SIZE: " << npc.inventory_size;
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
                        if (DEBUG_LEVEL > 2) cout << " (Item ID: " << nitem.id << " TYPE: " << nitem.type << " COST: " << nitem.cost << ")";
                        npc.inventory[i] = nitem;
                    }
                    object_txt >> a;
                    npc.is_merchant = (a > 0) ? true : false;
                    object_txt >> npc.quest_id;
                    object_txt >> npc.x;
                    object_txt >> npc.y;
                    if (DEBUG_LEVEL > 2) cout << " Merchant: " << npc.is_merchant << " QID: " << npc.quest_id << " X: " << npc.x << " Y: " << npc.y << endl;
                    npc.is_alive = true;
                    npc.is_ablaze = false;
                    npc.map_index = global_map_index;
                    npc_vector.push_back(npc);
                    break;
                case 2: // enemy
                    if (DEBUG_LEVEL > 2) cout << "    Found enemy (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
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
                    if (DEBUG_LEVEL > 2) cout << "    Found portal (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    object_txt >> portal.mapid_target;
                    object_txt >> portal.x_target;
                    object_txt >> portal.y_target;
                    if (DEBUG_LEVEL > 2) cout << "    Portal goes to  X:" << portal.x_target << " Y:" << portal.y_target << " MAP:" << portal.mapid_target << endl;
                    portal.x = x;
                    portal.y = y;
                    portal.mapid = global_map_index;
                    portal_vector.push_back(portal);
                    break;
                case 4: // undefined
                    if (DEBUG_LEVEL > 2) cout << "    Found undefined object (" << type << ") at " << x << ", " << y << " with num_data of " << num_data << endl;
                    cout << "WARN: Undefined object encountered " << endl;
                    break;
                default:
                    cout << "ERROR: Found bad object type: " << type  << "(" << num_data << ")" << endl;
                    return -10;

            }
        }
        object_txt.clear();
        object_txt.ignore(numeric_limits<streamsize>::max(), '\n');
        object_txt.ignore(1000);
    } else {
        if (DEBUG_LEVEL > 1) cout << "[O] Object lookup disabled, continuing to map parsing" << endl;
    }

    for (int v = 0; v < crate_vector.size(); v++){
        cout << "    Crate: X: " << crate_vector.at(v).x << " Y: " << crate_vector.at(v).y << " Num_Items: " << crate_vector.at(v).num_items << endl;
    }

    for (int v = 0; v < npc_vector.size(); v++){
        cout << "    NPC: X: " << npc_vector.at(v).x << " Y: " << npc_vector.at(v).y << " MAP_ID:" << npc_vector.at(v).map_index << " NUM_ITEMS:" << npc_vector.at(v).inventory_size << endl;;
    }

    if (DEBUG_LEVEL > 1) cout << "[ ] Finished parsing of objects.txt " << endl;
    object_txt.close();

    return 0;
}

int do_rogue_map(){
    // set up header file for parsing, or set up hex file
    ofstream header_file;
    cout << "[ ] Exporting data to C file" << endl;
    if (!build_header){
        cout << "WARN: Binary file output not yet supported, using .c file instead" << endl;
        build_header = true;
        // return -11;
    }

    if (DEBUG_LEVEL > 1) cout << "[H] Building map_data.h" << endl;
    header_file.open("map.c");
    header_file << "// Autogenerated C file from asterisk-cartographer utility" << endl;
    header_file << "// Built on " << currentDateTime() << endl << endl;

    header_file << "#include \"main.h\"" << endl;
    header_file << "#include <stdio.h>" << endl << endl;

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
    if (DEBUG_LEVEL > 1) cout << "[K] Read map key of size x: " << size_map.x << " y: " << size_map.y << endl;
    if (size_map.x != 32 || size_map.y != 1){
        cout << "ERROR: Map key is wrong size" << endl;
    }

    for(int i = 0; i < 32; i++){
        colors[i] = map_image.getPixel(i, 0);
    }
    if (DEBUG_LEVEL > 1) cout << "[ ] Finished reading map_key" << endl;

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

        if (DEBUG_LEVEL > 2) cout << "    Transforming map " << endl;

        // ugly hack #498739479
        map_image.flipHorizontally();
        sf::RenderTexture renderwindow_temp;
        renderwindow_temp.create(max(map_image.getSize().x, map_image.getSize().y), max(map_image.getSize().x, map_image.getSize().y)); // its for debug ok??
        sf::RectangleShape rshape;
        rshape.setSize(sf::Vector2f(map_image.getSize().x, map_image.getSize().y));
        rshape.setPosition(0, map_image.getSize().x);
        sf::Texture tmp_texture;
        tmp_texture.loadFromImage(map_image);
        rshape.setTexture(&tmp_texture);
        rshape.setRotation(-90);
        renderwindow_temp.clear();
        renderwindow_temp.draw(rshape);
        renderwindow_temp.getTexture().copyToImage();
        renderwindow_temp.display();
        map_image = renderwindow_temp.getTexture().copyToImage();

        std::string filedatdebugname = "map_" + patch::to_string(i) + ".png";
        map_image.saveToFile(filedatdebugname);

        // compute size
        size_map = map_image.getSize();

        // awful hack #98737987
        int x = size_map.x;
        size_map.x = size_map.y;
        size_map.y = x;

        if (DEBUG_LEVEL > 2) cout << "    Size X: " << size_map.x << " Y: " << size_map.y << endl;

        // set up header for export
        header_file << endl << "// GENERATED MAP " << prefix << i << endl;

        header_file << "int " << prefix << i << "_map_data[] = {";

        if (DEBUG_LEVEL > 1) cout << "[M] Loaded map " << prefix_alt << " with width " << size_map.x << " and height " << size_map.y << endl;

        if (DEBUG_LEVEL > 2) cout << "    Writing tile data . . . " << endl;
        coord_t startpos = {-1, -1, -1};
        for (int j = 0; j < size_map.x; j++){
            for (int k = 0; k < size_map.y; k++){
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

                if (m == 5){ // if spawn point found
                    if (startpos.map_index > -1){
                        cout << "WARN: Spawn already set, moving to " << k << ", " << j << endl;
                    } else {
                        if (DEBUG_LEVEL > 2) cout << "    Spawn identified at " << k << ", " << j << endl;
                    }
                    startpos.x = k;
                    startpos.y = j;
                    startpos.map_index = global_map_index;
                }

            }
            header_file << endl; // prettify output
        }

        if (startpos.map_index == -1){
            cout << "WARN: Failed to identify a spawn point " << endl;
        }
        header_file << "0};" << endl;
        header_file << "map_t " << prefix << i << "_map = {" << size_map.x << ", " << size_map.y << "," << prefix << i << "_map_data ";
        header_file << ", 0, null_entities_list};" << endl;

        if (DEBUG_LEVEL > 2) cout << "    Writing starting coordinates (" << startpos.x << ", " << startpos.y << ")" << endl;
        header_file << "coord_t " << prefix << i << "_start = {" << startpos.x << ", " << startpos.y << "};" << endl;

        if (DEBUG_LEVEL > 2) cout << "    Writing NPC definitions" << endl;
        header_file << "npc_t " << prefix << i << "_npc ( unsigned int x, unsigned int y ){" << endl; // define function def
        header_file << "    npc_t npc_null = {-1, 0, 0, 0, {-1, 0, ' ', 0}, false, false, false, 0, -1, -1 };" << endl;

        // define npc_t instances
        for (int m = 0; m < npc_vector.size(); m++){
            npc_t localnpc = npc_vector.at(m);
            if(localnpc.map_index != i) continue;
            header_file << "    npc_t npc_" << m << " = {" << localnpc.id << ", " << localnpc.health << ", " << localnpc.type << ", ";
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

        for(int s = 0; s < npc_vector.size(); s++){
            npc_t localnpc = npc_vector.at(s);
            if(localnpc.map_index != i) continue;
            header_file << "    if( x == " << localnpc.x << " && y == " << localnpc.y << " ) return npc_" << s << "; //" << localnpc.map_index << " " << i << endl;
        }
        header_file << endl << "    return npc_null;" << endl;
        header_file << "}" << endl;// close NPC definition

        if (DEBUG_LEVEL > 2) cout << "    Writing Portal definitions" << endl;
        header_file << "portal_t " << prefix << i << "_portal( unsigned int x, unsigned int y ){" << endl;
        header_file << "    portal_t portal_null = {0, 0, -1};" << endl;
        for (int s = 0; s < portal_vector.size(); s++ ){
            portal_t portallocal = portal_vector.at(s);
            if (portallocal.mapid == i){
                header_file << "    // " << portallocal.mapid << " " << i << endl;
                header_file << "    portal_t portal_" << s << " = {" << portallocal.x_target << ", " << portallocal.y_target << ", " << portallocal.mapid_target << " };" << endl;
            }
        }

        // set up return statements
        for (int s = 0; s < portal_vector.size(); s++ ){
            portal_t portallocal = portal_vector.at(s);
            if (portallocal.mapid == i){
                header_file << "    if ( x == " << portallocal.x << " && y == " << portallocal.y << " ) return portal_" << s << ";" << endl;
            }
        }

        header_file << "    return portal_null;" << endl;
        header_file << "}" << endl << endl;
    }


    // write NPCs.
    if (DEBUG_LEVEL > 1) cout << "[M] Generating registry" << endl;
    header_file << "int master_index = 0;" << endl;
    header_file << "map_t cached_map;" << endl;
    header_file << "map_master rogue_map_master[NUM_MAPS];" << endl;
    header_file << "npc_function_ft rogue_npc_master[NUM_MAPS];" << endl;
    header_file << "portal_function_ft rogue_portal_master[NUM_MAPS];" << endl;
    header_file << endl;

    if (DEBUG_LEVEL > 1) cout << "[M] Generating startup procedure" << endl;
    header_file << "void init_maps(){" << endl;
    for (int i = 0; i < num_maps; i++){
        header_file << "    rogue_map_master[" << i <<"] = (map_master) { " << prefix << i << "_map, " << prefix << i << "_start };" << endl;
        header_file << "    rogue_npc_master[" << i <<"] = "<< prefix << i << "_npc;" << endl;
        header_file << "    rogue_portal_master[" << i <<"] = "<< prefix << i << "_portal;" << endl;
    }

    header_file << "}" << endl << endl;

    header_file.close();

    return 0;
}

/*
Parse bitmap data
*/
int do_bitmap(){
    return 0;
}

/*
Parse quest data
*/
int do_quests(){
    char tmp;

    if (!quests_ok){
        cout << "[ ] Skiping Quests" << endl << "[ ] Done." << endl;
        return 0;
    }

    cout << "[ ] Now parsing quests.txt" << endl;
    quest_txt.open("artifact\\" + map_dir_name + "\\quests.txt");
    if (!quest_txt.is_open()) {
        cout << "ERROR: Failed to load quests.txt, exiting" << endl;
        return -14;
    }

    quest_txt >> tmp;
    if (tmp != '!'){
        cout << "ERROR: Malformed header, exiting" << endl;
        return -15;
    }

    string tstring;
    quest_txt >> tstring;
    if (tstring != "quests.txt"){
        cout << "ERROR: Malformed header, exiting" << endl;
        return -16;
    }

    quest_txt >> tmp;
    if (tmp != '!') {
        cout << "ERROR: Malformed header, exiting" << endl;
        return -17;
    }

    quest_txt >> tstring;
    if (tstring == "all_is_null") {
        if (DEBUG_LEVEL > 1) cout << "[Q] Quests are off" << endl;
        cout << "[ ] Done" << endl;
        return 0;
    } else if (tstring == "quests_on"){
        cout << "[Q] Quests are on" << endl;
    }

    int num_quests = 0;
    quest_txt >> num_quests;

    int num_text_objects = 0;
    quest_txt >> num_text_objects;

    for (int i = 0; i < num_text_objects; i++){
        int num_blocks = 0;
        quest_txt >> num_blocks;
        quest_txt >> num_blocks;
        std::string s = " ";
        for (int j = 0; j < num_blocks; j++){
            quest_txt >> s;
            quest_text_vector.push_back(s);
        }
    }

    for (int i = 0; i < num_quests; i++){
        char a;
        quest_txt >> a;
        if (a != 'Q'){
            cout << "ERROR: Bad quest definition, exiting" << endl;
            return -16;
        }

        int num_things;

    }

    return 0;
}

