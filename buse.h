#ifndef BUSE_H_INCLUDED
#define BUSE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*reply_callback)(int, int, int, const char *);


#include <sys/types.h>

  struct buse_operations {
    int (*read)(void *buf, u_int32_t len, u_int64_t offset, void *userdata);
    int (*write)(const void *buf, u_int32_t len, u_int64_t offset, void *userdata);
    void (*disc)(void *userdata);
    int (*flush)(void *userdata);
    int (*trim)(u_int64_t from, u_int32_t len, void *userdata);

    // either set size, OR set both blksize and size_blocks
    u_int64_t size;
    u_int32_t blksize;
    u_int64_t size_blocks;
  };

  struct async_buse_operations {
    int (*async_read)(void *buf, u_int32_t len, u_int64_t offset, reply_callback callback);
    int (*async_write)(const void *buf, u_int32_t len, u_int64_t offset, reply_callback callback);
    void (*async_disc)(reply_callback callback);
    int (*async_flush)(reply_callback callback);
    int (*async_trim)(u_int64_t from, u_int32_t len, reply_callback callback);

    // either set size, OR set both blksize and size_blocks
    u_int64_t size;
    u_int32_t blksize;
    u_int64_t size_blocks;
  };
  
  int buse_main(const char* dev_file, const struct buse_operations *bop, void *userdata);
  int async_buse_main(const char* dev_file, const struct buse_operations *bop, void *userdata);

#ifdef __cplusplus
}
#endif

#endif /* BUSE_H_INCLUDED */
