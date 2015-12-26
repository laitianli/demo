
#define STAP_VERSION(a, b) ( ((a) << 8) + (b) )
#ifndef STAP_COMPAT_VERSION
#define STAP_COMPAT_VERSION STAP_VERSION(1, 3)
#endif
#define STP_PRIVILEGED 1
#ifndef MAXNESTING
#define MAXNESTING 3
#endif
#include <asm/types.h>
#ifndef MAXSTRINGLEN
#if BITS_PER_LONG == 32
#define MAXSTRINGLEN 256
#else
#define MAXSTRINGLEN 512
#endif
#endif
#ifndef MAXACTION
#define MAXACTION 1000
#endif
#ifndef MAXACTION_INTERRUPTIBLE
#define MAXACTION_INTERRUPTIBLE (MAXACTION * 10)
#endif
#ifndef TRYLOCKDELAY
#define TRYLOCKDELAY 10 /* microseconds */
#endif
#ifndef MAXTRYLOCK
#define MAXTRYLOCK 100 /* 1 millisecond total */
#endif
#ifndef MAXMAPENTRIES
#define MAXMAPENTRIES 2048
#endif
#ifndef MAXERRORS
#define MAXERRORS 0
#endif
#ifndef MAXSKIPPED
#define MAXSKIPPED 100
#endif
#ifndef MINSTACKSPACE
#define MINSTACKSPACE 1024
#endif
#ifndef INTERRUPTIBLE
#define INTERRUPTIBLE 1
#endif
#ifndef STP_OVERLOAD_INTERVAL
#define STP_OVERLOAD_INTERVAL 1000000000LL
#endif
#ifndef STP_OVERLOAD_THRESHOLD
#define STP_OVERLOAD_THRESHOLD 500000000LL
#endif
#if !defined(STP_NO_OVERLOAD) && !defined(STAP_NO_OVERLOAD)
#define STP_OVERLOAD
#endif
#define STP_SKIP_BADVARS 0
#include "runtime.h"
#include "stack.c"
#include "stat.c"
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/profile.h>
#include <linux/random.h>
#include <linux/vermagic.h>
#include <linux/utsname.h>
#include <linux/version.h>
#include "loc2c-runtime.h" 
#include "access_process_vm.h" 

typedef char string_t[MAXSTRINGLEN];

#define STAP_SESSION_STARTING 0
#define STAP_SESSION_RUNNING 1
#define STAP_SESSION_ERROR 2
#define STAP_SESSION_STOPPING 3
#define STAP_SESSION_STOPPED 4
static atomic_t session_state = ATOMIC_INIT (STAP_SESSION_STARTING);
static atomic_t error_count = ATOMIC_INIT (0);
static atomic_t skipped_count = ATOMIC_INIT (0);
static atomic_t skipped_count_lowstack = ATOMIC_INIT (0);
static atomic_t skipped_count_reentrant = ATOMIC_INIT (0);
static atomic_t skipped_count_uprobe_reg = ATOMIC_INIT (0);
static atomic_t skipped_count_uprobe_unreg = ATOMIC_INIT (0);

#define _STP_REGS_USER_FLAG 1

struct context {
  atomic_t busy;
  const char *probe_point;
  const char *probe_name;
  int actionremaining;
  int nesting;
  string_t error_buffer;
  const char *last_error;
  const char *last_stmt;
  struct pt_regs *regs;
  #if defined __ia64__
  unsigned long *unwaddr;
  #endif
  struct kretprobe_instance *pi;
  int pi_longs;
  int regflags;
  int regparm;
  va_list *mark_va_list;
  const char * marker_name;
  const char * marker_format;
  void *data;
  #ifdef STP_TIMING
  Stat *statp;
  #endif
  #ifdef STP_OVERLOAD
  cycles_t cycles_base;
  cycles_t cycles_sum;
  #endif
  struct uretprobe_instance *ri;
  union {
    struct probe_1998_locals {
    } probe_1998;
    struct probe_1999_locals {
    } probe_1999;
    struct probe_2001_locals {
      string_t devname;
      union {
        struct {
          string_t __tmp0;
          int64_t __tmp2;
        };
        struct {
          string_t __tmp3;
        };
      };
    } probe_2001;
  } probe_locals;
  union {
    struct function_MINOR_locals {
      int64_t dev;
      int64_t __retvalue;
    } function_MINOR;
    struct function___bio_devname_locals {
      int64_t bio;
      int64_t __tmp0;
      int64_t __tmp1;
      string_t __retvalue;
    } function___bio_devname;
    struct function__dwarf_cast_get_cast_10_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_10;
    struct function__dwarf_cast_get_cast_3_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_3;
    struct function__dwarf_cast_get_cast_4_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_4;
    struct function__dwarf_cast_get_cast_5_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_5;
    struct function__dwarf_cast_get_cast_6_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_6;
    struct function__dwarf_cast_get_cast_7_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_7;
    struct function__dwarf_cast_get_cast_8_locals {
      int64_t pointer;
      int64_t __retvalue;
    } function__dwarf_cast_get_cast_8;
    struct function__dwarf_tvar_get_bio_0_locals {
      int64_t __retvalue;
    } function__dwarf_tvar_get_bio_0;
    struct function_bdevname_locals {
      int64_t hd;
      int64_t partno;
      string_t disk_name;
      int64_t bdev;
      union {
        struct {
          int64_t __tmp0;
          int64_t __tmp2;
        };
        struct {
          int64_t __tmp3;
          int64_t __tmp4;
          int64_t __tmp6;
          int64_t __tmp7;
          int64_t __tmp9;
          int64_t __tmp10;
          int64_t __tmp11;
        };
        struct {
          int64_t __tmp12;
          int64_t __tmp13;
        };
        struct {
          string_t __tmp14;
          int64_t __tmp16;
          int64_t __tmp17;
        };
        struct {
          string_t __tmp18;
          string_t __tmp19;
          int64_t __tmp20;
          string_t __tmp21;
          string_t __tmp23;
          int64_t __tmp24;
          string_t __tmp25;
          string_t __tmp26;
          int64_t __tmp27;
          string_t __tmp28;
        };
      };
      string_t __retvalue;
    } function_bdevname;
    struct function_isdigit_locals {
      string_t str;
      int64_t __retvalue;
    } function_isdigit;
    struct function_kernel_string_locals {
      int64_t addr;
      string_t __retvalue;
    } function_kernel_string;
    struct function_log_locals {
      string_t msg;
      /* no return value */
    } function_log;
    struct function_strlen_locals {
      string_t s;
      int64_t __retvalue;
    } function_strlen;
    struct function_substr_locals {
      string_t str;
      int64_t start;
      int64_t length;
      string_t __retvalue;
    } function_substr;
  } locals [MAXNESTING+1];
  #if MAXNESTING < 0
  #error "MAXNESTING must be positive"
  #endif
};

static struct context *contexts[NR_CPUS] = { NULL };


#include "probe_lock.h" 

#include <linux/bio.h>
#include <linux/genhd.h>



#include <linux/kdev_t.h>



static void function_MINOR (struct context * __restrict__ c);

