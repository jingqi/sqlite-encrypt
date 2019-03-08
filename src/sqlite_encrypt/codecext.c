/**
 * 主要结构参考：
 * https://github.com/OlivierJG/botansqlite3
 */

// HACK 黑科技，不直接编译 "sqlite3.c", 而是放到这里，从而可以使用 sqlite 内部的结构体定义
#include "sqlite3.c"

#if !defined(SQLITE_OMIT_DISKIO) && defined(SQLITE_HAS_CODEC)

#include "codec_c_interface.h"

Bool handle_error(void *codec)
{
    if (!codec)
        return 0;

    const char *error = get_and_reset_error(codec);
    if (error)
    {
        sqlite3ErrorWithMsg((sqlite3*)codec_get_db(codec), SQLITE_ERROR, "Codec error: %s", error);
        return 1;
    }
    return 0;
}

// 初始化模块
void sqlite3_activate_see(const char *info)
{}

// 加密/解密
void* sqlite3Codec(void *codec, void *data, Pgno page_num, int mode)
{
    if (codec == NULL) //Db not encrypted
        return data;

    switch(mode)
    {
    case 0: // Undo a "case 7" journal file encryption
    case 2: // Reload a page
    case 3: // Load a page
        if (has_read_key(codec))
            codec_decrypt(codec, page_num, (unsigned char*) data);
        break;

    case 6: // Encrypt a page for the main database file
        if (has_write_key(codec))
            data = codec_encrypt(codec, page_num, (unsigned char*) data, 1);
        break;

    case 7: // Encrypt a page for the journal file
        /*
         * Under normal circumstances, the readkey is the same as the writekey.  However,
         * when the database is being rekeyed, the readkey is not the same as the writekey.
         * (The writekey is the "destination key" for the rekey operation and the readkey
         * is the key the db is currently encrypted with)
         * Therefore, for case 7, when the rollback is being written, always encrypt using
         * the database's readkey, which is guaranteed to be the same key that was used to
         * read and write the original data.
         */
        if (has_read_key(codec))
            data = codec_encrypt(codec, page_num, (unsigned char*) data, 0);
        break;
    }

    handle_error(codec);

    return data;
}

// 报告页大小
void sqlite3CodecSizeChange(void *codec, int page_size, int reserve)
{
    set_page_size(codec, page_size);
}

// 销毁加密器
void sqlite3PagerFreeCodec(void *codec)
{
    if (codec)
        delete_codec(codec);
}

// 添加加密器以及处理函数
int sqlite3CodecAttach(sqlite3 *db, int n_db, const void *z_key, int n_key)
{
    void *codec = NULL;

    if (NULL == z_key || n_key <= 0)
    {
        // No key specified, could mean either use the main db's encryption or no encryption
        if (0 != n_db && n_key < 0)
        {
            // Is an attached database, therefore use the key of main database, if main database is encrypted
            void *main_codec = sqlite3PagerGetCodec(sqlite3BtreePager(db->aDb[0].pBt));
            if (main_codec != NULL)
            {
                codec = initialize_from_other_codec(main_codec, db);
                sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[n_db].pBt),
                                     sqlite3Codec,
                                     sqlite3CodecSizeChange,
                                     sqlite3PagerFreeCodec,
                                     codec);
            }
        }
    }
    else
    {
        // Key specified, setup encryption key for database
        codec = initialize_new_codec(db);
        generate_write_key(codec, (const char*) z_key, n_key);
        set_read_is_write(codec);
        sqlite3PagerSetCodec(sqlite3BtreePager(db->aDb[n_db].pBt),
                             sqlite3Codec,
                             sqlite3CodecSizeChange,
                             sqlite3PagerFreeCodec,
                             codec);
    }

    if (handle_error(codec))
        return SQLITE_ERROR;

    return SQLITE_OK;
}

void sqlite3CodecGetKey(sqlite3 *db, int n_db, void **z_key, int *n_key)
{
    // The unencrypted password is not stored for security reasons
    // therefore always return NULL
    *z_key = NULL;
    *n_key = -1;
}

int sqlite3_key_v2(sqlite3 *db, const char *z_db_name, const void *z_key, int n_key)
{
    // We don't use z_db_name (though maybe we could...). Pass-through to the old sqlite_key
    return sqlite3_key(db, z_key, n_key);
}

