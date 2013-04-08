/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;

public class Context {
    public native int seed(int s);
    public native int rand(int limit);
    public native long binomial(int n, int k);
}
