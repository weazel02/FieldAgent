#include <pebble.h>
#include "main.h"
#include "src/c/key_assembly.h"
#include <stdio.h>
#include <string.h>

//Created by: Wesley Thompson
//FieldAgent.c

//Global Variables
static Window *s_main_window;
static Window *names_window;
static TextLayer *title_layer;
SimpleMenuLayer *teams_layer;
SimpleMenuLayer *names_layer;
char *possibleTeams[5] = {"Choose a team!","HufflePuff", "Slytherin", "Gryffindor", "Ravenclaw"};
char *possibleNames[5] = {"Choose a name!","Draco", "Harry", "Luna", "Cedric"};
char* teamId = NULL;
char* playerId;
char* pebbleId;
char* gameId;
static char *fa_claim = "opCode=FA_CLAIM|"
                        "gameId=FEED|"
                        "pebbleId=8080477D|"
                        "team=aqua|"
                        "player=Alice|"
                        "latitude=43.706552|"
                        "longitude=-72.287418|"
                        "kragId=8080";
char* numClaimed;
char* numKrags;
char* currLat;
char* currLong;
int secondsPassed;
static ActionBarLayer *s_action_bar;
static GBitmap *s_up_bitmap, *s_down_bitmap, *s_check_bitmap;
char* currLocationMessage;
//bag_t hints



//Static Function Definitions 
static char* concat(const char *s1, const char *s2);
static char* createLocationMessage();
static void loadNamesLayer();
static void loadTeamsLayer();
static void loadTitleLayer();
static void loadGameOverLayer();
static void removeTitleLayer();
static void removeNamesLayer();
static void removeTeamsLayer();
static void teamItemClickCallback();
static void nameItemClickCallback();
static void init();
static void deinit();
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static char* createLocationMessage();
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void handleMessage(char* msg);
static void parseLocation(char* location);
SimpleMenuSection sec1[1];
SimpleMenuItem sectionItems1[4];
//static void update_time();

// AppMessage functions
static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context);

// Request and Send Message Functions 
static void request_pebbleId();
static void request_location();
static void send_message(char *message);






//Functions to remove layers from the windows
//
//
//
//
static void removeTitleLayer(){
  text_layer_destroy(title_layer);
}
static void removeTeamsLayer(){
  if(teams_layer){
     //layer_set_hidden((Layer*)menu_layer_get_layer(simple_menu_layer_get_menu_layer(teams_layer)), true);
  layer_remove_from_parent(simple_menu_layer_get_layer(teams_layer));  
  //simple_menu_layer_destroy(teams_layer);
  teams_layer = NULL;
  }
}

//Callback Functions
//
//
//
//
static void teamItemClickCallback(){
  if(teamId == NULL){
    int index = simple_menu_layer_get_selected_index(teams_layer);
    teamId = possibleTeams[index];
   // APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", teamId);
    printf(teamId);
    removeTeamsLayer();
    printf("Before loadNamesLayer()");
    loadNamesLayer();
    printf("After loadNamesLayer()");
  
  }
}
static void nameItemClickCallback(){
    int index = simple_menu_layer_get_selected_index(names_layer);
    playerId = possibleNames[index];
    APP_LOG(APP_LOG_LEVEL_DEBUG, "nameId: %s", playerId);
  
  // Load icon bitmaps
//s_up_bitmap = gbitmap_create_with_resource(NULL);
//s_down_bitmap = gbitmap_create_with_resource(NULL);
//s_check_bitmap = gbitmap_create_with_resource(NULL);

// Create ActionBarLayer
s_action_bar = action_bar_layer_create();
//action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);

// Set the icons
action_bar_layer_set_icon(s_action_bar, BUTTON_ID_UP, s_up_bitmap);
action_bar_layer_set_icon(s_action_bar, BUTTON_ID_DOWN, s_down_bitmap);
action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_check_bitmap);

// Add to Window
action_bar_layer_add_to_window(s_action_bar, s_main_window);
removeTeamsLayer();
  

}


