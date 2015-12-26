_KBUILD_CFLAGS := $(call flags,KBUILD_CFLAGS)
stap_check_gcc = $(shell  if $(CC) $(1) -S -o /dev/null -xc /dev/null > /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi)
CHECK_BUILD := $(CC) $(KBUILD_CPPFLAGS) $(CPPFLAGS) $(LINUXINCLUDE) $(_KBUILD_CFLAGS) $(CFLAGS_KERNEL) $(EXTRA_CFLAGS) $(CFLAGS) -DKBUILD_BASENAME=\"stap_7146\" -Werror -S -o /dev/null -xc 
stap_check_build = $(shell  if $(CHECK_BUILD) $(1) > /dev/null 2>&1 ; then echo "$(2)"; else echo "$(3)"; fi)
SYSTEMTAP_RUNTIME = "/usr/share/systemtap/runtime"
CONFIG_MODULE_SIG := n
EXTRA_CFLAGS :=
EXTRA_CFLAGS += -Iinclude2/asm/mach-default
STAPCONF_HEADER := `pwd`/stapconf_0b7c8e56450ca786d6781bf3c34e02b9_483.h
/tmp/stapeXQgXD/stap_7146.c: $(STAPCONF_HEADER)
$(STAPCONF_HEADER):
	@echo -n > $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-hrtimer-rel.c > /dev/null 2>&1; then echo "#define STAPCONF_HRTIMER_REL 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-hrtimer-getset-expires.c > /dev/null 2>&1; then echo "#define STAPCONF_HRTIMER_GETSET_EXPIRES 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-inode-private.c > /dev/null 2>&1; then echo "#define STAPCONF_INODE_PRIVATE 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-constant-tsc.c > /dev/null 2>&1; then echo "#define STAPCONF_CONSTANT_TSC 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-tsc-khz.c > /dev/null 2>&1; then echo "#define STAPCONF_TSC_KHZ 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-ktime-get-real.c > /dev/null 2>&1; then echo "#define STAPCONF_KTIME_GET_REAL 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-x86-uniregs.c > /dev/null 2>&1; then echo "#define STAPCONF_X86_UNIREGS 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-nameidata.c > /dev/null 2>&1; then echo "#define STAPCONF_NAMEIDATA_CLEANUP 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-unregister-kprobes.c > /dev/null 2>&1; then echo "#define STAPCONF_UNREGISTER_KPROBES 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-real-parent.c > /dev/null 2>&1; then echo "#define STAPCONF_REAL_PARENT 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-uaccess.c > /dev/null 2>&1; then echo "#define STAPCONF_LINUX_UACCESS_H 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-oneachcpu-retry.c > /dev/null 2>&1; then echo "#define STAPCONF_ONEACHCPU_RETRY 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-dpath-path.c > /dev/null 2>&1; then echo "#define STAPCONF_DPATH_PATH 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-synchronize-sched.c > /dev/null 2>&1; then echo "#define STAPCONF_SYNCHRONIZE_SCHED 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-task-uid.c > /dev/null 2>&1; then echo "#define STAPCONF_TASK_UID 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-vm-area.c > /dev/null 2>&1; then echo "#define STAPCONF_VM_AREA 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-procfs-owner.c > /dev/null 2>&1; then echo "#define STAPCONF_PROCFS_OWNER 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-alloc-percpu-align.c > /dev/null 2>&1; then echo "#define STAPCONF_ALLOC_PERCPU_ALIGN 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-x86-gs.c > /dev/null 2>&1; then echo "#define STAPCONF_X86_GS 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-grsecurity.c > /dev/null 2>&1; then echo "#define STAPCONF_GRSECURITY 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-trace-printk.c > /dev/null 2>&1; then echo "#define STAPCONF_TRACE_PRINTK 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-regset.c > /dev/null 2>&1; then echo "#define STAPCONF_REGSET 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-utrace-regset.c > /dev/null 2>&1; then echo "#define STAPCONF_UTRACE_REGSET 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-uprobe-get-pc.c > /dev/null 2>&1; then echo "#define STAPCONF_UPROBE_GET_PC 1"; fi >> $@
	@echo "#define STAPCONF_CPU_KHZ 1">> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-probe-kernel.c > /dev/null 2>&1; then echo "#define STAPCONF_PROBE_KERNEL 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-save-stack-trace.c > /dev/null 2>&1; then echo "#define STAPCONF_KERNEL_STACKTRACE 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-asm-syscall.c > /dev/null 2>&1; then echo "#define STAPCONF_ASM_SYSCALL_H 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-ring_buffer-flags.c > /dev/null 2>&1; then echo "#define STAPCONF_RING_BUFFER_FLAGS 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-kallsyms-on-each-symbol.c > /dev/null 2>&1; then echo "#define STAPCONF_KALLSYMS_ON_EACH_SYMBOL 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-walk-stack.c > /dev/null 2>&1; then echo "#define STAPCONF_WALK_STACK 1"; fi >> $@
	@if $(CHECK_BUILD) $(SYSTEMTAP_RUNTIME)/autoconf-mm-context-vdso.c > /dev/null 2>&1; then echo "#define STAPCONF_MM_CONTEXT_VDSO 1"; fi >> $@
EXTRA_CFLAGS += -include $(STAPCONF_HEADER)
EXTRA_CFLAGS += -freorder-blocks
EXTRA_CFLAGS += $(call cc-option,-Wframe-larger-than=600)
EXTRA_CFLAGS += -Wno-unused -Werror
EXTRA_CFLAGS += -I"/usr/share/systemtap/runtime"
obj-m := stap_7146.o