static void function___bio_devname (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_10 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_3 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_4 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_5 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_6 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_7 (struct context * __restrict__ c);

static void function__dwarf_cast_get_cast_8 (struct context * __restrict__ c);

static void function__dwarf_tvar_get_bio_0 (struct context * __restrict__ c);

static void function_bdevname (struct context * __restrict__ c);

static void function_isdigit (struct context * __restrict__ c);

static void function_kernel_string (struct context * __restrict__ c);

static void function_log (struct context * __restrict__ c);

static void function_strlen (struct context * __restrict__ c);

static void function_substr (struct context * __restrict__ c);

static void function_MINOR (struct context* __restrict__ c) {
  __label__ out;
  struct function_MINOR_locals *  __restrict__ l = & c->locals[c->nesting+1].function_MINOR;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'MINOR' at /usr/share/systemtap/tapset/dev.stp:19:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
     /* pure */
    THIS->__retvalue = MINOR(THIS->dev);

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function___bio_devname (struct context* __restrict__ c) {
  __label__ out;
  struct function___bio_devname_locals *  __restrict__ l = & c->locals[c->nesting+1].function___bio_devname;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '__bio_devname' at /usr/share/systemtap/tapset/ioblock.stp:57:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue[0] = '\0';
  #define return goto out
  strlcpy (l->__retvalue, 
  ({
    l->__tmp0 = 
    ({
      l->__tmp1 = l->bio;
      c->locals[c->nesting+1].function__dwarf_cast_get_cast_3.pointer = l->__tmp1;
      function__dwarf_cast_get_cast_3 (c);
      if (unlikely(c->last_error)) goto out;
      c->locals[c->nesting+1].function__dwarf_cast_get_cast_3.__retvalue;
    });
    c->locals[c->nesting+1].function_bdevname.bdev = l->__tmp0;
    function_bdevname (c);
    if (unlikely(c->last_error)) goto out;
    c->locals[c->nesting+1].function_bdevname.__retvalue;
  }), MAXSTRINGLEN);
  c->actionremaining -= 1;
  if (unlikely (c->actionremaining <= 0)) {
    c->last_error = "MAXACTION exceeded";
    c->last_stmt = "keyword at /usr/share/systemtap/tapset/ioblock.stp:59:5";
    goto out;
  }
  goto out;
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_10 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_10_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_10;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:50:28";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(12)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 12UL;
        addr = s0;
      }
    }
    { // synthesized
      addr += 0UL * 1UL;
    }
    THIS->__retvalue = addr;
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_3 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_3_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_3;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/ioblock.stp:59:21";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(16)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 16UL;
        addr = s0;
      }
    }
    { // synthesized
    { uint64_t value = deref (8, addr); addr = value; }
    }
    THIS->__retvalue = addr;
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_4 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_4_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_4;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:40:7";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(152)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 152UL;
        addr = s0;
      }
    }
    { // synthesized
    { uint64_t value = deref (8, addr); addr = value; }
    }
    THIS->__retvalue = addr;
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_5 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_5_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_5;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:42:6";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(136)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 136UL;
        addr = s0;
      }
    }
    { // synthesized
    { uint64_t value = deref (8, addr); addr = value; }
    }
    THIS->__retvalue = addr;
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_6 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_6_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_6;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:43:12";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(136)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 136UL;
        addr = s0;
      }
    }
    { // synthesized
    { uint64_t value = deref (8, addr); addr = value; }
    }
    { // DWARF expression: 0x23(148)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 148UL;
        addr = s0;
      }
    }
    { int32_t value = deref (4, addr); THIS->__retvalue = value; }
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_7 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_7_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_7;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:45:18";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 0UL;
        addr = s0;
      }
    }
    { uint32_t value = deref (4, addr); THIS->__retvalue = value; }
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_cast_get_cast_8 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_cast_get_cast_8_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_cast_get_cast_8;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '@cast' at /usr/share/systemtap/tapset/dev.stp:46:6";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uintptr_t addr;
    { // synthesized
      addr = THIS->pointer;
    }
    { // DWARF expression: 0x23(152)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 152UL;
        addr = s0;
      }
    }
    { // synthesized
    { uint64_t value = deref (8, addr); addr = value; }
    }
    { // DWARF expression: 0x23(4)
      {
        uintptr_t s0;
        s0 = addr;
        s0 = s0 + 4UL;
        addr = s0;
      }
    }
    { int32_t value = deref (4, addr); THIS->__retvalue = value; }
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function__dwarf_tvar_get_bio_0 (struct context* __restrict__ c) {
  __label__ out;
  struct function__dwarf_tvar_get_bio_0_locals *  __restrict__ l = & c->locals[c->nesting+1].function__dwarf_tvar_get_bio_0;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier '$bio' at /usr/share/systemtap/tapset/ioblock.stp:98:33";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
    
#define fetch_register k_fetch_register
#define store_register k_store_register
{
  {
    uint64_t addr;
    { // DWARF expression: 0x55
    { uint64_t value = fetch_register (5); addr = value; }
    }
    THIS->__retvalue = addr;
  }
  goto out;
if (0) goto deref_fault;
deref_fault:
  goto out;
}
/* pure *//* unprivileged */
#undef fetch_register

#undef store_register

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_bdevname (struct context* __restrict__ c) {
  __label__ out;
  struct function_bdevname_locals *  __restrict__ l = & c->locals[c->nesting+1].function_bdevname;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'bdevname' at /usr/share/systemtap/tapset/dev.stp:35:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->hd = 0;
  l->partno = 0;
  l->disk_name[0] = '\0';
  l->__retvalue[0] = '\0';
  #define return goto out
  {
    c->actionremaining -= 1;
    if (unlikely (c->actionremaining <= 0)) {
      c->last_error = "MAXACTION exceeded";
      c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:37:2";
      goto out;
    }
    if ((((l->bdev) == (((int64_t)0LL))))) {
      strlcpy (l->__retvalue, "N/A", MAXSTRINGLEN);
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:38:3";
        goto out;
      }
      goto out;
    }
    
    (void) 
    ({
      l->__tmp0 = 
      ({
        l->__tmp2 = l->bdev;
        c->locals[c->nesting+1].function__dwarf_cast_get_cast_4.pointer = l->__tmp2;
        function__dwarf_cast_get_cast_4 (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function__dwarf_cast_get_cast_4.__retvalue;
      });
      l->hd = l->__tmp0;
      l->__tmp0;
    });
    
    c->actionremaining -= 2;
    if (unlikely (c->actionremaining <= 0)) {
      c->last_error = "MAXACTION exceeded";
      c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:42:2";
      goto out;
    }
    if (
      ({
        l->__tmp3 = l->bdev;
        c->locals[c->nesting+1].function__dwarf_cast_get_cast_5.pointer = l->__tmp3;
        function__dwarf_cast_get_cast_5 (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function__dwarf_cast_get_cast_5.__retvalue;
      })) {
      (void) 
      ({
        l->__tmp4 = 
        ({
          l->__tmp6 = l->bdev;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_6.pointer = l->__tmp6;
          function__dwarf_cast_get_cast_6 (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_6.__retvalue;
        });
        l->partno = l->__tmp4;
        l->__tmp4;
      });
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "identifier 'partno' at /usr/share/systemtap/tapset/dev.stp:43:3";
        goto out;
      }
    }
    else {
      (void) 
      ({
        l->__tmp7 = ((
        ({
          l->__tmp9 = 
          ({
            l->__tmp10 = l->bdev;
            c->locals[c->nesting+1].function__dwarf_cast_get_cast_7.pointer = l->__tmp10;
            function__dwarf_cast_get_cast_7 (c);
            if (unlikely(c->last_error)) goto out;
            c->locals[c->nesting+1].function__dwarf_cast_get_cast_7.__retvalue;
          });
          c->locals[c->nesting+1].function_MINOR.dev = l->__tmp9;
          function_MINOR (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function_MINOR.__retvalue;
        })) - (
        ({
          l->__tmp11 = l->bdev;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_8.pointer = l->__tmp11;
          function__dwarf_cast_get_cast_8 (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_8.__retvalue;
        })));
        l->partno = l->__tmp7;
        l->__tmp7;
      });
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "identifier 'partno' at /usr/share/systemtap/tapset/dev.stp:45:3";
        goto out;
      }
    }
    
    c->actionremaining -= 1;
    if (unlikely (c->actionremaining <= 0)) {
      c->last_error = "MAXACTION exceeded";
      c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:48:2";
      goto out;
    }
    if ((! (l->partno))) {
      strlcpy (l->__retvalue, 
      ({
        l->__tmp12 = 
        ({
          l->__tmp13 = l->hd;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_10.pointer = l->__tmp13;
          function__dwarf_cast_get_cast_10 (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_10.__retvalue;
        });
        c->locals[c->nesting+1].function_kernel_string.addr = l->__tmp12;
        function_kernel_string (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function_kernel_string.__retvalue;
      }), MAXSTRINGLEN);
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:49:3";
        goto out;
      }
      goto out;
    }
    
    (void) 
    ({
      strlcpy (l->__tmp14, 
      ({
        l->__tmp16 = 
        ({
          l->__tmp17 = l->hd;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_10.pointer = l->__tmp17;
          function__dwarf_cast_get_cast_10 (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function__dwarf_cast_get_cast_10.__retvalue;
        });
        c->locals[c->nesting+1].function_kernel_string.addr = l->__tmp16;
        function_kernel_string (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function_kernel_string.__retvalue;
      }), MAXSTRINGLEN);
      strlcpy (l->disk_name, l->__tmp14, MAXSTRINGLEN);
      l->__tmp14;
    });
    
    c->actionremaining -= 2;
    if (unlikely (c->actionremaining <= 0)) {
      c->last_error = "MAXACTION exceeded";
      c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:51:2";
      goto out;
    }
    if (
      ({
        strlcpy (l->__tmp18, 
        ({
          strlcpy (l->__tmp19, l->disk_name, MAXSTRINGLEN);
          l->__tmp20 = ((
          ({
            strlcpy (l->__tmp21, l->disk_name, MAXSTRINGLEN);
            strlcpy (c->locals[c->nesting+1].function_strlen.s, l->__tmp21, MAXSTRINGLEN);
            function_strlen (c);
            if (unlikely(c->last_error)) goto out;
            c->locals[c->nesting+1].function_strlen.__retvalue;
          })) - (((int64_t)1LL)));
          strlcpy (c->locals[c->nesting+1].function_substr.str, l->__tmp19, MAXSTRINGLEN);
          c->locals[c->nesting+1].function_substr.start = l->__tmp20;
          c->locals[c->nesting+1].function_substr.length = ((int64_t)1LL);
          function_substr (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function_substr.__retvalue;
        }), MAXSTRINGLEN);
        strlcpy (c->locals[c->nesting+1].function_isdigit.str, l->__tmp18, MAXSTRINGLEN);
        function_isdigit (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function_isdigit.__retvalue;
      })) {
      strlcpy (l->__retvalue, 
      ({
        strlcpy (l->__tmp23, l->disk_name, MAXSTRINGLEN);
        l->__tmp24 = l->partno;
        _stp_snprintf (l->__tmp25, MAXSTRINGLEN, "%sp%lld", l->__tmp23, l->__tmp24);
        l->__tmp25;
      }), MAXSTRINGLEN);
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:52:3";
        goto out;
      }
      goto out;
    }
    else {
      strlcpy (l->__retvalue, 
      ({
        strlcpy (l->__tmp26, l->disk_name, MAXSTRINGLEN);
        l->__tmp27 = l->partno;
        _stp_snprintf (l->__tmp28, MAXSTRINGLEN, "%s%lld", l->__tmp26, l->__tmp27);
        l->__tmp28;
      }), MAXSTRINGLEN);
      c->actionremaining -= 1;
      if (unlikely (c->actionremaining <= 0)) {
        c->last_error = "MAXACTION exceeded";
        c->last_stmt = "keyword at /usr/share/systemtap/tapset/dev.stp:54:3";
        goto out;
      }
      goto out;
    }
    
  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_isdigit (struct context* __restrict__ c) {
  __label__ out;
  struct function_isdigit_locals *  __restrict__ l = & c->locals[c->nesting+1].function_isdigit;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'isdigit' at /usr/share/systemtap/tapset/string.stp:216:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
     /* pure */ /* unprivileged */
	THIS->__retvalue = isdigit(THIS->str[0]);

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_kernel_string (struct context* __restrict__ c) {
  __label__ out;
  struct function_kernel_string_locals *  __restrict__ l = & c->locals[c->nesting+1].function_kernel_string;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'kernel_string' at /usr/share/systemtap/tapset/conversions.stp:21:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue[0] = '\0';
  #define return goto out
  {
     /* pure */
  char *destination = THIS->__retvalue;
  deref_string (destination, THIS->addr, MAXSTRINGLEN);
  if (0) {
deref_fault: /* branched to from deref_string() */
    snprintf (CONTEXT->error_buffer, sizeof(CONTEXT->error_buffer),
        "kernel string copy fault at 0x%p", (void *) (uintptr_t) THIS->addr);
    CONTEXT->last_error = CONTEXT->error_buffer;
  }

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_log (struct context* __restrict__ c) {
  __label__ out;
  struct function_log_locals *  __restrict__ l = & c->locals[c->nesting+1].function_log;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'log' at /usr/share/systemtap/tapset/logging.stp:24:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  #define return goto out
  {
     /* unprivileged */
	_stp_printf ("%s\n", THIS->msg);

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_strlen (struct context* __restrict__ c) {
  __label__ out;
  struct function_strlen_locals *  __restrict__ l = & c->locals[c->nesting+1].function_strlen;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'strlen' at /usr/share/systemtap/tapset/string.stp:19:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue = 0;
  #define return goto out
  {
     /* pure */ /* unprivileged */
        THIS->__retvalue = strlen(THIS->s);

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


static void function_substr (struct context* __restrict__ c) {
  __label__ out;
  struct function_substr_locals *  __restrict__ l = & c->locals[c->nesting+1].function_substr;
  (void) l;
  #define CONTEXT c
  #define THIS l
  c->last_stmt = "identifier 'substr' at /usr/share/systemtap/tapset/string.stp:35:10";
  if (unlikely (c->nesting+1 >= MAXNESTING)) {
    c->last_error = "MAXNESTING exceeded";
    return;
  } else {
    c->nesting ++;
  }
  l->__retvalue[0] = '\0';
  #define return goto out
  {
     /* pure */ /* unprivileged */
	int64_t length = clamp_t(int64_t, THIS->length + 1, 0, MAXSTRINGLEN);
	if (THIS->start >= 0 && THIS->start < strlen(THIS->str))
		strlcpy(THIS->__retvalue, THIS->str + THIS->start, length);

  }
  #undef return
out:
  if (0) goto out;
  c->nesting --;
  #undef CONTEXT
  #undef THIS
}


#ifdef STP_TIMING
static __cacheline_aligned Stat time_probe_0;
#endif

static void probe_1998 (struct context * __restrict__ c) {
  __label__ out;
  struct probe_1998_locals * __restrict__ l = & c->probe_locals.probe_1998;
  (void) l;
  #ifdef STP_TIMING
  c->statp = & time_probe_0;
  #endif
  (void) 
  ({
    strlcpy (c->locals[c->nesting+1].function_log.msg, "ioblock begin\n", MAXSTRINGLEN);
    function_log (c);
    if (unlikely(c->last_error)) goto out;
    (void) 0;
  });
  c->actionremaining -= 1;
  if (unlikely (c->actionremaining <= 0)) {
    c->last_error = "MAXACTION exceeded";
    c->last_stmt = "identifier 'log' at ioblock.stp:2:2";
    goto out;
  }
out:
  _stp_print_flush();
}


#ifdef STP_TIMING
static __cacheline_aligned Stat time_probe_1;
#endif

static void probe_1999 (struct context * __restrict__ c) {
  __label__ out;
  struct probe_1999_locals * __restrict__ l = & c->probe_locals.probe_1999;
  (void) l;
  #ifdef STP_TIMING
  c->statp = & time_probe_1;
  #endif
  (void) 
  ({
    strlcpy (c->locals[c->nesting+1].function_log.msg, "ioblock end\n", MAXSTRINGLEN);
    function_log (c);
    if (unlikely(c->last_error)) goto out;
    (void) 0;
  });
  c->actionremaining -= 1;
  if (unlikely (c->actionremaining <= 0)) {
    c->last_error = "MAXACTION exceeded";
    c->last_stmt = "identifier 'log' at ioblock.stp:6:2";
    goto out;
  }
out:
  _stp_print_flush();
}


#ifdef STP_TIMING
static __cacheline_aligned Stat time_probe_2;
#endif

static void probe_2001 (struct context * __restrict__ c) {
  __label__ out;
  static const struct stp_probe_lock locks[] = {
  };
  struct probe_2001_locals * __restrict__ l = & c->probe_locals.probe_2001;
  (void) l;
  #ifndef STP_PRIVILEGED
  #error Internal Error: Probe kernel.function("generic_make_request@block/ll_rw_blk.c:3021") generated in --unprivileged mode
  #endif
  #ifdef STP_TIMING
  c->statp = & time_probe_2;
  #endif
  #if defined __ia64__
  bspcache(c->unwaddr, c->regs);
  #endif
  if (!stp_lock_probe(locks, ARRAY_SIZE(locks)))
    return;
  l->devname[0] = '\0';
  {
    (void) 
    ({
      strlcpy (l->__tmp0, 
      ({
        l->__tmp2 = 
        ({
          function__dwarf_tvar_get_bio_0 (c);
          if (unlikely(c->last_error)) goto out;
          c->locals[c->nesting+1].function__dwarf_tvar_get_bio_0.__retvalue;
        });
        c->locals[c->nesting+1].function___bio_devname.bio = l->__tmp2;
        function___bio_devname (c);
        if (unlikely(c->last_error)) goto out;
        c->locals[c->nesting+1].function___bio_devname.__retvalue;
      }), MAXSTRINGLEN);
      strlcpy (l->devname, l->__tmp0, MAXSTRINGLEN);
      l->__tmp0;
    });
    
    (void) 
    ({
      strlcpy (l->__tmp3, l->devname, MAXSTRINGLEN);
      _stp_printf ("devname = %s\n", l->__tmp3);
      ((int64_t)0LL);
    });
    
  }
  c->actionremaining -= 2;
  if (unlikely (c->actionremaining <= 0)) {
    c->last_error = "MAXACTION exceeded";
    c->last_stmt = "operator '{' at /usr/share/systemtap/tapset/ioblock.stp:96:1";
    goto out;
  }
out:
  stp_unlock_probe(locks, ARRAY_SIZE(locks));
  _stp_print_flush();
}

struct stap_probe {
  void (* const ph) (struct context*);
  const char * const pp;
  #ifdef STP_NEED_PROBE_NAME
  const char pn[18];
  #define STAP_PROBE_INIT(PH, PP, PN) { .ph=(PH), .pp=(PP), .pn=(PN) }
  #else
  #define STAP_PROBE_INIT(PH, PP, PN) { .ph=(PH), .pp=(PP) }
  #endif
};

/* ---- begin/end/error probes ---- */
static struct stap_be_probe {
  struct stap_probe probe;
  int state, type;
} stap_be_probes[] = {
  { .probe=STAP_PROBE_INIT(&probe_1998, "begin", "begin"), .state=STAP_SESSION_STARTING, .type=0 },
  { .probe=STAP_PROBE_INIT(&probe_1999, "end", "end"), .state=STAP_SESSION_STOPPING, .type=1 },
};
static void enter_be_probe (struct stap_be_probe *stp) {
  struct context* __restrict__ c;
  #if !INTERRUPTIBLE
  unsigned long flags;
  #endif
  #ifdef STP_TIMING
  cycles_t cycles_atstart = get_cycles ();
  #endif
  #if INTERRUPTIBLE
  preempt_disable ();
  #else
  local_irq_save (flags);
  #endif
  if (unlikely ((((unsigned long) (& c)) & (THREAD_SIZE-1))
    < (MINSTACKSPACE + sizeof (struct thread_info)))) {
    atomic_inc (& skipped_count);
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_lowstack);
    #endif
    goto probe_epilogue;
  }
  if (atomic_read (&session_state) != stp->state)
    goto probe_epilogue;
  c = contexts[smp_processor_id()];
  if (atomic_inc_return (& c->busy) != 1) {
    #if !INTERRUPTIBLE
    atomic_inc (& skipped_count);
    #endif
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_reentrant);
    #ifdef DEBUG_REENTRANCY
    _stp_warn ("Skipped %s due to %s residency on cpu %u\n", stp->probe.pp, c->probe_point ?: "?", smp_processor_id());
    #endif
    #endif
    atomic_dec (& c->busy);
    goto probe_epilogue;
  }
  
  c->last_stmt = 0;
  c->last_error = 0;
  c->nesting = -1;
  c->regs = 0;
  #if defined __ia64__
  c->unwaddr = 0;
  #endif
  c->probe_point = stp->probe.pp;
  #ifdef STP_NEED_PROBE_NAME
  c->probe_name = stp->probe.pn;
  #else
  c->probe_name = 0;
  #endif
  c->pi = 0;
  c->pi_longs = 0;
  c->regflags = 0;
  c->regparm = 0;
  c->marker_name = NULL;
  c->marker_format = NULL;
  #if INTERRUPTIBLE
  c->actionremaining = MAXACTION_INTERRUPTIBLE;
  #else
  c->actionremaining = MAXACTION;
  #endif
  #ifdef STP_TIMING
  c->statp = 0;
  #endif
  c->ri = 0;
  (*stp->probe.ph) (c);
  #ifdef STP_TIMING
  {
    cycles_t cycles_atend = get_cycles ();
    int32_t cycles_elapsed = ((int32_t)cycles_atend > (int32_t)cycles_atstart)
      ? ((int32_t)cycles_atend - (int32_t)cycles_atstart)
      : (~(int32_t)0) - (int32_t)cycles_atstart + (int32_t)cycles_atend + 1;
    #ifdef STP_TIMING
    if (likely (c->statp)) _stp_stat_add(*c->statp, cycles_elapsed);
    #endif
  }
  #endif
  c->probe_point = 0;
  c->probe_name = 0;
  if (unlikely (c->last_error && c->last_error[0])) {
    if (c->last_stmt != NULL)
      _stp_softerror ("%s near %s", c->last_error, c->last_stmt);
    else
      _stp_softerror ("%s", c->last_error);
    atomic_inc (& error_count);
    if (atomic_read (& error_count) > MAXERRORS) {
      atomic_set (& session_state, STAP_SESSION_ERROR);
      _stp_exit ();
    }
  }
  atomic_dec (&c->busy);
probe_epilogue:
  if (unlikely (atomic_read (& skipped_count) > MAXSKIPPED)) {
    if (unlikely (pseudo_atomic_cmpxchg(& session_state, STAP_SESSION_RUNNING, STAP_SESSION_ERROR) == STAP_SESSION_RUNNING))
    _stp_error ("Skipped too many probes, check MAXSKIPPED or try again with stap -t for more details.");
  }
  #if INTERRUPTIBLE
  preempt_enable_no_resched ();
  #else
  local_irq_restore (flags);
  #endif
}
/* ---- dwarf probes ---- */
#if ! defined(CONFIG_KPROBES)
#error "Need CONFIG_KPROBES!"
#endif

#ifndef KRETACTIVE
#define KRETACTIVE (max(15,6*(int)num_possible_cpus()))
#endif
#include "kprobes-common.h"
static int enter_kprobe_probe (struct kprobe *inst, struct pt_regs *regs);
static int enter_kretprobe_probe (struct kretprobe_instance *inst, struct pt_regs *regs);
#if defined(STAPCONF_UNREGISTER_KPROBES)
static void * stap_unreg_kprobes[1];
#endif
static struct stap_dwarf_kprobe stap_dwarf_kprobes[1];
static struct stap_dwarf_probe {
  const unsigned return_p:1;
  const unsigned maxactive_p:1;
  const unsigned optional_p:1;
  unsigned registered_p:1;
  const unsigned short maxactive_val;
  const unsigned short saved_longs;
  const unsigned short saved_strings;
  const char module[7];
  const char section[7];
  const unsigned long address;
  struct stap_probe probe;
  void (* const entry_ph) (struct context*);
  const unsigned long sdt_sem_offset;
  unsigned long sdt_sem_address;
  struct task_struct *tsk;
  const char *pathname;
  struct stap_task_finder_target finder;
} stap_dwarf_probes[] = {
  { .address=(unsigned long)0x19ef8ULL, .module="kernel", .section="_stext", .probe=STAP_PROBE_INIT(&probe_2001, "kernel.function(\"generic_make_request@block/ll_rw_blk.c:3021\")", "ioblock.request"), .sdt_sem_offset=0, },
};

static int enter_kprobe_probe (struct kprobe *inst, struct pt_regs *regs) {
  int kprobe_idx = ((uintptr_t)inst-(uintptr_t)stap_dwarf_kprobes)/sizeof(struct stap_dwarf_kprobe);
  struct stap_dwarf_probe *sdp = &stap_dwarf_probes[((kprobe_idx >= 0 && kprobe_idx < 1)?kprobe_idx:0)];
  struct context* __restrict__ c;
  #if !INTERRUPTIBLE
  unsigned long flags;
  #endif
  #if defined(STP_TIMING) || defined(STP_OVERLOAD)
  cycles_t cycles_atstart = get_cycles ();
  #endif
  #if INTERRUPTIBLE
  preempt_disable ();
  #else
  local_irq_save (flags);
  #endif
  if (unlikely ((((unsigned long) (& c)) & (THREAD_SIZE-1))
    < (MINSTACKSPACE + sizeof (struct thread_info)))) {
    atomic_inc (& skipped_count);
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_lowstack);
    #endif
    goto probe_epilogue;
  }
  if (atomic_read (&session_state) != STAP_SESSION_RUNNING)
    goto probe_epilogue;
  c = contexts[smp_processor_id()];
  if (atomic_inc_return (& c->busy) != 1) {
    #if !INTERRUPTIBLE
    atomic_inc (& skipped_count);
    #endif
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_reentrant);
    #ifdef DEBUG_REENTRANCY
    _stp_warn ("Skipped %s due to %s residency on cpu %u\n", sdp->probe.pp, c->probe_point ?: "?", smp_processor_id());
    #endif
    #endif
    atomic_dec (& c->busy);
    goto probe_epilogue;
  }
  
  c->last_stmt = 0;
  c->last_error = 0;
  c->nesting = -1;
  c->regs = 0;
  #if defined __ia64__
  c->unwaddr = 0;
  #endif
  c->probe_point = sdp->probe.pp;
  #ifdef STP_NEED_PROBE_NAME
  c->probe_name = sdp->probe.pn;
  #else
  c->probe_name = 0;
  #endif
  c->pi = 0;
  c->pi_longs = 0;
  c->regflags = 0;
  c->regparm = 0;
  c->marker_name = NULL;
  c->marker_format = NULL;
  #if INTERRUPTIBLE
  c->actionremaining = MAXACTION_INTERRUPTIBLE;
  #else
  c->actionremaining = MAXACTION;
  #endif
  #ifdef STP_TIMING
  c->statp = 0;
  #endif
  c->ri = 0;
  c->regs = regs;
  {
    unsigned long kprobes_ip = REG_IP(c->regs);
    SET_REG_IP(regs, (unsigned long) inst->addr);
    (*sdp->probe.ph) (c);
    SET_REG_IP(regs, kprobes_ip);
  }
  #if defined(STP_TIMING) || defined(STP_OVERLOAD)
  {
    cycles_t cycles_atend = get_cycles ();
    int32_t cycles_elapsed = ((int32_t)cycles_atend > (int32_t)cycles_atstart)
      ? ((int32_t)cycles_atend - (int32_t)cycles_atstart)
      : (~(int32_t)0) - (int32_t)cycles_atstart + (int32_t)cycles_atend + 1;
    #ifdef STP_TIMING
    if (likely (c->statp)) _stp_stat_add(*c->statp, cycles_elapsed);
    #endif
    #ifdef STP_OVERLOAD
    {
      cycles_t interval = (cycles_atend > c->cycles_base)
        ? (cycles_atend - c->cycles_base)
        : (STP_OVERLOAD_INTERVAL + 1);
      c->cycles_sum += cycles_elapsed;
      if (interval > STP_OVERLOAD_INTERVAL) {
        if (c->cycles_sum > STP_OVERLOAD_THRESHOLD) {
          _stp_error ("probe overhead exceeded threshold");
          atomic_set (&session_state, STAP_SESSION_ERROR);
          atomic_inc (&error_count);
        }
        c->cycles_base = cycles_atend;
        c->cycles_sum = 0;
      }
    }
    #endif
  }
  #endif
  c->probe_point = 0;
  c->probe_name = 0;
  if (unlikely (c->last_error && c->last_error[0])) {
    if (c->last_stmt != NULL)
      _stp_softerror ("%s near %s", c->last_error, c->last_stmt);
    else
      _stp_softerror ("%s", c->last_error);
    atomic_inc (& error_count);
    if (atomic_read (& error_count) > MAXERRORS) {
      atomic_set (& session_state, STAP_SESSION_ERROR);
      _stp_exit ();
    }
  }
  atomic_dec (&c->busy);
probe_epilogue:
  if (unlikely (atomic_read (& skipped_count) > MAXSKIPPED)) {
    if (unlikely (pseudo_atomic_cmpxchg(& session_state, STAP_SESSION_RUNNING, STAP_SESSION_ERROR) == STAP_SESSION_RUNNING))
    _stp_error ("Skipped too many probes, check MAXSKIPPED or try again with stap -t for more details.");
  }
  #if INTERRUPTIBLE
  preempt_enable_no_resched ();
  #else
  local_irq_restore (flags);
  #endif
  return 0;
}

static int enter_kretprobe_common (struct kretprobe_instance *inst, struct pt_regs *regs, int entry) {
  struct kretprobe *krp = inst->rp;
  int kprobe_idx = ((uintptr_t)krp-(uintptr_t)stap_dwarf_kprobes)/sizeof(struct stap_dwarf_kprobe);
  struct stap_dwarf_probe *sdp = &stap_dwarf_probes[((kprobe_idx >= 0 && kprobe_idx < 1)?kprobe_idx:0)];
  struct context* __restrict__ c;
  #if !INTERRUPTIBLE
  unsigned long flags;
  #endif
  #if defined(STP_TIMING) || defined(STP_OVERLOAD)
  cycles_t cycles_atstart = get_cycles ();
  #endif
  #if INTERRUPTIBLE
  preempt_disable ();
  #else
  local_irq_save (flags);
  #endif
  if (unlikely ((((unsigned long) (& c)) & (THREAD_SIZE-1))
    < (MINSTACKSPACE + sizeof (struct thread_info)))) {
    atomic_inc (& skipped_count);
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_lowstack);
    #endif
    goto probe_epilogue;
  }
  if (atomic_read (&session_state) != STAP_SESSION_RUNNING)
    goto probe_epilogue;
  c = contexts[smp_processor_id()];
  if (atomic_inc_return (& c->busy) != 1) {
    #if !INTERRUPTIBLE
    atomic_inc (& skipped_count);
    #endif
    #ifdef STP_TIMING
    atomic_inc (& skipped_count_reentrant);
    #ifdef DEBUG_REENTRANCY
    _stp_warn ("Skipped %s due to %s residency on cpu %u\n", sdp->probe.pp, c->probe_point ?: "?", smp_processor_id());
    #endif
    #endif
    atomic_dec (& c->busy);
    goto probe_epilogue;
  }
  
  c->last_stmt = 0;
  c->last_error = 0;
  c->nesting = -1;
  c->regs = 0;
  #if defined __ia64__
  c->unwaddr = 0;
  #endif
  c->probe_point = sdp->probe.pp;
  #ifdef STP_NEED_PROBE_NAME
  c->probe_name = sdp->probe.pn;
  #else
  c->probe_name = 0;
  #endif
  c->pi = 0;
  c->pi_longs = 0;
  c->regflags = 0;
  c->regparm = 0;
  c->marker_name = NULL;
  c->marker_format = NULL;
  #if INTERRUPTIBLE
  c->actionremaining = MAXACTION_INTERRUPTIBLE;
  #else
  c->actionremaining = MAXACTION;
  #endif
  #ifdef STP_TIMING
  c->statp = 0;
  #endif
  c->ri = 0;
  c->regs = regs;
  c->pi = inst;
  c->pi_longs = sdp->saved_longs;
  {
    unsigned long kprobes_ip = REG_IP(c->regs);
    if (entry) {
      SET_REG_IP(regs, (unsigned long) inst->rp->kp.addr);
      (sdp->entry_ph) (c);
    } else {
      SET_REG_IP(regs, (unsigned long)inst->ret_addr);
      (sdp->probe.ph) (c);
    }
    SET_REG_IP(regs, kprobes_ip);
  }
  #if defined(STP_TIMING) || defined(STP_OVERLOAD)
  {
    cycles_t cycles_atend = get_cycles ();
    int32_t cycles_elapsed = ((int32_t)cycles_atend > (int32_t)cycles_atstart)
      ? ((int32_t)cycles_atend - (int32_t)cycles_atstart)
      : (~(int32_t)0) - (int32_t)cycles_atstart + (int32_t)cycles_atend + 1;
    #ifdef STP_TIMING
    if (likely (c->statp)) _stp_stat_add(*c->statp, cycles_elapsed);
    #endif
    #ifdef STP_OVERLOAD
    {
      cycles_t interval = (cycles_atend > c->cycles_base)
        ? (cycles_atend - c->cycles_base)
        : (STP_OVERLOAD_INTERVAL + 1);
      c->cycles_sum += cycles_elapsed;
      if (interval > STP_OVERLOAD_INTERVAL) {
        if (c->cycles_sum > STP_OVERLOAD_THRESHOLD) {
          _stp_error ("probe overhead exceeded threshold");
          atomic_set (&session_state, STAP_SESSION_ERROR);
          atomic_inc (&error_count);
        }
        c->cycles_base = cycles_atend;
        c->cycles_sum = 0;
      }
    }
    #endif
  }
  #endif
  c->probe_point = 0;
  c->probe_name = 0;
  if (unlikely (c->last_error && c->last_error[0])) {
    if (c->last_stmt != NULL)
      _stp_softerror ("%s near %s", c->last_error, c->last_stmt);
    else
      _stp_softerror ("%s", c->last_error);
    atomic_inc (& error_count);
    if (atomic_read (& error_count) > MAXERRORS) {
      atomic_set (& session_state, STAP_SESSION_ERROR);
      _stp_exit ();
    }
  }
  atomic_dec (&c->busy);
probe_epilogue:
  if (unlikely (atomic_read (& skipped_count) > MAXSKIPPED)) {
    if (unlikely (pseudo_atomic_cmpxchg(& session_state, STAP_SESSION_RUNNING, STAP_SESSION_ERROR) == STAP_SESSION_RUNNING))
    _stp_error ("Skipped too many probes, check MAXSKIPPED or try again with stap -t for more details.");
  }
  #if INTERRUPTIBLE
  preempt_enable_no_resched ();
  #else
  local_irq_restore (flags);
  #endif
  return 0;
}

static int enter_kretprobe_probe (struct kretprobe_instance *inst, struct pt_regs *regs) {
  return enter_kretprobe_common(inst, regs, 0);
}

static int enter_kretprobe_entry_probe (struct kretprobe_instance *inst, struct pt_regs *regs) {
  return enter_kretprobe_common(inst, regs, 1);
}

#include "kprobes-common.c"


static int systemtap_module_init (void) {
  int rc = 0;
  int cpu;
  int i=0, j=0;
  const char *probe_point = "";
  {
    const char* release = UTS_RELEASE;
    if (strcmp (release, "2.6.18-baseline")) {
      _stp_error ("module release mismatch (%s vs %s)", release, "2.6.18-baseline");
      rc = -EINVAL;
    }
    if (_stp_module_check()) rc = -EINVAL;
  }
  if (rc) goto out;
  #ifdef STAP_NEED_GETTIMEOFDAY
  rc = _stp_init_time();
  if (rc) {
    _stp_error ("couldn't initialize gettimeofday");
    goto out;
  }
  #endif
  (void) probe_point;
  (void) i;
  (void) j;
  atomic_set (&session_state, STAP_SESSION_STARTING);
  for_each_possible_cpu(cpu) {
    contexts[cpu] = _stp_kzalloc(sizeof(struct context));
    if (contexts[cpu] == NULL) {
      _stp_error ("context (size %lu) allocation failed", (unsigned long) sizeof (struct context));
      rc = -ENOMEM;
      goto out;
    }
  }
  #ifdef STP_TIMING
  time_probe_0 = _stp_stat_init (HIST_NONE);
  time_probe_1 = _stp_stat_init (HIST_NONE);
  time_probe_2 = _stp_stat_init (HIST_NONE);
  #endif
  _stp_print_kernel_info("1.3/0.148", (num_online_cpus() * sizeof(struct context)), 3);
  for (i=0; i<2; i++) {
    struct stap_be_probe* stp = & stap_be_probes [i];
    if (stp->type == 0)
      enter_be_probe (stp); /* rc = 0 */
  }
  if (rc) {
    _stp_error ("probe %s registration error (rc %d)", probe_point, rc);
    atomic_set (&session_state, STAP_SESSION_ERROR);
    goto out;
  }
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
    unsigned long relocated_addr = _stp_kmodule_relocate (sdp->module, sdp->section, sdp->address);
    if (relocated_addr == 0) continue;
    probe_point = sdp->probe.pp;
    if (sdp->return_p) {
      kp->u.krp.kp.addr = (void *) relocated_addr;
      if (sdp->maxactive_p) {
        kp->u.krp.maxactive = sdp->maxactive_val;
      } else {
        kp->u.krp.maxactive = KRETACTIVE;
      }
      kp->u.krp.handler = &enter_kretprobe_probe;
      #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25)
      if (sdp->entry_ph) {
        kp->u.krp.entry_handler = &enter_kretprobe_entry_probe;
        kp->u.krp.data_size = sdp->saved_longs * sizeof(int64_t) + 
                              sdp->saved_strings * MAXSTRINGLEN;
      }
      #endif
      #ifdef __ia64__
      kp->dummy.addr = kp->u.krp.kp.addr;
      kp->dummy.pre_handler = NULL;
      rc = register_kprobe (& kp->dummy);
      if (rc == 0) {
        rc = register_kretprobe (& kp->u.krp);
        if (rc != 0)
          unregister_kprobe (& kp->dummy);
      }
      #else
      rc = register_kretprobe (& kp->u.krp);
      #endif
    } else {
      kp->u.kp.addr = (void *) relocated_addr;
      kp->u.kp.pre_handler = &enter_kprobe_probe;
      #ifdef __ia64__
      kp->dummy.addr = kp->u.kp.addr;
      kp->dummy.pre_handler = NULL;
      rc = register_kprobe (& kp->dummy);
      if (rc == 0) {
        rc = register_kprobe (& kp->u.kp);
        if (rc != 0)
          unregister_kprobe (& kp->dummy);
      }
      #else
      rc = register_kprobe (& kp->u.kp);
      #endif
    }
    if (rc) {
      sdp->registered_p = 0;
      if (!sdp->optional_p)
        _stp_warn ("probe %s (address 0x%lx) registration error (rc %d)", probe_point, (unsigned long) relocated_addr, rc);
      rc = 0;
    }
    else sdp->registered_p = 1;
  }
  if (rc) {
    _stp_error ("probe %s registration error (rc %d)", probe_point, rc);
    atomic_set (&session_state, STAP_SESSION_ERROR);
    for (i=0; i<2; i++) {
      struct stap_be_probe* stp = & stap_be_probes [i];
      if (stp->type == 1)
        enter_be_probe (stp);
    }
    for (i=0; i<2; i++) {
      struct stap_be_probe* stp = & stap_be_probes [i];
      if (stp->type == 2)
        enter_be_probe (stp);
    }
    goto out;
  }
  
  /* ---- task finder ---- */
  if (rc == 0) {
    rc = stap_start_task_finder();
    if (rc) {
      stap_stop_task_finder();
    }
  }
  if (rc) {
    _stp_error ("probe %s registration error (rc %d)", probe_point, rc);
    atomic_set (&session_state, STAP_SESSION_ERROR);
    for (i=0; i<1; i++) {
      struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
      unsigned short sdt_semaphore = 0;
      if (sdp->sdt_sem_address && __access_process_vm_noflush (sdp->tsk, sdp->sdt_sem_address, &sdt_semaphore, sizeof (sdt_semaphore), 0)) {
        sdt_semaphore --;
        __access_process_vm_noflush (sdp->tsk, sdp->sdt_sem_address, &sdt_semaphore, sizeof (sdt_semaphore), 1);
      }
    }
    #if defined(STAPCONF_UNREGISTER_KPROBES)
    j = 0;
    for (i=0; i<1; i++) {
      struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
      struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
      if (! sdp->registered_p) continue;
      if (!sdp->return_p)
        stap_unreg_kprobes[j++] = &kp->u.kp;
    }
    unregister_kprobes((struct kprobe **)stap_unreg_kprobes, j);
    j = 0;
    for (i=0; i<1; i++) {
      struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
      struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
      if (! sdp->registered_p) continue;
      if (sdp->return_p)
        stap_unreg_kprobes[j++] = &kp->u.krp;
    }
    unregister_kretprobes((struct kretprobe **)stap_unreg_kprobes, j);
    #ifdef __ia64__
    j = 0;
    for (i=0; i<1; i++) {
      struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
      struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
      if (! sdp->registered_p) continue;
      stap_unreg_kprobes[j++] = &kp->dummy;
    }
    unregister_kprobes((struct kprobe **)stap_unreg_kprobes, j);
    #endif
    #endif
    for (i=0; i<1; i++) {
      struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
      struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
      if (! sdp->registered_p) continue;
      if (sdp->return_p) {
      #if !defined(STAPCONF_UNREGISTER_KPROBES)
        unregister_kretprobe (&kp->u.krp);
        #endif
        atomic_add (kp->u.krp.nmissed, & skipped_count);
        #ifdef STP_TIMING
        if (kp->u.krp.nmissed)
          _stp_warn ("Skipped due to missed kretprobe/1 on '%s': %d\n", sdp->probe.pp, kp->u.krp.nmissed);
        #endif
        atomic_add (kp->u.krp.kp.nmissed, & skipped_count);
        #ifdef STP_TIMING
        if (kp->u.krp.kp.nmissed)
          _stp_warn ("Skipped due to missed kretprobe/2 on '%s': %lu\n", sdp->probe.pp, kp->u.krp.kp.nmissed);
        #endif
      } else {
      #if !defined(STAPCONF_UNREGISTER_KPROBES)
        unregister_kprobe (&kp->u.kp);
        #endif
        atomic_add (kp->u.kp.nmissed, & skipped_count);
        #ifdef STP_TIMING
        if (kp->u.kp.nmissed)
          _stp_warn ("Skipped due to missed kprobe on '%s': %lu\n", sdp->probe.pp, kp->u.kp.nmissed);
        #endif
      }
      #if !defined(STAPCONF_UNREGISTER_KPROBES) && defined(__ia64__)
      unregister_kprobe (&kp->dummy);
      #endif
      sdp->registered_p = 0;
    }
    for (i=0; i<2; i++) {
      struct stap_be_probe* stp = & stap_be_probes [i];
      if (stp->type == 1)
        enter_be_probe (stp);
    }
    for (i=0; i<2; i++) {
      struct stap_be_probe* stp = & stap_be_probes [i];
      if (stp->type == 2)
        enter_be_probe (stp);
    }
    goto out;
  }
  if (atomic_read (&session_state) == STAP_SESSION_STARTING)
    atomic_set (&session_state, STAP_SESSION_RUNNING);
  return 0;
out:
  #ifdef STAPCONF_SYNCHRONIZE_SCHED
  synchronize_sched();
  #endif
  #ifdef STAP_NEED_GETTIMEOFDAY
   _stp_kill_time();
  #endif
  for_each_possible_cpu(cpu) {
    if (contexts[cpu] != NULL) {
      _stp_kfree(contexts[cpu]);
      contexts[cpu] = NULL;
    }
  }
  return rc;
}


static void systemtap_module_exit (void) {
  int holdon;
  int i=0, j=0;
  int cpu;
  (void) i;
  (void) j;
  if (atomic_read (&session_state) == STAP_SESSION_STARTING)
    return;
  if (atomic_read (&session_state) == STAP_SESSION_RUNNING)
    atomic_set (&session_state, STAP_SESSION_STOPPING);
  
  /* ---- task finder ---- */
  stap_stop_task_finder();
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    unsigned short sdt_semaphore = 0;
    if (sdp->sdt_sem_address && __access_process_vm_noflush (sdp->tsk, sdp->sdt_sem_address, &sdt_semaphore, sizeof (sdt_semaphore), 0)) {
      sdt_semaphore --;
      __access_process_vm_noflush (sdp->tsk, sdp->sdt_sem_address, &sdt_semaphore, sizeof (sdt_semaphore), 1);
    }
  }
  #if defined(STAPCONF_UNREGISTER_KPROBES)
  j = 0;
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
    if (! sdp->registered_p) continue;
    if (!sdp->return_p)
      stap_unreg_kprobes[j++] = &kp->u.kp;
  }
  unregister_kprobes((struct kprobe **)stap_unreg_kprobes, j);
  j = 0;
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
    if (! sdp->registered_p) continue;
    if (sdp->return_p)
      stap_unreg_kprobes[j++] = &kp->u.krp;
  }
  unregister_kretprobes((struct kretprobe **)stap_unreg_kprobes, j);
  #ifdef __ia64__
  j = 0;
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
    if (! sdp->registered_p) continue;
    stap_unreg_kprobes[j++] = &kp->dummy;
  }
  unregister_kprobes((struct kprobe **)stap_unreg_kprobes, j);
  #endif
  #endif
  for (i=0; i<1; i++) {
    struct stap_dwarf_probe *sdp = & stap_dwarf_probes[i];
    struct stap_dwarf_kprobe *kp = & stap_dwarf_kprobes[i];
    if (! sdp->registered_p) continue;
    if (sdp->return_p) {
    #if !defined(STAPCONF_UNREGISTER_KPROBES)
      unregister_kretprobe (&kp->u.krp);
      #endif
      atomic_add (kp->u.krp.nmissed, & skipped_count);
      #ifdef STP_TIMING
      if (kp->u.krp.nmissed)
        _stp_warn ("Skipped due to missed kretprobe/1 on '%s': %d\n", sdp->probe.pp, kp->u.krp.nmissed);
      #endif
      atomic_add (kp->u.krp.kp.nmissed, & skipped_count);
      #ifdef STP_TIMING
      if (kp->u.krp.kp.nmissed)
        _stp_warn ("Skipped due to missed kretprobe/2 on '%s': %lu\n", sdp->probe.pp, kp->u.krp.kp.nmissed);
      #endif
    } else {
    #if !defined(STAPCONF_UNREGISTER_KPROBES)
      unregister_kprobe (&kp->u.kp);
      #endif
      atomic_add (kp->u.kp.nmissed, & skipped_count);
      #ifdef STP_TIMING
      if (kp->u.kp.nmissed)
        _stp_warn ("Skipped due to missed kprobe on '%s': %lu\n", sdp->probe.pp, kp->u.kp.nmissed);
      #endif
    }
    #if !defined(STAPCONF_UNREGISTER_KPROBES) && defined(__ia64__)
    unregister_kprobe (&kp->dummy);
    #endif
    sdp->registered_p = 0;
  }
  for (i=0; i<2; i++) {
    struct stap_be_probe* stp = & stap_be_probes [i];
    if (stp->type == 1)
      enter_be_probe (stp);
  }
  for (i=0; i<2; i++) {
    struct stap_be_probe* stp = & stap_be_probes [i];
    if (stp->type == 2)
      enter_be_probe (stp);
  }
  #ifdef STAPCONF_SYNCHRONIZE_SCHED
  synchronize_sched();
  #endif
  do {
    int i;
    holdon = 0;
    for (i=0; i < NR_CPUS; i++)
      if (cpu_possible (i) && contexts[i] != NULL && atomic_read (& contexts[i]->busy)) holdon = 1;
      yield ();
  } while (holdon);
  #ifdef STAPCONF_SYNCHRONIZE_SCHED
  synchronize_sched();
  #endif
  for_each_possible_cpu(cpu) {
    if (contexts[cpu] != NULL) {
      _stp_kfree(contexts[cpu]);
      contexts[cpu] = NULL;
    }
  }
  #ifdef STP_TIMING
  {
    if (likely (time_probe_0)) {
      const char *probe_point = "begin";
      const char *decl_location = "ioblock.stp:1:1";
      struct stat_data *stats = _stp_stat_get (time_probe_0, 0);
      if (stats->count) {
        int64_t avg = _stp_div64 (NULL, stats->sum, stats->count);
        _stp_printf ("probe %s (%s), hits: %lld, cycles: %lldmin/%lldavg/%lldmax\n",
        probe_point, decl_location, (long long) stats->count, (long long) stats->min, (long long) avg, (long long) stats->max);
      }
      _stp_stat_del (time_probe_0);
    }
    if (likely (time_probe_1)) {
      const char *probe_point = "end";
      const char *decl_location = "ioblock.stp:5:1";
      struct stat_data *stats = _stp_stat_get (time_probe_1, 0);
      if (stats->count) {
        int64_t avg = _stp_div64 (NULL, stats->sum, stats->count);
        _stp_printf ("probe %s (%s), hits: %lld, cycles: %lldmin/%lldavg/%lldmax\n",
        probe_point, decl_location, (long long) stats->count, (long long) stats->min, (long long) avg, (long long) stats->max);
      }
      _stp_stat_del (time_probe_1);
    }
    if (likely (time_probe_2)) {
      const char *probe_point = "ioblock.request";
      const char *decl_location = "ioblock.stp:9:1";
      struct stat_data *stats = _stp_stat_get (time_probe_2, 0);
      if (stats->count) {
        int64_t avg = _stp_div64 (NULL, stats->sum, stats->count);
        _stp_printf ("probe %s (%s), hits: %lld, cycles: %lldmin/%lldavg/%lldmax\n",
        probe_point, decl_location, (long long) stats->count, (long long) stats->min, (long long) avg, (long long) stats->max);
      }
      _stp_stat_del (time_probe_2);
    }
    _stp_print_flush();
  }
  #endif
  #ifdef STAP_NEED_GETTIMEOFDAY
   _stp_kill_time();
  #endif
  if (atomic_read (& skipped_count) || atomic_read (& error_count) || atomic_read (& skipped_count_reentrant)) {
    _stp_warn ("Number of errors: %d, skipped probes: %d\n", (int) atomic_read (& error_count), (int) atomic_read (& skipped_count));
    #ifdef STP_TIMING
    {
      int ctr;
      ctr = atomic_read (& skipped_count_lowstack);
      if (ctr) _stp_warn ("Skipped due to low stack: %d\n", ctr);
      ctr = atomic_read (& skipped_count_reentrant);
      if (ctr) _stp_warn ("Skipped due to reentrancy: %d\n", ctr);
      ctr = atomic_read (& skipped_count_uprobe_reg);
      if (ctr) _stp_warn ("Skipped due to uprobe register failure: %d\n", ctr);
      ctr = atomic_read (& skipped_count_uprobe_unreg);
      if (ctr) _stp_warn ("Skipped due to uprobe unregister failure: %d\n", ctr);
    }
    #endif
    _stp_print_flush();
  }
}


static int probe_start (void) {
  return systemtap_module_init () ? -1 : 0;
}

static void probe_exit (void) {
  systemtap_module_exit ();
}
#include "stap-symbols.h"
MODULE_DESCRIPTION("systemtap-generated probe");
MODULE_LICENSE("GPL");
