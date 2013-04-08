/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;

public class Iterator {
    Sequence hand;
    int[] invertbuf;

    private native void _new(Sequence d, Sequence h, int[] b);

    public Iterator(Sequence deck, int k, int total) {
        hand = new Sequence(k);
        invertbuf = new int[52];
        _new(deck, hand, invertbuf);
    }
}
