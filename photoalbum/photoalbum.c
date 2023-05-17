#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoalbum.h"

Photo *create_photo(int id, const char *description) {
    Photo *new_photo = malloc(sizeof(Photo));
    if (!new_photo) {
        return NULL;
    } else {
        new_photo->id = id;
        if (description) {
            new_photo->description = malloc(strlen(description) + 1);
            strcpy(new_photo->description, description);
        } else {
            new_photo->description = NULL;
        }
        return new_photo;
    }
}

void print_photo(Photo *photo) {
    if (photo) {
        printf("Photo Id: %d, Description: ", photo->id);
        if (!photo->description) {
            printf("None\n");
        } else {
            printf("%s\n", photo->description);
        }
    }
}

void destroy_photo(Photo *photo) {
    if (photo) {
        free(photo->description);
        free(photo);
    }
}

void initialize_album(Album *album) {
    if (album) {
        album->size = 0;
    }
}

void print_album(const Album *album) {
    if (album) {
        if (album->size == 0) {
            printf("Album has no photos.\n");
        } else {
            int i;
            for (i = 0; i < album->size; i++) {
                print_photo(album->all_photos[i]);
            }
        }
    }
}

void destroy_album(Album *album) {
    if (album) {
        int i;
        for (i = 0; i < album->size; i++) {
            destroy_photo(album->all_photos[i]);
        }
        album->size = 0;
    }
}

void add_photo_to_album(Album *album, int id, const char *description) {
    if (album) {
        if (album->size < MAX_ALBUM_SIZE) {
            Photo *new_photo = create_photo(id, description);
            if (new_photo) {
                album->all_photos[album->size++] = new_photo;
            }
        }
    }
}