//Functions to load layers to the window
//
//
//
//
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
static void loadTitleLayerRed(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  title_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(title_layer, GColorRed);
  text_layer_set_text_color(title_layer, GColorBlack);
  text_layer_set_text(title_layer, "Project Krag");
  text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  GSize title_size = text_layer_get_content_size(title_layer);
 
  text_layer_set_size(title_layer, title_size);
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(title_layer));
}
static void loadTitleLayerGreen(){
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  title_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(title_layer, GColorGreen);
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
    sectionItems[i] = (SimpleMenuItem){.title=possibleTeams[i], .callback=(SimpleMenuLayerSelectCallback) teamItemClickCallback};
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
  layer_add_child(window_layer, simple_menu_layer_get_layer(names_layer));
}






//Handler Functions
//
//
//
//
static void main_window_load(Window *window) {
  /*
  printf("Window load called");
  gameId = "feed101";
  teamId = "Ravenclaw";
  playerId = "Draco";
  pebbleId = "D808";
  currLat = "-73.4566";
  currLong = "112.3234";
  */
  //const char* test = "opcode=FA_LOCATION|gameId=feed101|pebbleId=D808|team=Ravenclaw|player=Draco|latitude=-73.4566|longitude=112.3234|statusReq=0";
 
  
  
}

static void main_window_unload(Window *window) {
  //removeTitleLayer();
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  if(secondsPassed == 15){
    //requestLocation();
    //char *location_message = createLocationMessage();
    //sendMessage(location_message);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Time to Send Message!");
    secondsPassed = 0;
  }
  secondsPassed ++;
}



// Main Initialization Functions
//
//
//
//

static void init() {
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
   
   //loadNamesLayer();
  // Show the Window on the watch, with animated=true

  //loadTeamsLayer();
  loadTitleLayer();
  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  
  
   //Set the handlers for AppMessage inbox/outbox events. Set these    *
  //handlers BEFORE calling open, otherwise you might miss a message. */
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);

    //open the app message communication protocol. Request as much space *
    //as possible, because our messages can be quite large at times.     */
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  
  request_pebbleId();
  request_location();
  if(pebbleId!=NULL && currLat != NULL && currLong != NULL){
    loadTitleLayerGreen();
  }else{
    loadTitleLayerRed();
  }
  printf("end of init");

}

static void deinit() {
  // Destroy Window
  printf("deinit called. b4 window destruction");
  window_destroy(s_main_window);
  printf("deinit: after window destruction");
  tick_timer_service_unsubscribe();
}
//AppMessage Callback Functions
//
//
//
//
  
// inbox_received_callback
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    /* Possible message types defined in key_assembly.h
    enum {
      AppKeyJSReady = 0,      // The JS environment on the companion smartphone is up and running!
      AppKeySendMsg,          // Send a message over Bluetooth to the companion smartphone and then on to the Game Server
      AppKeyRecvMsg,          // A message from the Game Server is available (arrived over Bluetooth)
      AppKeyLocation,         // Request your GPS location from the companion smartphone
      AppKeyPebbleId,         // Request your unique pebble ID from the companion smartphone
      AppKeySendError         // Error: companion app can't connect to the Proxy (and ultimately the Game Server).
    };
    */

    /* 1. Check to see if JS environment ready message received. */
    Tuple *ready_tuple = dict_find(iterator, AppKeyJSReady);
    if(ready_tuple) {
        // Log the value sent as part of the received message.
        char *ready_str = ready_tuple->value->cstring;
        APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeyJSReady: %s\n", ready_str);
    }

    /* 2. Check to see if an opcode message was received received. */
    Tuple *msg_tuple = dict_find(iterator, AppKeyRecvMsg);
    if(msg_tuple) {
        // Log the value sent as part of the received message.
        char *msg = msg_tuple->value->cstring;
        APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeyrecvMsg: %s\n", msg);
        handleMessage(msg);
      
      //Parse Message 
      //Handle UI Changes 
    }

    /* 3. Check to see if a location message received. */
    Tuple *loc_tuple = dict_find(iterator, AppKeyLocation);
    if(loc_tuple) {
        // Log the value sent as part of the received message.
        char *location = loc_tuple->value->cstring;
        APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeyLocation: %s\n", location);
        parseLocation(location);
      
      //Parse Location lat|long
      //Store in globals
    }

    /* 4. Check to see if a PebbleId message received. */
    Tuple *id_tuple = dict_find(iterator, AppKeyPebbleId);
    if(id_tuple) {
        // Log the value sent as part of the received message.
        char *pebbId = id_tuple->value->cstring;
        pebbleId = pebbId;
        APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeyPebbleId: %s\n", pebbleId);
      
      //Store in global
    }

    /* 5. Check to see if an error message was received. */
    Tuple *error_tuple = dict_find(iterator, AppKeyLocation);
    if(error_tuple) {
        // Log the value sent as part of the received message.
        char *error = error_tuple->value->cstring;
       APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeySendError: %s\n", error);
    }
}

