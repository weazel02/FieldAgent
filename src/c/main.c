#include <pebble.h>
#include "main.h"

//Global Variables
static Window *s_main_window;
static TextLayer *title_layer;
SimpleMenuLayer *teams_layer;
SimpleMenuLayer *names_layer;
char *possibleTeams[5] = {"Choose a team!","HufflePuff", "Slytherin", "Gryffindor", "Ravenclaw"};
char *possibleNames[5] = {"Choose a name!","Draco", "Harry", "Luna", "Cedric"};
char* teamId = NULL;
char* nameId;

static void loadNamesLayer();
static void loadTeamsLayer();
static void loadTitleLayer();
static void removeTitleLayer();
static void removeNamesLayer();
static void removeTeamsLayer();
static void teamItemClickCallback();
static void nameItemClickCallback();




//Function to remove layers from the windows
static void removeTitleLayer(){
  text_layer_destroy(title_layer);
}
static void removeTeamsLayer(){
  if(teams_layer){
     //layer_set_hidden((Layer*)menu_layer_get_layer(simple_menu_layer_get_menu_layer(teams_layer)), true);
  layer_remove_from_parent(simple_menu_layer_get_layer(teams_layer));  
  simple_menu_layer_destroy(teams_layer);
  teams_layer = NULL;
  }
}

//Callback Functions
static void teamItemClickCallback(){
  if(teamId == NULL){
  int index = simple_menu_layer_get_selected_index(teams_layer);
  teamId = possibleTeams[index];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", teamId);
  removeTeamsLayer();
  loadTitleLayer();
  loadNamesLayer();
  }
}
static void nameItemClickCallback(){
    int index = simple_menu_layer_get_selected_index(names_layer);
    nameId = possibleNames[index];
    APP_LOG(APP_LOG_LEVEL_DEBUG, "nameId: %s", nameId);

}


//Functions to load layers to the window

static void loadTitleLayer(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  title_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(title_layer, GColorClear);
  text_layer_set_text_color(title_layer, GColorBlack);
  text_layer_set_text(title_layer, "Project Krag");
  text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  GSize title_size = text_layer_get_content_size(title_layer);
 
  text_layer_set_size(title_layer, title_size);
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(title_layer));
}

static void loadTeamsLayer(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_frame(window_layer);
    
  SimpleMenuSection sec[1];
  SimpleMenuItem sectionItems[4];
  
  for(int i = 0 ; i < 5; i++){
    if(i >= 1){
    sectionItems[i] = (SimpleMenuItem){.title=possibleTeams[i], .callback=(SimpleMenuLayerSelectCallback) &teamItemClickCallback};
    }else {
      sectionItems[i] = (SimpleMenuItem){.title=possibleTeams[i]};
    }
  }
  sec[0] = (SimpleMenuSection) {.items = sectionItems, .num_items = 5}; 
  teams_layer = simple_menu_layer_create(bounds, s_main_window, sec, 1, teamItemClickCallback);
  layer_add_child(window_layer, simple_menu_layer_get_layer(teams_layer));
}
static void loadNamesLayer(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  layer_remove_child_layers(window_layer);
  GRect bounds = layer_get_frame(window_layer);
    
  SimpleMenuSection sec[1];
  SimpleMenuItem sectionItems[4];
  
  for(int i = 0 ; i < 5; i++){
    if(i >= 1){
    sectionItems[i] = (SimpleMenuItem){.title=possibleNames[i], .callback=(SimpleMenuLayerSelectCallback) &nameItemClickCallback};
    }else {
      sectionItems[i] = (SimpleMenuItem){.title=possibleNames[i]};
    }
  }
  sec[0] = (SimpleMenuSection) {.items = sectionItems, .num_items = 5}; 
  names_layer = simple_menu_layer_create(bounds, s_main_window, sec, 1, nameItemClickCallback);
  layer_add_child(window_layer, menu_layer_get_layer(simple_menu_layer_get_menu_layer(names_layer)));
}






//Handler Functions
static void main_window_load(Window *window) {
 
  //loadTeamsLayer();
  //removeTeamsLayer();
  //loadTitleLayer();

}

static void main_window_unload(Window *window) {
  //removeTitleLayer();
 

}





// Main Initialization Functions 

static void init() {
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
   loadTeamsLayer();
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

}

static void deinit() {
  
  // Destroy Window
  window_destroy(s_main_window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

