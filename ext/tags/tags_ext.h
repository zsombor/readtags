#ifndef __TAGS_RUBY_EXT_H___
#define __TAGS_RUBY_EXT_H___

#include <ruby.h>
#include <assert.h>

#include "readtags.h"

typedef struct {
    tagFile *file;
    int flags;
    VALUE file_name;
    tagFileInfo info;
    tagEntry entry;
    tagResult last_result;
} tag_file;


void Init_tags_file();

#endif
