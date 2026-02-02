/* date.x - 远程日期时间服务协议 */
program DATE_PROG {
    version DATE_VERS {
        long BIN_DATE(void) = 1;   /* 过程1: 获取二进制格式时间 */
        string STR_DATE(long) = 2; /* 过程2: 将时间转换为字符串 */
    } = 1; /* 版本号 */
} = 0x31234567; /* 唯一的程序编号 */