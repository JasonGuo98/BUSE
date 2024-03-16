#ifndef BUSE_H_INCLUDED
#define BUSE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif



#include <sys/types.h>
#include <linux/nbd.h>

struct nbd_request_context;

typedef void (*finish_callback)(int, struct nbd_request_context * );

typedef struct nbd_request_context {
  struct nbd_request request;
  struct nbd_reply reply;
  finish_callback callback;
  int sk;
  void *chunk;
} nbd_request_context_t;

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
    void (*async_read)(void *buf, u_int32_t len, u_int64_t offset, struct nbd_request_context * ctx);
    void (*async_write)(const void *buf, u_int32_t len, u_int64_t offset, struct nbd_request_context * ctx);
    void (*async_disc)(struct nbd_request_context * ctx);
    void (*async_flush)(struct nbd_request_context * ctx);
    void (*async_trim)(u_int64_t from, u_int32_t len, struct nbd_request_context * ctx);

    // either set size, OR set both blksize and size_blocks
    u_int64_t size;
    u_int32_t blksize;
    u_int64_t size_blocks;
  };
  
  int buse_main(const char* dev_file, const struct buse_operations *bop, void *userdata);
  int async_buse_main(const char* dev_file, const struct async_buse_operations *bop, void *userdata);

#ifdef __cplusplus
}
#endif

#endif /* BUSE_H_INCLUDED */
