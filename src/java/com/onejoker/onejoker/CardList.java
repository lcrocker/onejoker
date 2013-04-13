/* OneJoker library <https://github.com/lcrocker/OneJoker>
 *
 * CardList is designed to look more or less like a Java List class, but it is
 * really just a wrapper for the C structure, so you can't do everything you
 * can do with a list. But it enables you to call the fast functions of the C
 * library like the shuffler, hand evaluator, and combinatorics.
 */
package com.onejoker.onejoker;
import java.nio.*;
import java.util.ArrayList;
import java.util.Iterator;

public class CardList {
    private ByteBuffer mBuf;
    private static int mStructSize, mLengthOff;

    private static native int nStructSize();
    private static native int nLengthOff();
    static {
        mStructSize = nStructSize();
        mLengthOff = nLengthOff();
    }
    public CardList(int size) {
        mBuf = ByteBuffer.allocateDirect(mStructSize + 4 * size);
        mBuf.order(ByteOrder.LITTLE_ENDIAN);
        nNew(mBuf, size);
    }
    public CardList(int size, ArrayList<Card> cards) {
        this(size);
        this.add(cards);
    }
    public CardList(ArrayList<Card> cards) {
        this(cards.size(), cards);
    }
    public CardList(int size, String s) {
        this(size);
        this.add(s);
    }
    public CardList(String s) {
        ArrayList<Card> cards = Card.listFromString(s);
        int size = cards.size();
        mBuf = ByteBuffer.allocateDirect(mStructSize + 4 * size);
        mBuf.order(ByteOrder.LITTLE_ENDIAN);
        nNew(mBuf, size);
        this.add(cards);
    }
    public boolean add(int c) { return nAppend(mBuf, c); }
    public boolean add(int i, int c) { return nInsert(mBuf, i, c); }
    public boolean add(Card c) { return nAppend(mBuf, c.intValue()); }
    public boolean add(ArrayList<Card> cards) {
        boolean r = false;
        for (Card c : cards) { r = nAppend(mBuf, c.intValue()); }
        return r;
    }
    public boolean add(String s) {
        ArrayList<Card> cards = Card.listFromString(s);
        return this.add(cards);
    }
    public boolean addAll(CardList s) { return nExtend(mBuf, s.mBuf, 0); }
    public void clear() { mBuf.putInt(mLengthOff, 0); }

    public boolean contains(int c) { return -1 != nIndex(mBuf, c); }
    public boolean contains(Card c) { return this.contains(c.intValue()); }
    public boolean contains(String s) {
        return this.contains(Card.parseInt(s));
    }
    public boolean equals(CardList s) { return nEquals(mBuf, s.mBuf); }
    public boolean equals(String s) { return this.equals(new CardList(s)); }
    public int getInt(int i) { return mBuf.getInt(mStructSize + 4 * i); }
    public Card get(int i) { return new Card(this.getInt(i)); }
    public int fill(int c, int t) { return nFill(mBuf, c, t); }
    public int fill(DeckType t, int c) { return this.fill(c, t.value()); }
    public int fill(DeckType t) { return this.fill(t, t.size()); }
    public int fill(int c) { return this.fill(DeckType.STANDARD, c); }
    public int hashCode() { return nFNVHash(mBuf); }
    public int indexOf(int c) { return nIndex(mBuf, c); }
    public int indexOf(Card c) { return this.indexOf(c.intValue()); }
    public boolean isEmpty() { return 0 == size(); }
    public Iterator<Card> iterator() { return new CardListIterator(this); }
    public int popInt() { return nPop(mBuf); }
    public Card pop() { return new Card(this.popInt()); }
    public int removeIndex(int i) { return nDelete(mBuf, i); }
    public boolean removeCard(int c) { return nRemove(mBuf, c); }
    public boolean removeCard(Card c) { return this.removeCard(c.intValue()); }
    public void reverse() { nReverse(mBuf); }
    public void set(int i, int c) { mBuf.putInt(mStructSize + 4 * i, c); }
    public void set(int i, Card c) { this.set(i, c.intValue()); }
    public void shuffle() { nShuffle(mBuf); }
    public int size() { return mBuf.getInt(mLengthOff); }
    public void sort() { nSort(mBuf); }

    public ArrayList<Card> toList() {
        ArrayList<Card> cards = new ArrayList<Card>();
        for (int i = 0; i < this.size(); ++i) { cards.add(this.get(i)); }
        return cards;
    }
    public String toString() {
        String s = "(";
        for (int i = 0; i < this.size(); ++i) {
            if (0 != i) s += " ";
            s += Card.name(this.get(i));
        }
        return s + ")";
    }
    public void truncate(int s) { mBuf.putInt(mLengthOff, s); }

    private static native boolean nAppend(ByteBuffer b, int c);
    private static native int nDelete(ByteBuffer b, int i);
    private static native boolean nEquals(ByteBuffer b, ByteBuffer c);
    private static native boolean nExtend(ByteBuffer d, ByteBuffer s, int c);
    private static native int nFill(ByteBuffer b, int c, int t);
    private static native int nFNVHash(ByteBuffer b);
    private static native int nIndex(ByteBuffer b, int c);
    private static native boolean nInsert(ByteBuffer b, int i, int c);
    private static native void nNew(ByteBuffer b, int s);
    private static native int nPop(ByteBuffer b);
    private static native boolean nRemove(ByteBuffer b, int c);
    private static native void nReverse(ByteBuffer b);
    private static native void nShuffle(ByteBuffer b);
    private static native void nSort(ByteBuffer b);
}

class CardListIterator implements Iterator<Card> {
    private int mIndex;
    private CardList mCards;

    CardListIterator(CardList cards) {
        mIndex = 0;
        mCards = cards;
    }
    public boolean hasNext() { return mIndex != mCards.size(); }
    public Card next() {
        Card c = mCards.get(mIndex);
        ++mIndex;
        return c;
    }
    public void remove() {
        if (0 == mIndex || 0 == mCards.size()) return;
        --mIndex;
        mCards.removeIndex(mIndex);
    }
}
