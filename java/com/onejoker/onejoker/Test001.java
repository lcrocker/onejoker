/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;

public class Test001 {
    public native long binomial(int n, int k);
    static {
        System.loadLibrary("onejoker");
    }
    public static void main(String[] args) {
        Context c = new Context();
        long r = c.binomial(52, 5);
        System.out.printf("%d\n", r);

        for (int i = 0; i < 10; i += 1) {
            System.out.printf("%d ", c.rand(100));
        }
        System.out.printf("\n");
    }
}
