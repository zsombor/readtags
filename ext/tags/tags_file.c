#include "tags_ext.h"

static void rb_tags_file_free(tag_file *tags)
{
   tagsClose(tags->file);
   tags->file = NULL;
}

static void rb_tags_file_mark(tag_file *tags)
{
    rb_gc_mark(tags->file_name);
}


static VALUE rb_tags_file_allocate(VALUE klass)
{
    tag_file *tags = malloc(sizeof(tag_file));
    if(tags == NULL){
        rb_raise(rb_eNoMemError, "out of memory");
    }

    memset(tags, sizeof(tag_file), 0);
    tags->file_name = rb_str_buf_new2("");

    return Data_Wrap_Struct(klass, &rb_tags_file_mark, &rb_tags_file_free, tags);
}


static inline int option_or_default_int(VALUE hash, const char* name, int default_val)
{
    VALUE rb_value;
    rb_value = rb_hash_aref(hash, ID2SYM(rb_intern(name)));
    if(FIXNUM_P(rb_value)) {
        return NUM2INT(rb_value);
    }
    return default_val;
}


static VALUE rb_tags_file_init(VALUE self, VALUE tag_file_name)
{
    tag_file *tags;
    char *file_name;
    Data_Get_Struct(self, tag_file, tags);

    Check_Type(tag_file_name, T_STRING);
    tags->file_name = tag_file_name;
    file_name = StringValueCStr(tag_file_name);

    tags->file = tagsOpen(file_name, &tags->info);
    if(tags->file == NULL){
        rb_raise(rb_eRuntimeError, "Could not open '%s' tags file!", file_name);
    }
    return Qnil;
}

static VALUE sym_name, sym_file, sym_line_pattern, sym_line_number, sym_kind, sym_file_scope, sym_ext;
static void init_symbols()
{
    sym_name = ID2SYM(rb_intern("name"));
    sym_file = ID2SYM(rb_intern("file"));
    sym_line_pattern = ID2SYM(rb_intern("line_pattern"));
    sym_line_number = ID2SYM(rb_intern("line_number"));
    sym_kind = ID2SYM(rb_intern("kind"));
    sym_file_scope = ID2SYM(rb_intern("file_scope"));
    sym_ext =  ID2SYM(rb_intern("ext"));
}


static VALUE format_tag_entry(tag_file *tags)
{
    VALUE result = rb_hash_new();

    if(TagSuccess == tags->last_result) {
        VALUE extensions = rb_hash_new();
        unsigned short i;

        rb_hash_aset(result, sym_name, rb_str_new2(tags->entry.name));
        rb_hash_aset(result, sym_file, rb_str_new2(tags->entry.file));

        if(tags->entry.address.pattern)
            rb_hash_aset(result, sym_line_pattern, rb_str_new2(tags->entry.address.pattern));
        rb_hash_aset(result, sym_line_number, INT2FIX(tags->entry.address.lineNumber));

        rb_hash_aset(result, sym_kind, rb_str_new2(tags->entry.kind));

        if(tags->entry.fileScope)
            rb_hash_aset(result, sym_file_scope, Qtrue);

        for(i = 0; i < tags->entry.fields.count; i++) {
            rb_hash_aset(extensions,
                         rb_str_new2(tags->entry.fields.list[i].key),
                         rb_str_new2(tags->entry.fields.list[i].value));
        }
        rb_hash_aset(result, sym_ext, extensions);
    }
    return result;
}



static VALUE rb_tags_file_find_first(int argc, VALUE *argv, VALUE self)
{
    tag_file *tags;
    VALUE options;
    VALUE name;

    Data_Get_Struct(self, tag_file, tags);

    rb_scan_args(argc, argv, "11", &name, &options);
    Check_Type(name, T_STRING);
    if(NIL_P(options)){
        options = rb_hash_new();
    }

    if(Qtrue == option_or_default_int(options, "full_match", Qtrue))
        tags->flags |= TAG_FULLMATCH;
    if(Qtrue == option_or_default_int(options, "partial_match", Qtrue))
        tags->flags |= TAG_PARTIALMATCH;
    if(Qtrue == option_or_default_int(options, "observe_case", Qtrue))
        tags->flags |= TAG_OBSERVECASE;
    if(Qtrue == option_or_default_int(options, "ignore_case", Qtrue))
        tags->flags |= TAG_IGNORECASE;

    tags->last_result = tagsFind(tags->file, &tags->entry, StringValueCStr(name), tags->flags);
    return format_tag_entry(tags);
}

static VALUE rb_tags_file_find_next(VALUE self)
{
    tag_file *tags;
    Data_Get_Struct(self, tag_file, tags);

    if(TagSuccess == tags->last_result)
        tagsFindNext(tags->file, &tags->entry);
    return format_tag_entry(tags);
}


static VALUE rb_tags_file_close(VALUE self)
{
    tag_file *tags;
    Data_Get_Struct(self, tag_file, tags);
    rb_tags_file_free(tags);
    return Qnil;
}

static VALUE rb_tags_file_name(VALUE self)
{
    tag_file *tags;
    Data_Get_Struct(self, tag_file, tags);
    return tags->file_name;
}


extern VALUE rb_mTags;
VALUE rb_cTags_File;


void Init_tags_file()
{
    init_symbols();
    rb_cTags_File = rb_define_class_under(rb_mTags, "File", rb_cObject);
    rb_define_alloc_func(rb_cTags_File, rb_tags_file_allocate);
    rb_define_method(rb_cTags_File, "initialize", rb_tags_file_init, 1);
    rb_define_method(rb_cTags_File, "find_first", rb_tags_file_find_first, -1);
    rb_define_method(rb_cTags_File, "find_next", rb_tags_file_find_next, 0);
    rb_define_method(rb_cTags_File, "close!", rb_tags_file_close, 0);
    rb_define_method(rb_cTags_File, "file_name", rb_tags_file_name, 0);
}

