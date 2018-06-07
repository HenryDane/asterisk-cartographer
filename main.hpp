#define NUM_TILES 255 // (2 ^ 8)

#define NUM_TEXTURES 512 // (2 ^ 16 )
#define NUM_TEXTURES_DEFINED 16

#define S_HEIGHT 600
#define S_WIDTH 800

extern unsigned int DEBUG_LEVEL;

typedef struct coord_t {
    int x;
    int y;
    int map_index;
};

typedef struct item_t {
    int id;
    int type;
    bool unuseable;
    char data[10];
    int data_len;
    int tex_id;
};

// updated version
typedef struct item_m_t{
    int id;
    int type;
    bool unuseable;
    std::string data;
};

typedef struct crate_t {
    int x;
    int y;
    int num_items;
    std::vector<item_t> inventory;
    int mapid;
    int id;
};

// does not yet support giving/taking items
typedef struct quest_dialogue_t {
    char * data;
    int dat_len;
    int image_id; // -1 for black, 0 for no change
};

// function pointer typedefs
typedef bool (*quest_validate_ft) (void); // checks if step is complete
typedef void (*quest_assign_ft) (void); // assigns step (may do nothing)

typedef struct quest_dialogue_block_t {
    int id; // must be positive
    quest_dialogue_t * dialogue_list; // length of list must be same as num_dialogue
    int num_dialogue; //
};

typedef struct quest_t{
    int id;
    char title[32];
    int title_len;
    char issuer[32];
    int issuer_len;
    std::vector<quest_dialogue_block_t> dialogue;
    std::vector<quest_validate_ft> validation_functions; // length of list must be same as num_stages
    std::vector<quest_assign_ft> assignment_functions; // length of list must be same as num_stages
    int num_stages;
    int reward_exp;
    int reward_credits;
    item_t reward_item;
};

typedef struct quest_active_t{
    quest_t quest;
    int block_index;
    bool complete; // if true, will be removed at some point
};

typedef struct mquest_t{
    int id;
    std::string title; // assemble to char title [len];
    std::string issuer; // assemble to char issuer [len];
    int stages;
    std::vector<int> text_address;
    std::vector<int> image_address;
    std::vector<int> verify_address;
    std::vector<int> action_address;
    int exp_reward;
    int credit_reward;
    item_t item_reward;
};

typedef struct npc_item_t {
    int id;
    int type;
    char data [16];
    int data_len; // could be one byte
    int cost;
};

typedef struct enemy_t {
    int x;
    int y;
    int map_index;
    int health;
    int type;
    bool immortal;
    int id;
    int ammunition;
};

typedef struct portal_t{
    int x;
    int y;
    int x_target;
    int y_target;
    int mapid;
    int mapid_target;
};

typedef struct npc_t {
    int id; // -1 if the npc is null
    int health; // of 1000
    int type;
    int inventory_size;
    npc_item_t inventory [16]; // make to a fixed size? npcs probably dont need to have working inventories
    bool is_merchant;
    bool is_ablaze;
    bool is_alive;
    int quest_id; // 0 for no quest, negative numbers trigger cutscene of corresponding number but positive
    int x;
    int y;
    int map_index;
};

// function pointer typedefs
typedef npc_t (*npc_function_ft) (unsigned int x, unsigned int y); // gets npc at point

typedef struct map_t {
    unsigned int w; // can be one byte
    unsigned int h; // can be one byte
    int tile_type [32 * 32]; // can be one byte (if negative then is portal to another area)
    bool on_fire [32 * 32];
    bool under_water [32 * 32];
};


typedef struct map_master {
    map_t mapdat;
    coord_t coord;
};

// flavor text
typedef struct flavor_text {
    char personnell[16];
    int weight;
    char alignment[16];
};

// for flight level view
typedef struct level_data {
    int x;
    int y;
    char data[16];
    bool persistent;
    int type;
    unsigned int num;
    bool map_exists;
    int map_id;
    int map_w;
    int map_h;
};

// for info on each tile in sector
typedef struct tile_data {
    int x;
    int y;
    char data[16];
    int num_level_data;
    const level_data * flight_data;
};

typedef struct entity {
    int x;
    int y;
    int vx;
    int vy;
    char data[16];
    int type; // 0 = station, 1 = asteroid, 2 = enemy, 3 = debris, 4 = ice station, 6 = planet, 5 = rocket (player), 6 = rocket (other)
    int id;
};

typedef struct entity_o_t{
    int id;
    int x;
    int y;
    int vx;
    int vy;
    int type;
};

extern std::vector<item_t> item_vector;
extern std::vector<npc_t> npc_vector;
extern std::vector<map_t> map_vector;
extern std::vector<crate_t> crate_vector;
extern std::vector<coord_t> start_vector;
extern std::vector<enemy_t> enemy_vector;
extern std::vector<portal_t> portal_vector;

// quest stuff
extern std::vector<std::string> quest_text_vector;
extern std::vector<mquest_t> quest_vector;

extern std::string map_dir_name;

extern sf::Color colors[32];

// fixes MinGW bug
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }
}

const std::string currentDateTime();
