// Copyright (c) 2015 Douglas Otwell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include <pebble.h>
#include "goto.h"
#include "common.h"
#include "basic_menu_layer.h"

static Window *s_window;
static MenuLayer *s_menu;
static GBitmap *s_icon_full;
static GBitmap *s_icon_info;
static GBitmap *s_icon_home;
static GBitmap *s_icon_music;
static GBitmap *s_icon_video;
static GBitmap *s_icon_pics;
static GBitmap *s_icon_weather;
static BasicMenuModel *s_model;

static void menu_select_cb(MenuLayer *menu_layer, MenuIndex *cell_index, BasicMenuItem *item) {
  char *value = item->value;
  outbox_send(value);
  window_stack_pop(true);
}

static void initialize_ui(void) {
  s_icon_full = gbitmap_create_with_resource(RESOURCE_ID_ICON_FULL);
  s_icon_info = gbitmap_create_with_resource(RESOURCE_ID_ICON_INFO);
  s_icon_home = gbitmap_create_with_resource(RESOURCE_ID_ICON_HOME);
  s_icon_music = gbitmap_create_with_resource(RESOURCE_ID_ICON_PLAYER);
  s_icon_video = gbitmap_create_with_resource(RESOURCE_ID_ICON_VIDEO);
  s_icon_pics = gbitmap_create_with_resource(RESOURCE_ID_ICON_PICS);
  s_icon_weather = gbitmap_create_with_resource(RESOURCE_ID_ICON_WEATHER);

  s_window = window_create();
  window_set_fullscreen(s_window, false);
  window_set_background_color(s_window, GColorBlack);
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_frame(window_layer);

  s_model = basic_menu_model_create();
  basic_menu_model_add_item(s_model, "Fullscreen", s_icon_full, "GO_FULL");
  basic_menu_model_add_item(s_model, "Info", s_icon_info, "GO_INFO");
  basic_menu_model_add_item(s_model, "Home", s_icon_home, "GO_HOME");
  basic_menu_model_add_item(s_model, "Music", s_icon_music, "GO_MUSIC");
  basic_menu_model_add_item(s_model, "Video", s_icon_video, "GO_VIDEO");
  basic_menu_model_add_item(s_model, "Pictures", s_icon_pics, "GO_PICS");
  basic_menu_model_add_item(s_model, "Weather", s_icon_weather, "GO_WEATHER");
  
  s_menu = basic_menu_layer_create(bounds, s_model);
  basic_menu_model_set_select_callback(s_model, menu_select_cb);
  
  menu_layer_set_click_config_onto_window(s_menu, s_window);
  layer_add_child(window_layer, menu_layer_get_layer(s_menu));
}

static void destroy_ui() {
  window_destroy(s_window);
  basic_menu_layer_destroy(s_menu);
  basic_menu_model_destroy(s_model);
  gbitmap_destroy(s_icon_full);
  gbitmap_destroy(s_icon_info);
  gbitmap_destroy(s_icon_home);
  gbitmap_destroy(s_icon_music);
  gbitmap_destroy(s_icon_video);
  gbitmap_destroy(s_icon_pics);
  gbitmap_destroy(s_icon_weather);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_goto(void) {
  initialize_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_goto(void) {
  window_stack_remove(s_window, true);
}