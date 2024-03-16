#ifndef BUSE_H_INCLUDED
#define BUSE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif



#include <sys/types.h>
#include <linux/nbd.h>
#include "lfqueue.h"

struct nbd_request_context;

typedef void (*finish_callback)(struct nbd_request_context * );

typedef struct nbd_request_context {
  struct nbd_request request;
  struct nbd_reply reply;
  finish_callback finish; // 可以并发调用，将指针传入一个无锁队列中，另一个线程从队列中取出并调用
  finish_callback callback; // 不能并发调用，在finish线程中轮询无锁队列，调用callback

  lfqueue_t *finish_ctx_queue;
  int sk;
  int err;
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
    lfqueue_t finish_ctx_queue;
  };
  
  int buse_main(const char* dev_file, const struct buse_operations *bop, void *userdata);
  int async_buse_main(const char* dev_file, const struct async_buse_operations *bop, void *userdata);

#ifdef __cplusplus
}
#endif

#endif /* BUSE_H_INCLUDED */
