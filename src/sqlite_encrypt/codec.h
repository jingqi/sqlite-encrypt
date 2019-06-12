/**
 * 这个文件定义了 Codec 的 C++ 和 C 接口。Codec 是加密功能的具体实现
 */
#ifndef ___HEADFILE_F092748F_6792_43E8_A49C_B1DDB615F3F4_
#define ___HEADFILE_F092748F_6792_43E8_A49C_B1DDB615F3F4_

#include <stdlib.h> // for size_t
#include <stdint.h>

#ifndef __cplusplus
#   include <stdbool.h>
#endif

#ifndef EXTERN_C
#   ifdef __cplusplus
#       define EXTERN_C extern "C"
#   else
#       define EXTERN_C extern
#   endif
#endif

#ifndef NOEXCEPT
#   ifdef __cplusplus
#       define NOEXCEPT noexcept
#   else
#       define NOEXCEPT
#   endif
#endif


#ifdef __cplusplus
class Codec
{
public:
    Codec(void *db) noexcept;
    Codec(const Codec *another, void *db) noexcept;
    ~Codec() noexcept;

    void set_page_size(size_t page_size) noexcept;

    void generate_write_key(const char *user_password, size_t password_length) noexcept;
    void generate_read_key(const char *user_password, size_t password_length) noexcept;

    void drop_write_key() noexcept;
    void set_write_is_read() noexcept;
    void set_read_is_write() noexcept;

    bool has_write_key() const noexcept;
    bool has_read_key() const noexcept;

    void *get_db() const noexcept;
    const char* get_and_reset_error() noexcept;

    uint8_t* encrypt(uint32_t page_no, const uint8_t *data, bool use_write_key) noexcept;
    void decrypt(uint32_t page_no, uint8_t *data) noexcept;

private:
    Codec(const Codec&) = delete;
    Codec& operator=(const Codec&) = delete;

    void get_page_iv(uint32_t page_no, bool use_write_key, uint8_t iv[16]) noexcept;

private:
    void *_db = nullptr;

    bool _has_read_key = false, _has_write_key = false;
    uint8_t _read_key[32], _write_key[32];

    size_t _page_size = 0;
    uint8_t *_page_cache = nullptr;

    const char *_err_msg = nullptr;
};
#endif /* __cplusplus */

/*******************************************************************************/

EXTERN_C void* codec_new(void *db) NOEXCEPT;
EXTERN_C void* codec_new_from_other(const void *other_codec, void *db) NOEXCEPT;

EXTERN_C void codec_delete(void *codec) NOEXCEPT;

EXTERN_C void codec_generate_write_key(void *codec, const char *user_password, size_t password_length) NOEXCEPT;
EXTERN_C void codec_drop_write_key(void *codec) NOEXCEPT;

EXTERN_C void codec_set_write_is_read(void *codec) NOEXCEPT;
EXTERN_C void codec_set_read_is_write(void *codec) NOEXCEPT;

EXTERN_C unsigned char* codec_encrypt(void *codec, uint32_t page_no, const uint8_t *data, bool use_write_key) NOEXCEPT;
EXTERN_C void codec_decrypt(void *codec, uint32_t page_no, uint8_t *data) NOEXCEPT;

EXTERN_C void codec_set_page_size(void *codec, size_t page_size) NOEXCEPT;

EXTERN_C bool codec_has_read_key(void *codec) NOEXCEPT;
EXTERN_C bool codec_has_write_key(void *codec) NOEXCEPT;

EXTERN_C void* codec_get_db(void *codec) NOEXCEPT;

EXTERN_C const char* codec_get_and_reset_error(void *codec) NOEXCEPT;

#endif
