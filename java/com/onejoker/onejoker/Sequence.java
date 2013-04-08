/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;

public class Sequence {
    int[] cards;

    private native void _new(int[] c);

    public Sequence(int size) {
        cards = new int[size];
        _new(cards);
    }
}
