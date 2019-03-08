
#include <assert.h>
#include <string.h>

#include <nut/security/digest/md5.h>
#include <nut/security/encrypt/aes_cbc_pkcs5.h>

#include "codec.h"

Codec::Codec(void *db)
    : _db(db)
{}

Codec::Codec(const Codec *another, void *db)
    : _db(db), _has_read_key(another->_has_read_key), _has_write_key(another->_has_write_key),
      _page_size(another->_page_size)
{
    ::memcpy(_read_key, another->_read_key, 16);
    ::memcpy(_write_key, another->_write_key, 16);
    _page_cache = (uint8_t*) ::malloc(sizeof(_page_size));
}

Codec::~Codec()
{
    if (NULL != _page_cache)
        ::free(_page_cache);
    _page_cache = NULL;
}

void Codec::set_page_size(int page_size)
{
    assert(0 == page_size % 16);
    if (_page_size == page_size)
        return;
    _page_size = page_size;
    _page_cache = (uint8_t*) ::realloc(_page_cache, page_size);
}

void Codec::generate_write_key(const char *user_password, int password_length)
{
    nut::MD5 md5;
    md5.update(user_password, password_length);
    md5.digest();
    md5.get_bytes_result(_write_key);
    _has_write_key = true;
}

void Codec::generate_read_key(const char *user_password, int password_length)
{
    nut::MD5 md5;
    md5.update(user_password, password_length);
    md5.digest();
    md5.get_bytes_result(_read_key);
    _has_read_key = true;
}

void Codec::drop_write_key()
{
    _has_write_key = false;
}

void Codec::set_write_is_read()
{
    ::memcpy(_write_key, _read_key, 16);
    _has_write_key = true;
}

void Codec::set_read_is_write()
{
    ::memcpy(_read_key, _write_key, 16);
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
    _err_msg = NULL;
    return ret;
}

void Codec::get_page_iv(int page, bool use_write_key, uint8_t iv[16])
{
    nut::MD5 md5;
    int32_t page_no = page; // Format to 4 bytes
    md5.update((const uint8_t*) &page_no, 4);
    md5.digest();
    md5.get_bytes_result(iv);

    nut::AES aes;
    aes.set_key(use_write_key ? _write_key : _read_key, 128);
    aes.encrypt(iv, iv);
}

uint8_t* Codec::encrypt(int page, uint8_t *data, bool use_write_key)
{
    uint8_t iv[16];
    get_page_iv(page, use_write_key, iv);

    nut::AES_CBC_PKCS5 aes_cbc;
    aes_cbc.start_encrypt(use_write_key ? _write_key : _read_key, 128, iv);
    aes_cbc.update_encrypt(data, _page_size);
    aes_cbc.finish_encrypt();
    nut::COWArray<uint8_t> rs = aes_cbc.get_result();
    assert(rs.size() == _page_size + 16);
    ::memcpy(_page_cache, rs.data(), _page_size); // Discard the padding

    return _page_cache;
}

void Codec::decrypt(int page, uint8_t *data)
{
    uint8_t iv[16];
    get_page_iv(page, false, iv);

    nut::AES_CBC_PKCS5 aes_cbc;
    aes_cbc.start_decrypt(_read_key, 128, iv);
    aes_cbc.update_decrypt(data, _page_size);
    // aes_cbc.finish_decrypt(); // NOTE No padding anymore
    nut::COWArray<uint8_t> rs = aes_cbc.get_result();
    assert(rs.size() == _page_size);
    ::memcpy(data, rs.data(), _page_size);
}
