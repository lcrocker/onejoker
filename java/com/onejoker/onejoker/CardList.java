/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */
package com.onejoker.onejoker;
import java.nio.*;

public class CardList {
    private ByteBuffer mBuf;
    private static int mStructSize, mLengthOff;

    private static native int nStructSize();
    private static native int nLengthOff();
    static {
        mStructSize = nStructSize();
        mLengthOff = nLengthOff();
    }
    private static native boolean nAppend(ByteBuffer b, int c);
    private static native int nDelete(ByteBuffer b, int i);
    private static native boolean nEquals(ByteBuffer b, ByteBuffer c);
    private static native boolean nExtend(ByteBuffer d, ByteBuffer s, int c);
    private static native int nFill(ByteBuffer b, int c, int t);
    private static native int nFNVHash(ByteBuffer b);
    private static native int nIndex(ByteBuffer b, int c);
    private static native void nNew(ByteBuffer b, int s);
    private static native int nPop(ByteBuffer b);
    private static native boolean nRemove(ByteBuffer b, int c);
    private static native void nReverse(ByteBuffer b);
    private static native void nShuffle(ByteBuffer b);
    private static native void nSort(ByteBuffer b);

    public boolean add(int c) { return nAppend(mBuf, c); }
    public boolean addAll(CardList s) { return nExtend(mBuf, s.mBuf, 0); }
    public void clear() { mBuf.putInt(mLengthOff, 0); }
    public boolean contains(int c) { return -1 != nIndex(mBuf, c); }
    public boolean equals(CardList s) { return nEquals(mBuf, s.mBuf); }
    public int get(int i) { return mBuf.getInt(mStructSize + 4 * i); }
    public int fill(int c, int t) { return nFill(mBuf, c, t); }
    public int hashCode() { return nFNVHash(mBuf); }
    public int indexOf(int c) { return nIndex(mBuf, c); }
    public boolean isEmpty() { return 0 == size(); }
    public int pop() { return nPop(mBuf); }
    public int remove(int i) { return nDelete(mBuf, i); }
    public boolean removeCard(int c) { return nRemove(mBuf, c); }
    public void reverse() { nReverse(mBuf); }
    public void set(int i, int c) { mBuf.putInt(mStructSize + 4 * i, c); }
    public void shuffle() { nShuffle(mBuf); }
    public int size() { return mBuf.getInt(mLengthOff); }
    public void sort() { nSort(mBuf); }
    public int[] toArray() {
        int[] a = new int[size()];
        for (int i = 0; i < size(); ++i) { a[i] = get(i); }
        return a;
    }
    public void truncate(int s) { mBuf.putInt(mLengthOff, s); }

    public CardList(int size) {
        mBuf = ByteBuffer.allocateDirect(mStructSize + 4 * size);
        mBuf.order(ByteOrder.LITTLE_ENDIAN);
        nNew(mBuf, size);
    }
}
