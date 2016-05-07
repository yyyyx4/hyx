#ifndef BLOB_H
#define BLOB_H

#include "common.h"
#include "history.h"

enum blob_alloc {
    BLOB_MALLOC = 0,
    BLOB_MMAP,
};

struct blob {
    enum blob_alloc alloc;

    char *filename;
    size_t len;
    byte *data;
    uint8_t *dirty;

    struct diff *undo, *redo;

    struct {
        size_t len;
        byte *data;
    } clipboard;
};

void blob_init(struct blob *blob);
void blob_replace(struct blob *blob, size_t pos, byte *data, size_t len, bool save_history);
void blob_insert(struct blob *blob, size_t pos, byte *data, size_t len, bool save_history);
void blob_delete(struct blob *blob, size_t pos, size_t len, bool save_history);
void blob_free(struct blob *blob);

bool blob_can_move(struct blob *blob);

bool blob_undo(struct blob *blob);
bool blob_redo(struct blob *blob);

void blob_yank(struct blob *blob, size_t pos, size_t len);
size_t blob_paste(struct blob *blob, size_t pos, enum op_type type);

void blob_load(struct blob *blob, char *filename);
bool blob_save(struct blob *blob, char *filename);

#define blob_length(B) ((size_t const) (B)->len)
//size_t blob_length(struct blob *blob);
#define blob_at(B, N) (*blob_lookup((B), (N), NULL))
//byte blob_at(struct blob *blob, size_t pos);
byte const *blob_lookup(struct blob *blob, size_t pos, size_t *len);
void blob_read_strict(struct blob *blob, size_t pos, byte *buf, size_t len);

#endif
