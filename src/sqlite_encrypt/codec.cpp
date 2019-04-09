
#include <assert.h>
#include <string.h>

#include <nut/security/digest/sha2_256.h>
#include <nut/security/encrypt/aes.h>

#include "codec.h"


Codec::Codec(void *db)
    : _db(db)
{}

Codec::Codec(const Codec *another, void *db)
    : _db(db), _has_read_key(another->_has_read_key), _has_write_key(another->_has_write_key),
      _page_size(another->_page_size)
{
    ::memcpy(_read_key, another->_read_key, 32);
    ::memcpy(_write_key, another->_write_key, 32);
    _page_cache = (uint8_t*) ::malloc(sizeof(_page_size));
}

Codec::~Codec()
{
    if (nullptr != _page_cache)
        ::free(_page_cache);
    _page_cache = nullptr;
}

void Codec::set_page_size(size_t page_size)
{
    assert(0 == page_size % 16);
    if (_page_size == page_size)
        return;
    _page_size = page_size;
    _page_cache = (uint8_t*) ::realloc(_page_cache, page_size);
}

void Codec::generate_write_key(const char *user_password, size_t password_length)
{
    assert(nullptr != user_password);

    nut::SHA2_256 sha;
    sha.update(user_password, password_length);
    sha.digest();
    ::memcpy(_write_key, sha.get_result(), 32);
    _has_write_key = true;
}

void Codec::generate_read_key(const char *user_password, size_t password_length)
{
    assert(nullptr != user_password);

    nut::SHA2_256 sha;
    sha.update(user_password, password_length);
    sha.digest();
    ::memcpy(_read_key, sha.get_result(), 32);
    _has_read_key = true;
}

void Codec::drop_write_key()
{
    _has_write_key = false;
}

void Codec::set_write_is_read()
{
    ::memcpy(_write_key, _read_key, 32);
    _has_write_key = true;
}

void Codec::set_read_is_write()
{
    ::memcpy(_read_key, _write_key, 32);
    _has_read_key = true;
}

bool Codec::has_write_key() const
{
    return _has_write_key;
}

bool Codec::has_read_key() const
{
    return _has_read_key;
}

void* Codec::get_db() const
{
    return _db;
}

const char* Codec::get_and_reset_error()
{
    const char *ret = _err_msg;
    _err_msg = nullptr;
    return ret;
}

void Codec::get_page_iv(uint32_t page_no, bool use_write_key, uint8_t iv[16])
{
    nut::SHA2_256 sha;
    sha.update((const uint8_t*) &page_no, 4);
    sha.digest();
    for (int i = 0; i < 16; ++i)
        iv[i] = sha.get_result()[i] ^ sha.get_result()[i + 16];

    nut::AES aes;
    aes.set_key(use_write_key ? _write_key : _read_key, 256);
    aes.encrypt(iv, iv);
}

static void xor_buf(uint8_t *dst, const uint8_t *src, size_t cb)
{
    assert(nullptr != dst && nullptr != src);
    for (size_t i = 0; i < cb; ++i)
        dst[i] ^= src[i];
}

uint8_t* Codec::encrypt(uint32_t page_no, const uint8_t *data, bool use_write_key)
{
    uint8_t iv[16];
    get_page_iv(page_no, use_write_key, iv);

    nut::AES aes;
    aes.set_key(use_write_key ? _write_key : _read_key, 256);
    assert(0 == _page_size % 16);
    for (size_t i = 0; i < _page_size; i += 16)
    {
        xor_buf(iv, data + i, 16);
        aes.encrypt(iv, iv);
        ::memcpy(_page_cache + i, iv, 16);
    }

    return _page_cache;
}

void Codec::decrypt(uint32_t page_no, uint8_t *data)
{
    uint8_t iv[16];
    get_page_iv(page_no, false, iv);

    nut::AES aes;
    uint8_t decrypt_buf[16];
    aes.set_key(_read_key, 256);
    assert(0 == _page_size % 16);
    for (size_t i = 0; i < _page_size; i += 16)
    {
        aes.decrypt(data + i, decrypt_buf);
        xor_buf(decrypt_buf, iv, 16);
        ::memcpy(iv, data + i, 16);
        ::memcpy(data + i, decrypt_buf, 16);
    }
}

/*******************************************************************************/

EXTERN_C void* codec_new(void *db)
{
    return new Codec(db);
}

EXTERN_C void* codec_new_from_other(const void *other_codec, void *db)
{
    return new Codec((Codec*) other_codec, db);
}

EXTERN_C void codec_delete(void *codec)
{
    delete (Codec*) codec;
}

EXTERN_C void codec_generate_write_key(void *codec, const char *user_password, size_t password_length)
{
    ((Codec*) codec)->generate_write_key(user_password, password_length);
}

EXTERN_C void codec_drop_write_key(void *codec)
{
    ((Codec*) codec)->drop_write_key();
}

EXTERN_C void codec_set_write_is_read(void *codec)
{
    ((Codec*) codec)->set_write_is_read();
}

EXTERN_C void codec_set_read_is_write(void *codec)
{
    ((Codec*) codec)->set_read_is_write();
}

EXTERN_C unsigned char* codec_encrypt(void *codec, uint32_t page_no, const uint8_t *data, bool use_write_key)
{
    return ((Codec*) codec)->encrypt(page_no, data, use_write_key);
}

EXTERN_C void codec_decrypt(void *codec, uint32_t page_no, uint8_t *data)
{
    ((Codec*) codec)->decrypt(page_no, data);
}

EXTERN_C void codec_set_page_size(void *codec, size_t page_size)
{
    ((Codec*) codec)->set_page_size(page_size);
}

EXTERN_C bool codec_has_read_key(void *codec)
{
    return ((Codec*) codec)->has_read_key();
}

EXTERN_C bool codec_has_write_key(void *codec)
{
    return ((Codec*) codec)->has_write_key();
}

EXTERN_C void* codec_get_db(void *codec)
{
    return ((Codec*) codec)->get_db();
}

EXTERN_C const char* codec_get_and_reset_error(void *codec)
{
    return ((Codec*) codec)->get_and_reset_error();
}
