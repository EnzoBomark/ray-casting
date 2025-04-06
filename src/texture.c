
#include "texture.h"

static const char* texture_paths[NUM_TEXTURES] = {
  // walls
  "./images/redbrick.png",     // 1
  "./images/purplestone.png",  // 2
  "./images/mossystone.png",   // 3
  "./images/graystone.png",    // 4
  "./images/colorstone.png",   // 5
  "./images/bluestone.png",    // 6
  "./images/wood.png",         // 7
  "./images/eagle.png",        // 8

  // sprites
  "./images/armor.png",   // 9
  "./images/barrel.png",  // 10
  "./images/guard.png",   // 11
  "./images/light.png",   // 12
  "./images/table.png",   // 13
};

void load_textures(void) {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    upng_t* upng = upng_new_from_file(texture_paths[i]);
    if (upng != NULL) {
      upng_decode(upng);

      if (upng_get_error(upng) == UPNG_EOK) {
        textures[i] = upng;
      }
      else {
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
