#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_NAME "/dev/sm-scsi0"
typedef  char __s8;
typedef unsigned char __u8;

typedef  short __s16;
typedef unsigned short __u16;

typedef  int __s32;
typedef unsigned int __u32;

typedef  long __s64;
typedef unsigned long __u64;

typedef __u16  __le16;
typedef __u16  __be16;
typedef __u32  __le32;
typedef __u32  __be32;
typedef __u64  __le64;
typedef __u64  __be64;

struct mdp_superblock_1 {
    /* constant array information - 128 bytes */
    __le32 magic;/* MD_SB_MAGIC: 0xa92b4efc - little endian */
    __le32 major_version;/* 1 */
    __le32 feature_map;/* bit 0 set if 'bitmap_offset' is meaningful */
    __le32 pad0;/* always set to 0 when writing */

    __u8 set_uuid[16];/* user-space generated. */
    char set_name[32];/* set and interpreted by user-space */

    __le64 ctime;/* lo 40 bits are seconds, top 24 are microseconds or 0*/
    __le32 level;/* -4 (multipath), -1 (linear), 0,1,4,5 */
    __le32 layout;/* only for raid5 and raid10 currently */
    __le64 size;/* used size of component devices, in 512byte sectors */

    __le32 chunksize;/* in 512byte sectors */
    __le32 raid_disks;
    __le32 bitmap_offset;/* sectors after start of superblock that bitmap starts
                                         * NOTE: signed, so bitmap can be before
                                         * superblock
                                         *               * only meaningful of
                                         *               feature_map[0] is set.
                                         *                               */

    /* These are only valid with feature bit '4' */
    __le32 new_level;/* new level we are reshaping to*/
    __le64 reshape_position;/* next address in array-space for reshape */
    __le32 delta_disks;/* change in number of raid_disks*/
    __le32 new_layout;/* new layout*/
    __le32 new_chunk;/* new chunk size (512byte sectors)*/
    __u8 pad1[128-124];/* set to 0 when written */

    /* constant this-device information - 64 bytes */
    __le64 data_offset;/* sector start of data, often 0 */
    __le64 data_size;/* sectors in this device that can be used for data */
    __le64 super_offset;/* sector start of this superblock */
    __le64 recovery_offset;/* sectors before this offset (from data_offset) have been recovered */
    __le32 dev_number;/* permanent identifier of this  device - not role in raid */
    __le32 cnt_corrected_read; /* number of read errors that were corrected by re-writing */
    __u8 device_uuid[16]; /* user-space setable, ignored by kernel */
    __u8 devflags;/* per-device flags.  Only one defined...*/
#define WriteMostly 11/* mask for writemostly flag in above */
    __u8 pad2[64-57];/* set to 0 when writing */

    /* array state information - 64 bytes */
    __le64 utime;/* 40 bits second, 24 btes microseconds */
    __le64 events;/* incremented when superblock updated */
    __le64 resync_offset;/* data before this offset (from data_offset) known to be in sync */
    __le32 sb_csum;/* checksum upto devs[max_dev] */
    __le32 max_dev;/* size of devs[] array to consider */
    __u8 pad3[64-32];/* set to 0 when writing */

    /* device state information. Indexed by dev_number.
     *   * 2 bytes per device
     *       * Note there are no per-device state flags. State information is
     *       rolled
     *           * into the 'roles' value.  If a device is spare or faulty, then
     *           it doesn't
     *               * have a meaningful role.
     *                   */
        __le16 dev_roles[0];/* role in array, or 0xffff for a spare, or 0xfffe for faulty */
};


int main(int argc,char** argv)
{
    int fd = open(DEVICE_NAME,O_RDONLY);
    if(fd <= 0)
    {
        printf("[Error] open device [%s] failed.\n",DEVICE_NAME);
        return 0;
    }

    unsigned long begin_addr = 8 * 512;
    unsigned long data_size = 4096;
    off_t cur_addr = lseek(fd,begin_addr,SEEK_SET);
    printf("curr_addr=0x%llx\n",cur_addr);

    char buf[4096] = {0};
    size_t readlen = read(fd,buf,sizeof(buf));
	struct mdp_superblock_1 *sb = (struct mdp_superblock_1*)buf;
	
    printf("readlen=%d\n",readlen);
    printf("magic:0x%x,level:%d\n",(sb->magic),sb->level);

    return 0;
}
