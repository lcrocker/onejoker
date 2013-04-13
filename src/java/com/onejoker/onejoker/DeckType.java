/* OneJoker library <https://github.com/lcrocker/OneJoker>
 *
 * Enumeration of deck types. Uses the fill function from CardList, which
 * calls into the C library, to get the 
 */
package com.onejoker.onejoker;

public enum DeckType {
    STANDARD(0, 52), ONEJOKER(1, 53), TWOJOKERS(2, 54),
    STRIPPED32(3, 32), STRIPPED40(4, 40), STRIPPED40J(5, 41);
    private final static int NTYPES = 6;

    private final int mValue;
    private final int mSize;
    private DeckType(int value, int size) {
        this.mValue = value;
        this.mSize = size;
    }
    public int value() { return this.mValue; }
    public int size() { return this.mSize; }

    private static final CardList[] mDecks = new CardList[NTYPES];
    static {
        for (int i = 0; i < NTYPES; ++i) {
            int c = DeckType.nCards(i);
            mDecks[i] = new CardList(c);
            mDecks[i].fill(c, i);
        }
    }
    public static CardList getDeck(int type) { return mDecks[type]; }
    public static CardList getDeck(DeckType t) { return mDecks[t.mValue]; }

    private static native int nCards(int type);
}
