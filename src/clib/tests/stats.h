/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */

struct buckets {
    double mean, stddev, maxz;
    long n, count;
    long buckets[1];
};

extern struct buckets *create_buckets(int n);
extern void free_buckets(struct buckets *bp);
extern int add_value(struct buckets *bp, int v);
extern int calculate_stats(struct buckets *bp);

