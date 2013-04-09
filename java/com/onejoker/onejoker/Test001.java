/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;

public class Test001 {
    static {
        System.loadLibrary("onejoker");
    }
    public static void main(String[] args) {
        System.out.printf("** 1\n");
        CardList s = new CardList(10);
        System.out.printf("** 2, %d\n", s.size());

        s.add(45);
        s.add(12);
        System.out.printf("** 3 %d %d %d\n", s.size(), s.get(0), s.get(1));

        s.set(1, 5);
        System.out.printf("** 4 %d %d %d\n", s.size(), s.get(0), s.get(1));
        s.sort();
        System.out.printf("** 5 %d %d %d\n", s.size(), s.get(0), s.get(1));
    }
}
