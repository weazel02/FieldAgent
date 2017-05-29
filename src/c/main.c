#include <pebble.h>
#include "main.h"
#include "src/c/key_assembly.h"
#include <stdio.h>
#include <string.h>

//Global Variables
static Window *s_main_window;
static TextLayer *title_layer;
SimpleMenuLayer *teams_layer;
SimpleMenuLayer *names_layer;
char *possibleTeams[5] = {"Choose a team!","HufflePuff", "Slytherin", "Gryffindor", "Ravenclaw"};
char *possibleNames[5] = {"Choose a name!","Draco", "Harry", "Luna", "Cedric"};
char* teamId = NULL;
char* nameId;
char*pebbleId;
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
//bag_t hints



//Static Function Definitions 
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
//static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
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
  
  
   //Set the handlers for AppMessage inbox/outbox events. Set these    *
  //handlers BEFORE calling open, otherwise you might miss a message. */
    app_message_register_inbox_received(inbox_received_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);

    //open the app message communication protocol. Request as much space *
    //as possible, because our messages can be quite large at times.     */
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

}

static void deinit() {
  
  // Destroy Window
  window_destroy(s_main_window);

}

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
      
      //Parse Message 
      //Handle UI Changes 
    }

    /* 3. Check to see if a location message received. */
    Tuple *loc_tuple = dict_find(iterator, AppKeyLocation);
    if(loc_tuple) {
        // Log the value sent as part of the received message.
        char *location = loc_tuple->value->cstring;
        APP_LOG(APP_LOG_LEVEL_INFO, "Got AppKeyLocation: %s\n", location);
      
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

// send_message
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

static void handleMessage(char* msg){
  
  //Parse Msg
  //set_t parsedMsgSet;
  //if no error
  //char* opcode = set_find(parsedMsgSet,"opcode");
  char *opcode;
  char ga_hint[8] = ("GA_HINT");
  char game_status[12] = ("GAME_STATUS");
  char game_over[10] = ("GAME_OVER");
  char gs_response[12] = ("GS_RESPONSE");
  char invalid_message[25] = ("SH_ERROR_INVALID_MESSAGE");
  char invalid_opcode[24] = ("SH_ERROR_INVALID_OPCODE");
  char invalid_field[23] = ("SH_ERROR_INVALID_FIELD");
  char invalid_gameId[25] = ("SH_ERROR_INVALID_GAME_ID");
  char invalid_teamname[26] = ("SH_ERROR_INVALID_TEAMNAME");
  char invalid_playername[28] = ("SH_ERROR_INVALID_PLAYERNAME");
  char invalid_id[20] = ("SH_ERROR_INVALID_ID");
  char claimed[11] = ("SH_CLAIMED");
  char claimed_already[19] ("SH_CLAIMED_ALREADY");
  
  if(strcmp(ga_hint,opcode) == 0){
    char *hint;
    // char *hint = set_find(parsedMsgSet,"hint")
    // bag_insert(hints, hint);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Hint Received. Hint: %s", hint);
  }
  if(strcmp(game_status,opcode) == 0){
    //numClaimed = set_find(parsedMsgSet, "numClaimed");
    //numKrags = set_find(parsedMsgSet, "numKrags");
     APP_LOG(APP_LOG_LEVEL_DEBUG, "numClaimed: %s, numKrags: %s", numClaimed,numKrags);
  }
  if(strcmp(game_over,opcode) == 0){
    loadGameOverLayer();
  }
  
  if(strcmp(gs_response,opcode) == 0){
    char* respCode;
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
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