// outbox_sent_callback
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Message successfully sent.\n");
}

// inbox_dropped_callback
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Message dropped from inbox.\n");
}

// outbox_failed_callback
static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Message failed to send.\n");
}
//Functions to request data from the smartphone
//
//
//
//

// request_pebbleId
static void request_pebbleId() {
    /* 1. Declare dictionary iterator */
    DictionaryIterator *out_iter;

    /* 2. Prepare the outbox */
    AppMessageResult result = app_message_outbox_begin(&out_iter);

    /* 3. If outbox was prepared, send request. Otherwise, log error. */
    if (result == APP_MSG_OK) {
        int value = 1;

        /* construct and send message, outgoing value ignored */
        dict_write_int(out_iter, AppKeyPebbleId, &value, sizeof(value), true);

        result = app_message_outbox_send();

        if (result != APP_MSG_OK) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending pebbleId request from outbox.\n");
        }
    } else {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox for pebbleId request.\n");
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "Requested pebbleId.\n");
}

// request_location
static void request_location() {
    /* 1. Declare dictionary iterator */
    DictionaryIterator *out_iter;

    /* 2. Prepare the outbox */
    AppMessageResult result = app_message_outbox_begin(&out_iter);

    /* 3. If outbox was prepared, send request. Otherwise, log error. */
    if (result == APP_MSG_OK) {
        int value = 1;

        /* construct and send message, outgoing value ignored */
        dict_write_int(out_iter, AppKeyLocation, &value, sizeof(value), true);

        result = app_message_outbox_send();

        if (result != APP_MSG_OK) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending locationrequest from outbox.\n");
        }
    } else {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox for location request.\n");
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "Requested location.\n");
}

