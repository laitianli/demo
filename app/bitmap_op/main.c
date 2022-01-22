#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

static inline uint32_t
rte_combine32ms1b(register uint32_t x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x;
}

static inline uint64_t
rte_combine64ms1b(register uint64_t v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return v;
}

static inline uint32_t
rte_align32pow2(uint32_t x)
{
    x--;
    x = rte_combine32ms1b(x);

    return x + 1;
}

static inline uint32_t
rte_log2_u32(uint32_t v)
{
    if (v == 0)
        return 0;
    v = rte_align32pow2(v);
    return __builtin_ctz(v);
}

static inline uint64_t
rte_align64pow2(uint64_t v)
{
    v--;
    v = rte_combine64ms1b(v);

    return v + 1;
}

static inline uint32_t
rte_log2_u64(uint64_t v)
{
    if (v == 0)
        return 0;
    v = rte_align64pow2(v);
    /* we checked for v being 0 already, so no undefined behavior */
    return __builtin_ctzll(v);
}


void test_builtin_api(void)
{
    unsigned int val = 7368; /* 1 1100 1100 1000 */
    printf("============================================================\n");
    /* 从底位到高位，返回二进制数第一个为1的位 */
    printf("__builtin_ffs(%u): %d\n", val, __builtin_ffs(val)); /* 4 */

    /* 返回前导0个数（高位0的个数） */
    printf("__builtin_clz(%u): %d\n", val, __builtin_clz(val)); /* 19 */

    /* 返回低位为1的个数*/
    printf("__builtin_ctz(%u): %d\n", val, __builtin_ctz(val)); /* 3 */

    /* 返回1的个数 */
    printf("__builtin_popcount(%u): %d\n", val, __builtin_popcount(val)); /* 6 */

    /* 返回奇偶校验位 */
    printf("__builtin_parity(%u): %d\n", val, __builtin_parity(val)); /* 0 */

    printf("__builtin_bswap32(%u): 0x%x\n", val, __builtin_bswap32(val)); /* 1100 1000 0001 1100 0000 0000 0000 0000  */
    unsigned long long ll_val = 7368; /* 1 1100 1100 1000 */

    /* 从底位到高位，返回二进制数第一个为1的位 */
    printf("__builtin_ffsll(%llu): %d\n", ll_val, __builtin_ffsll(ll_val)); /* 4 */

    /* 返回前导0个数（高位0的个数） */
    printf("__builtin_clzll(%llu): %d\n", ll_val, __builtin_clzll(ll_val)); /* 51 */

    /* 返回低位为1的个数*/
    printf("__builtin_ctzll(%llu): %d\n", ll_val, __builtin_ctzll(ll_val)); /* 3 */

    /* 返回1的个数 */
    printf("__builtin_popcountll(%llu): %d\n", ll_val, __builtin_popcountll(ll_val)); /* 6 */

    /* 返回奇偶校验位 */
    printf("__builtin_parityll(%llu): %d\n", ll_val, __builtin_parityll(ll_val)); /* 0 */
    printf("============================================================\n\n");
}
void test_combine()
{
    int val = 13;
    printf("rte_combine32ms1b(%d): %d\n", val, rte_combine32ms1b(val)); /* 15 */ 
    printf("rte_align32pow2(%d): %d\n", val, rte_align32pow2(val)); /* 16 */
    printf("rte_log2_u32(%d): %d\n", val, rte_log2_u32(val)); 

    int val2 = 50;
    printf("rte_combine64ms1b(%d): %d\n", val2, rte_combine64ms1b(val2)); /* 63 */
    printf("rte_align64pow2(%d): %d\n", val2, rte_align64pow2(val2)); /* 16 */
    printf("rte_log2_u64(%d): %d\n", val2, rte_log2_u64(val2));/* 6 */ 
}

int main(int argv, char** argc)
{
    test_builtin_api();
    test_combine();
    return 0;
}
