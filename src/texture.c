
#include "texture.h"

static const char* texture_paths[NUM_TEXTURES] = {
  // walls
  "./assets/images/redbrick.png",     // 1
  "./assets/images/purplestone.png",  // 2
  "./assets/images/mossystone.png",   // 3
  "./assets/images/graystone.png",    // 4
  "./assets/images/colorstone.png",   // 5
  "./assets/images/bluestone.png",    // 6
  "./assets/images/wood.png",         // 7
  "./assets/images/eagle.png",        // 8

  // sprites
  "./assets/images/armor.png",   // 9
  "./assets/images/barrel.png",  // 10
  "./assets/images/guard.png",   // 11
  "./assets/images/light.png",   // 12
  "./assets/images/table.png",   // 13
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
