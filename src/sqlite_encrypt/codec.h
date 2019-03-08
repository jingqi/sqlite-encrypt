
#ifndef ___HEADFILE_F092748F_6792_43E8_A49C_B1DDB615F3F4_
#define ___HEADFILE_F092748F_6792_43E8_A49C_B1DDB615F3F4_

#include <stdlib.h>
#include <stdint.h>

class Codec
{
    void *_db = NULL;

    bool _has_read_key = false, _has_write_key = false;
    uint8_t _read_key[16], _write_key[16];

    int _page_size = 0;
    uint8_t *_page_cache = NULL;

    const char *_err_msg = NULL;

private:
    void get_page_iv(int page, bool use_write_key, uint8_t iv[16]);

public:
    Codec(void *db);
    Codec(const Codec *another, void *db);
    ~Codec();

    void set_page_size(int page_size);
    void generate_write_key(const char *user_password, int password_length);
    void generate_read_key(const char *user_password, int password_length);
    void drop_write_key();
    void set_write_is_read();
    void set_read_is_write();

    bool has_write_key() const;
    bool has_read_key() const;
    void *get_db() const;
    const char* get_and_reset_error();

    uint8_t* encrypt(int page, uint8_t *data, bool use_write_key);
    void decrypt(int page, uint8_t *data);
};

#endif