int sqlite3_rekey_v2(sqlite3 *db, const char *z_db_name, const void *z_key, int n_key)
{
    // We don't use z_db_name (though maybe we could...). Pass-through to the old sqlite_rekey
    return sqlite3_rekey(db, z_key, n_key);
}

int sqlite3_key(sqlite3 *db, const void *z_key, int n_key)
{
    // The key is only set for the main database, not the temp database
    return sqlite3CodecAttach(db, 0, z_key, n_key);
}

int sqlite3_rekey(sqlite3 *db, const void *z_key, int n_key)
{
    // Changes the encryption key for an existing database.
    int rc = SQLITE_ERROR;
    Btree *pbt = db->aDb[0].pBt;
    Pager *p_pager = sqlite3BtreePager(pbt);
    void *codec = sqlite3PagerGetCodec(p_pager);

    if ((z_key == NULL || n_key == 0) && codec == NULL)
    {
        // Database not encrypted and key not specified. Do nothing
        return SQLITE_OK;
    }

    if (codec == NULL)
    {
        // Database not encrypted, but key specified. Encrypt database
        codec = initialize_new_codec(db);
        generate_write_key(codec, (const char*) z_key, n_key);

        if (handle_error(codec))
            return SQLITE_ERROR;

        sqlite3PagerSetCodec(p_pager, sqlite3Codec, sqlite3CodecSizeChange,
                             sqlite3PagerFreeCodec, codec);
    }
    else if (z_key == NULL || n_key == 0)
    {
        // Database encrypted, but key not specified. Decrypt database
        // Keep read key, drop write key
        drop_write_key(codec);
    }
    else
    {
        // Database encrypted and key specified. Re-encrypt database with new key
        // Keep read key, change write key to new key
        generate_write_key(codec, (const char*) z_key, n_key);
        if (handle_error(codec))
            return SQLITE_ERROR;
    }

    // Start transaction
    rc = sqlite3BtreeBeginTrans(pbt, 1);
    if (rc == SQLITE_OK)
    {
        // Rewrite all pages using the new encryption key (if specified)
        int n_page_count = -1;
        sqlite3PagerPagecount(p_pager, &n_page_count);
        Pgno n_page = (Pgno) n_page_count;

        Pgno n_skip = PAGER_MJ_PGNO(p_pager);
        DbPage *p_page;

        Pgno n;
        for (n = 1; rc == SQLITE_OK && n <= n_page; n++)
        {
            if (n == n_skip)
                continue;

            rc = sqlite3PagerGet(p_pager, n, &p_page, 0);

            if (!rc)
            {
                rc = sqlite3PagerWrite(p_page);
                sqlite3PagerUnref(p_page);
            }
            else
            {
                sqlite3ErrorWithMsg(db, SQLITE_ERROR, "%s", "Error while rekeying database page. Transaction Canceled.");
            }
        }
    }
    else
    {
        sqlite3ErrorWithMsg(db, SQLITE_ERROR, "%s", "Error beginning rekey transaction. Make sure that the current encryption key is correct.");
    }

    if (rc == SQLITE_OK)
    {
        // All good, commit
        rc = sqlite3BtreeCommit(pbt);

        if (rc == SQLITE_OK)
        {
            // Database rekeyed and committed successfully, update read key
            if (has_write_key(codec))
                set_read_is_write(codec);
            else // No write key == no longer encrypted
                sqlite3PagerSetCodec(p_pager, NULL, NULL, NULL, NULL);
        }
        else
        {
            // FIXME: can't trigger this, not sure if rollback is needed, reference implementation didn't rollback
            sqlite3ErrorWithMsg(db, SQLITE_ERROR, "%s", "Could not commit rekey transaction.");
        }
    }
    else
    {
        // Rollback, rekey failed
        sqlite3BtreeRollback(pbt, SQLITE_ERROR, 0);

        // Go back to read key
        if (has_read_key(codec))
            set_write_is_read(codec);
        else // Database wasn't encrypted to start with
            sqlite3PagerSetCodec(p_pager, NULL, NULL, NULL, NULL);
    }

    return rc;
}

#endif
