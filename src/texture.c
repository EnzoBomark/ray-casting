
#include "texture.h"

static const char* texture_paths[NUM_TEXTURES] = {
  // walls
  "./assets/textures/redbrick.png",     // 0
  "./assets/textures/purplestone.png",  // 1
  "./assets/textures/mossystone.png",   // 2
  "./assets/textures/graystone.png",    // 3
  "./assets/textures/colorstone.png",   // 4
  "./assets/textures/bluestone.png",    // 5
  "./assets/textures/wood.png",         // 6
  "./assets/textures/eagle.png",        // 7

  // sprites
  "./assets/sprites/armor.png",   // 8
  "./assets/sprites/barrel.png",  // 9
  "./assets/sprites/guard.png",   // 10
  "./assets/sprites/light.png",   // 11
  "./assets/sprites/table.png",   // 12

  // handgun
  "./assets/weapons/handgun-1.png", // 13
  "./assets/weapons/handgun-2.png", // 14
  "./assets/weapons/handgun-3.png", // 15
  "./assets/weapons/handgun-4.png", // 16
  "./assets/weapons/handgun-5.png", // 17
};

void load_textures(void) {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_t* upng = upng_new_from_file(texture_paths[i]);
    if (upng != NULL) {
      upng_decode(upng);

      if (upng_get_error(upng) == UPNG_EOK) {
        textures[i] = upng;
      } else {
        printf("Error decoding texture %s \n", texture_paths[i]);
      }
    }

    else {
      printf("Error loading texture %s \n", texture_paths[i]);
    }
  }

}

void free_textures(void) {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_free(textures[i]);
  }
}
