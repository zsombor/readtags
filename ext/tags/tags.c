#include "tags_ext.h"

VALUE rb_mTags;

void Init_tags()
{
    rb_mTags = rb_define_module("Tags");
    Init_tags_file();
}
