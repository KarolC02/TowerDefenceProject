#pragma once

// Here we define some constants that work well and we dont want to change, mainly the sizes of UI, towers etc

enum class TowerType {
    None,
    Pellet,
    Bash,
    Squirt,
    Frost,
    Swarm,
    Dart
};

/*\
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
                                        COLORS
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
#define PELLET_TOWER_COLOR sf::Color(105, 105, 105)    // Dim Gray
#define SWARM_TOWER_COLOR sf::Color(153, 76, 0)        // Dark Orange (for more distinction)
#define BASH_TOWER_COLOR sf::Color(139, 0, 0)          // Dark Red (Scarlet Dark Red)
#define FROST_TOWER_COLOR sf::Color(70, 130, 180)      // Steel Blue (Dark Ice Blue)
#define DART_TOWER_COLOR sf::Color(60, 179, 113)       // Medium Sea Green (for more distinction)
#define SQUIRT_TOWER_COLOR sf::Color(119, 136, 153)    // Light Slate Gray
#define GRAY sf::Color(128, 128, 128)
#define PINK sf::Color(255, 192, 203)
#define BACKGROUND_FRAME_COLOR sf::Color(160, 160, 160, 200)
#define INFOBAR_COLOR sf::Color(121, 25, 87, 200)
#define TOWER_INFO_BAR_COLOR sf::Color(22, 200, 187, 200)

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
                                    SIZES, proportions
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
#define TOWER_SIDE_LENGTH 40
#define GRID_WIDTH 14
#define GRID_HEIGHT 12
#define GRID_CELL_SIZE (TOWER_SIDE_LENGTH / 2)
#define ARENA_WIDTH GRID_WIDTH * TOWER_SIDE_LENGTH  // Example width, adjust as needed
#define ARENA_HEIGHT GRID_HEIGHT * TOWER_SIDE_LENGTH // Example height, adjust as needed
#define WINDOW_WIDTH ARENA_WIDTH*2  // Example width, adjust as needed
#define WINDOW_HEIGHT ARENA_HEIGHT*2 // Example height, adjust as needed
#define UI_WIDTH 200
#define LEFT_OFFSET TOWER_SIDE_LENGTH*2// Example offsets from the left and top edges of the window
#define TOP_OFFSET TOWER_SIDE_LENGTH*4
#define INFOBAR_HEIGHT 50
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
                                mazing parameters
 
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
#define startX 0
#define startY 11
#define endX 27
#define endY 11


#define CONFIG_PATH "/Users/karol/desktop/Tower_defence/Tower_defence/Resources/config.json"
#define TOWER_PARAMETERS_PATH "/Users/karol/desktop/Tower_defence/Tower_defence/Resources/towers.json"
