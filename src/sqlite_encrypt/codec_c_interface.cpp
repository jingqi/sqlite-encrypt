
#include "codec_c_interface.h"
#include "codec.h"

void* initialize_new_codec(void *db)
{
    return new Codec(db);
}

void* initialize_from_other_codec(const void *other_codec, void *db)
{
    return new Codec((Codec*) other_codec, db);
}

void delete_codec(void *codec)
{
    delete (Codec*) codec;
}

void generate_write_key(void *codec, const char *user_password, int password_length)
{
    ((Codec*) codec)->generate_write_key(user_password, password_length);
}

void drop_write_key(void *codec)
{
    ((Codec*) codec)->drop_write_key();
}

void set_write_is_read(void *codec)
{
    ((Codec*) codec)->set_write_is_read();
}

void set_read_is_write(void *codec)
{
    ((Codec*) codec)->set_read_is_write();
}

unsigned char* codec_encrypt(void *codec, int page, unsigned char *data, Bool use_write_key)
{
    return ((Codec*) codec)->encrypt(page, data, use_write_key);
}

void codec_decrypt(void *codec, int page, unsigned char *data)
{
    ((Codec*) codec)->decrypt(page, data);
}

void set_page_size(void *codec, int page_size)
{
    ((Codec*) codec)->set_page_size(page_size);
}

Bool has_read_key(void *codec)
{
    return ((Codec*) codec)->has_read_key();
}

Bool has_write_key(void *codec)
{
    return ((Codec*) codec)->has_write_key();
}

void* codec_get_db(void *codec)
{
    return ((Codec*) codec)->get_db();
}

const char* get_and_reset_error(void *codec)
{
    return ((Codec*) codec)->get_and_reset_error();
}
