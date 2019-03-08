
#ifndef ___HEADFILE_5311222A_44D4_407C_AF40_82ED3E3E2046_
#define ___HEADFILE_5311222A_44D4_407C_AF40_82ED3E3E2046_


#ifdef __cplusplus
typedef unsigned char Bool;
#endif

#ifdef __cplusplus
extern "C"
#endif
void* initialize_new_codec(void *db);

#ifdef __cplusplus
extern "C"
#endif
void* initialize_from_other_codec(const void *other_codec, void *db);

#ifdef __cplusplus
extern "C"
#endif
void generate_write_key(void *codec, const char *user_password, int password_length);

#ifdef __cplusplus
extern "C"
#endif
void drop_write_key(void *codec);

#ifdef __cplusplus
extern "C"
#endif
void set_write_is_read(void *codec);

#ifdef __cplusplus
extern "C"
#endif
void set_read_is_write(void *codec);

#ifdef __cplusplus
extern "C"
#endif
unsigned char* codec_encrypt(void *codec, int page, unsigned char *data, Bool use_write_key);

#ifdef __cplusplus
extern "C"
#endif
void codec_decrypt(void *codec, int page, unsigned char *data);

#ifdef __cplusplus
extern "C"
#endif
void set_page_size(void *codec, int page_size);

#ifdef __cplusplus
extern "C"
#endif
Bool has_read_key(void *codec);

#ifdef __cplusplus
extern "C"
#endif
Bool has_write_key(void *codec);

#ifdef __cplusplus
extern "C"
#endif
void* codec_get_db(void *codec);

#ifdef __cplusplus
extern "C"
#endif
const char* get_and_reset_error(void *codec);

#ifdef __cplusplus
extern "C"
#endif
void delete_codec(void *codec);

#endif
