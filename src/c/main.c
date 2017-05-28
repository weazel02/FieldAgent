#include <pebble.h>
#include "main.h"

//Global Variables
static Window *s_main_window;
static TextLayer *title_layer;
SimpleMenuLayer *teams_layer;
const char *possibleTeams[4] = {"HufflePuff", "Slytherin", "Gryffindor", "Ravenclaw"};


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
static void removeTitleLayer(){
  text_layer_destroy(title_layer);
}
static void loadTeamsLayer(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  simple_menu_layer_create(bounds, window_layer, const SimpleMenuSection * sections, int32_t num_sections, void * callback_context)
  
  
}
  


//Handler Functions
static void main_window_load(Window *window) {
  loadTitleLayer();

}

static void main_window_unload(Window *window) {
  removeTitleLayer();

}





static void init() {
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

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

