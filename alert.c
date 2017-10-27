#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

static int button_event = 0, up = 1, down = 2, select = 3;

static void in_recieved_handler(DictionaryIterator *iter, void *context){
  
}

void send_int(uint8_t key, uint8_t cmd){
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
      
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
      
    app_message_outbox_send();
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    vibes_double_pulse();
    send_int(button_event, select);
}
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    vibes_double_pulse();
    send_int(button_event, up);
}
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    vibes_double_pulse();
    send_int(button_event, down);
}
static void config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

static void init(void) {  
	// Create a window and get information about the window
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
	
  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
	text_layer_set_text(s_text_layer, "Click to Send");
  
  // Set the font and text alignment
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);

  
  window_set_click_config_provider(s_window, (ClickConfigProvider)config_provider);
  app_message_register_inbox_received(in_recieved_handler);
  app_message_open(512, 512);
	// Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
