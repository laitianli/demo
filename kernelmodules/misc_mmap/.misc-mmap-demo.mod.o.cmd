cmd_/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o := gcc -Wp,-MD,/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/.misc-mmap-demo.mod.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-redhat-linux/4.1.2/include -Iinclude  -I/home/laitianli/kernel/linux-2.6.32.9/arch/x86/include -include include/linux/autoconf.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m64 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -fstack-protector -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -pg -Wdeclaration-after-statement -Wno-pointer-sign  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(misc_mmap_demo.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(misc_mmap_demo)" -D"DEBUG_HASH=4" -D"DEBUG_HASH2=9" -DMODULE -c -o /home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o /home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.c

deps_/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o := \
  /home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.c \
    $(wildcard include/config/module/unload.h) \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/poison.h \
  include/linux/prefetch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/types.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/highmem64g.h) \
  include/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/posix_types_64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/processor.h \
    $(wildcard include/config/x86/vsmp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/m386.h) \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/vm86.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ptrace.h \
    $(wildcard include/config/x86/ptrace/bts.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ptrace-abi.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/segment.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
  include/linux/linkage.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/alignment/16.h) \
  include/linux/stringify.h \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/math_emu.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/sigcontext.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/current.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
  /usr/lib/gcc/x86_64-redhat-linux/4.1.2/include/stdarg.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/x86/cmov.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/alternative.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/asm.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/cpufeature.h \
    $(wildcard include/config/x86/invlpg.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/swab.h \
    $(wildcard include/config/x86/bswap.h) \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/asm-generic/percpu.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/system.h \
    $(wildcard include/config/ia32/emulation.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
    $(wildcard include/config/x86/ppro/fence.h) \
    $(wildcard include/config/x86/oostore.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/cmpxchg.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/cmpxchg_64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/irqflags.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/paravirt.h \
    $(wildcard include/config/highpte.h) \
    $(wildcard include/config/paravirt/spinlocks.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  include/linux/const.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/page_types.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/page_64_types.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
    $(wildcard include/config/flatmem.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/pgtable_64_types.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/paravirt/debug.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/desc_defs.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  include/asm-generic/kmap_types.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/string.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/string_64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/page.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/page_64.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/asm-generic/getorder.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/msr.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/msr-index.h \
  include/linux/ioctl.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/cpumask.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ds.h \
    $(wildcard include/config/x86/ds.h) \
  include/linux/err.h \
  include/linux/personality.h \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/math64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/stat.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ftrace.h \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/atomic.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/atomic_64.h \
  include/asm-generic/atomic-long.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/spinlock.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/rwlock.h \
  include/linux/spinlock_api_smp.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/mmzone.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/mmzone_64.h \
    $(wildcard include/config/numa/emu.h) \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/smp.h \
    $(wildcard include/config/x86/io/apic.h) \
    $(wildcard include/config/x86/32/smp.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/mpspec.h \
    $(wildcard include/config/x86/numaq.h) \
    $(wildcard include/config/mca.h) \
    $(wildcard include/config/eisa.h) \
    $(wildcard include/config/x86/mpparse.h) \
    $(wildcard include/config/acpi.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/mpspec_def.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/x86_init.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/bootparam.h \
  include/linux/screen_info.h \
  include/linux/apm_bios.h \
  include/linux/edd.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/e820.h \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memtest.h) \
  include/linux/ioport.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/ist.h \
  include/video/edid.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/apic.h \
    $(wildcard include/config/x86/x2apic.h) \
  include/linux/delay.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/delay.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
  include/linux/workqueue.h \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
    $(wildcard include/config/no/hz.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/timex.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/tsc.h \
    $(wildcard include/config/x86/tsc.h) \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/apicdef.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/fixmap.h \
    $(wildcard include/config/x86/visws/apic.h) \
    $(wildcard include/config/x86/f00f/bug.h) \
    $(wildcard include/config/x86/cyclone/timer.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/provide/ohci1394/dma/init.h) \
    $(wildcard include/config/intel/txt.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/acpi.h \
    $(wildcard include/config/acpi/numa.h) \
  include/acpi/pdc_intel.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/numa.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/numa_64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/mmu.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/vsyscall.h \
    $(wildcard include/config/generic/time.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/io_apic.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/irq_vectors.h \
    $(wildcard include/config/sparse/irq.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/sparsemem.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/topology.h \
    $(wildcard include/config/x86/ht.h) \
    $(wildcard include/config/x86/64/acpi/numa.h) \
  include/asm-generic/topology.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/elf.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/user.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/user_64.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/auxvec.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/vdso.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/local.h \
  include/linux/percpu.h \
    $(wildcard include/config/have/legacy/per/cpu/area.h) \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/debug/kmemleak.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
    $(wildcard include/config/kmemtrace.h) \
  include/linux/kmemtrace.h \
  include/trace/events/kmem.h \
  include/trace/define_trace.h \
  include/linux/kmalloc_sizes.h \
  include/linux/pfn.h \
  /home/laitianli/kernel/linux-2.6.32.9/arch/x86/include/asm/module.h \
    $(wildcard include/config/m586.h) \
    $(wildcard include/config/m586tsc.h) \
    $(wildcard include/config/m586mmx.h) \
    $(wildcard include/config/mcore2.h) \
    $(wildcard include/config/matom.h) \
    $(wildcard include/config/m686.h) \
    $(wildcard include/config/mpentiumii.h) \
    $(wildcard include/config/mpentiumiii.h) \
    $(wildcard include/config/mpentiumm.h) \
    $(wildcard include/config/mpentium4.h) \
    $(wildcard include/config/mk6.h) \
    $(wildcard include/config/mk8.h) \
    $(wildcard include/config/x86/elan.h) \
    $(wildcard include/config/mcrusoe.h) \
    $(wildcard include/config/mefficeon.h) \
    $(wildcard include/config/mwinchipc6.h) \
    $(wildcard include/config/mwinchip3d.h) \
    $(wildcard include/config/mcyrixiii.h) \
    $(wildcard include/config/mviac3/2.h) \
    $(wildcard include/config/mviac7.h) \
    $(wildcard include/config/mgeodegx1.h) \
    $(wildcard include/config/mgeode/lx.h) \
    $(wildcard include/config/4kstacks.h) \
  include/asm-generic/module.h \
  include/trace/events/module.h \
  include/linux/vermagic.h \
  include/linux/utsrelease.h \

/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o: $(deps_/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o)

$(deps_/home/laitianli/demo/app_kernelmodules/kernelmodules/misc_mmap/misc-mmap-demo.mod.o):
