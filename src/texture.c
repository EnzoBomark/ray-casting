
#include "texture.h"

static const char* texture_paths[NUM_TEXTURES] = {
    "./images/redbrick.png",
    "./images/purplestone.png",
    "./images/mossystone.png",
    "./images/graystone.png",
    "./images/colorstone.png",
    "./images/bluestone.png",
    "./images/wood.png",
    "./images/eagle.png",
};

void load_wall_textures(void) {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng;
        upng = upng_new_from_file(texture_paths[i]);
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                wall_textures[i].upng_texture = upng;
                wall_textures[i].width = upng_get_width(upng);
                wall_textures[i].height = upng_get_height(upng);
                wall_textures[i].texture_buffer = (color_t*)upng_get_buffer(upng);
            }
        }
    }

}

void free_wall_textures(void) {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_free(wall_textures[i].upng_texture);
    }
}
