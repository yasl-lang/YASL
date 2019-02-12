#include "lexinput.h"

struct LEXINPUT {
  FILE *fp;
  ByteBuffer *bb;
  int pos;
  int iseof;
  int (*getc)(struct LEXINPUT *lp);
  int (*tell)(struct LEXINPUT *lp);
  int (*seek)(struct LEXINPUT *lp, int w, int cmd);
  int (*close)(struct LEXINPUT *lp);
  int (*eof)(struct LEXINPUT *lp);
};


int lxgetc(struct LEXINPUT *lp)
{
#undef getc
  int ch = lp->getc(lp);
  //  //  printf("getc = '%c' %d\n", ch, ch);
  return ch;
}

int lxtell(struct LEXINPUT *lp)
{
  int d = lp->tell(lp);
  //  printf("tell = %d\n", d);
  return d;
}

int lxseek(struct LEXINPUT *lp, int w, int cmd)
{
  int r = lp->seek(lp, w, cmd);
  //  printf("seek = %d %d %d\n", r, w, cmd);
  //  lxtell(lp);
  return r;
}

int lxclose(struct LEXINPUT *lp)
{
    return lp->close(lp);
}
int lxeof(struct LEXINPUT *lp)
{
  int d = lp->eof(lp);
  //  printf("eof = %d\n", d);
  return d;
}

static  int lexinput_file_getc(struct LEXINPUT *lp)
{
    return fgetc(lp->fp);
}
static  int lexinput_file_tell(struct LEXINPUT *lp)
{
    return ftell(lp->fp);
}
static  int lexinput_file_seek(struct LEXINPUT *lp, int w, int cmd)
{
    return fseek(lp->fp, w, cmd);
}
static  int lexinput_file_eof(struct LEXINPUT *lp)
{
    return feof(lp->fp);
}
static  int lexinput_file_close(struct LEXINPUT *lp)
{
    fclose(lp->fp);
    lp->fp = 0;
    free(lp);
    return 0;
}

struct LEXINPUT *lexinput_new_file(FILE *fp)
{
    struct LEXINPUT *lp = (struct LEXINPUT *)malloc(sizeof(struct LEXINPUT));
    lp->fp = fp;
    lp->getc = lexinput_file_getc;
    lp->tell = lexinput_file_tell;
    lp->seek = lexinput_file_seek;
    lp->close = lexinput_file_close;
    lp->eof = lexinput_file_eof;
    return lp;
}

#include "bytebuffer/bytebuffer.h"

static int lexinput_bb_eof(struct LEXINPUT *lp);
static  int lexinput_bb_getc(struct LEXINPUT *lp)
{
    if (lp->pos >= (signed)lp->bb->count) {
        lp->iseof = 1;
        return -1;
    }
    return lp->bb->bytes[lp->pos++];
}
static  int lexinput_bb_tell(struct LEXINPUT *lp)
{
    return lp->pos;
}
static  int lexinput_bb_seek(struct LEXINPUT *lp, int w, int cmd)
{
    if (cmd == 0) {
        lp->pos = w;
    }
    else if (cmd == 1) {
        lp->pos += w;
    }
    else if (cmd == 2) {
        lp->pos = lp->bb->count = w;
    }
    if (lp->pos < (signed)lp->bb->count)         lp->iseof = 0;
    return 0;
}
static  int lexinput_bb_eof(struct LEXINPUT *lp)
{
    int ret;
    if (lp->pos >= (signed)lp->bb->count) {
      return lp->iseof;
    }
    return 0;
}
static  int lexinput_bb_close(struct LEXINPUT *lp)
{
    free(lp->bb);
    lp->bb = 0;
    free(lp);
    return 0;
}

struct LEXINPUT *lexinput_new_bb(char *buf, int len)
{
    struct LEXINPUT *lp = (struct LEXINPUT *)malloc(sizeof(struct LEXINPUT));
    lp->bb = bb_new(len);
    bb_append(lp->bb, (unsigned char *)buf, len);
    lp->getc = lexinput_bb_getc;
    lp->tell = lexinput_bb_tell;
    lp->seek = lexinput_bb_seek;
    lp->close = lexinput_bb_close;
    lp->eof = lexinput_bb_eof;
    lp->iseof = 0;
    lp->pos = 0;    
    return lp;
}