//Function to send message to the server
//
//
//
//
static void send_message(char *message) {
    /* 1. Declare dictionary iterator */
    DictionaryIterator *out_iter;

    /* 2. Prepare the outbox */
    AppMessageResult result = app_message_outbox_begin(&out_iter);

    /* 3. If outbox was prepared, send message. Otherwise, log error. */
    if (result == APP_MSG_OK) {
      
        /* Construct and send the message */
        dict_write_cstring(out_iter, AppKeySendMsg, message);
      
        result = app_message_outbox_send();
      
        if(result != APP_MSG_OK) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending message from outbox.\n");
        }
    } else {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox for send_message.\n");
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "Sent message.\n");
}
//Function to handle incoming messages 
//
//
//
//
static void handleMessage(char* msg){
  
  //Parse Msg
  //set_t parsedMsgSet;
  //if no error
  //char* opcode = set_find(parsedMsgSet,"opcode");
  char *opcode = "opcode";
  char ga_hint[8] = ("GA_HINT");
  ga_hint[7] = '\0';
  char game_status[12] = ("GAME_STATUS");
  game_status[11] = '\0';
  char game_over[10] = ("GAME_OVER");
  game_over[9] = '\0';
  char gs_response[12] = ("GS_RESPONSE");
  gs_response[11] = '\0';
  char invalid_message[25] = ("SH_ERROR_INVALID_MESSAGE");
  invalid_message[24] = '\0';
  char invalid_opcode[24] = ("SH_ERROR_INVALID_OPCODE");
  invalid_opcode[23] = '\0';
  char invalid_field[23] = ("SH_ERROR_INVALID_FIELD");
  invalid_field[22] = '\0';
  char invalid_gameId[25] = ("SH_ERROR_INVALID_GAME_ID");
  invalid_gameId[24] = '\0';
  char invalid_teamname[26] = ("SH_ERROR_INVALID_TEAMNAME");
  invalid_teamname[25] = '\0';
  char invalid_playername[28] = ("SH_ERROR_INVALID_PLAYERNAME");
  invalid_playername[27] = '\0';
  char invalid_id[20] = ("SH_ERROR_INVALID_ID");
  invalid_id[19] = '\0';
  char claimed[11] = ("SH_CLAIMED");
  claimed[10] = '\0';
  char claimed_already[19] = ("SH_CLAIMED_ALREADY");
  claimed_already[18] = '\0';
  
  if(strcmp(ga_hint,opcode) == 0){
    char *hint = "hint";
    // char *hint = set_find(parsedMsgSet,"hint")
    // bag_insert(hints, hint);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Hint Received. Hint: %s", hint);
  }
  if(strcmp(game_status,opcode) == 0){
    char * numClaimed = "5";
    char * numKrags = "10";
    //numClaimed = set_find(parsedMsgSet, "numClaimed");
    //numKrags = set_find(parsedMsgSet, "numKrags");
     APP_LOG(APP_LOG_LEVEL_DEBUG, "numClaimed: %s, numKrags: %s", numClaimed,numKrags);
  }
  if(strcmp(game_over,opcode) == 0){
    //loadGameOverLayer();
  }
  
  if(strcmp(gs_response,opcode) == 0){
    char* respCode = "respCode";
    //respCode = set_find(parsedMsgSet, "respCode");
    
    if(strcmp(invalid_message,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Message!");
    }
    if(strcmp(invalid_opcode,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Opcode!");
    }
    if(strcmp(invalid_field,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Field!");
    }
    if(strcmp(invalid_gameId,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid GameId!");
    }
    if(strcmp(invalid_teamname,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Teamname!");
    }
    if(strcmp(invalid_playername,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Playername!");
    }
    if(strcmp(invalid_id,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Id!");
    }
    if(strcmp(claimed,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Krag Claimed!");
    }
    if(strcmp(claimed_already,respCode) == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Krag Claimed Already!");
    }
  }
}  

//Helper Functions
//
//
//
//
static void parseLocation(char* location){
  char *token_location = location;
  
  while (*token_location != '|')
    token_location++;
  
  int lat_length = (token_location - location);
  location[lat_length] = '\0';
  currLat = location;
  
  //char *new_latitude = malloc(lat_length + 1);
  //new_latitude[lat_length] = '\0';
  //strncpy(new_latitude, location, lat_length);
  //currLat = new_latitude;
  currLong = token_location + 1;
  printf(currLat);
  printf(currLong);
  
}
  
  



static char* createLocationMessage(){

  char *loc_message = calloc(200, sizeof(char));
  
  APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);  
  
  loc_message = strcat(loc_message,"opcode=FA_LOCATION|gameId=");
  loc_message = strcat(loc_message,gameId);
   //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
   printf(loc_message);
  loc_message = strcat(loc_message, "|pebbleId=");
  //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, pebbleId);
   //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, "|team=");
  //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, teamId);
   //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, "|player=");
   //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, playerId);
  //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, "|latitude=");
  // APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message =strcat(loc_message, currLat);
  //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message = strcat(loc_message, "|longitude=");
   //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message = strcat(loc_message, currLong);
  //APP_LOG(APP_LOG_LEVEL_DEBUG,loc_message);
  loc_message = strcat(loc_message, "|statusReq=0");
  printf(loc_message);

  //currLocationMessage = "";
  //currLocationMessage = strcat(currLocationMessage, loc_message);

  
  return loc_message;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
//MAIN
//
//
//
//
int main(void) {
  init();
  printf("b4 app event loop");
  app_event_loop();
  printf("after app event loop");
  deinit();
}

