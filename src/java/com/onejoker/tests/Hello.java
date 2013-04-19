/* OneJoker library <http://lcrocker.github.io/OneJoker>
 *
 * "Hello World" app included here to keep docs in sync with code.
 */
package com.onejoker.tests;
import com.onejoker.onejoker.*;

public class Hello {
    static {
        System.loadLibrary("onejoker");
    }
    public static void main(String[] args) {
        CardList deck = new CardList(52), hand = new CardList(5);
        deck.fill();
        deck.shuffle();

        for (int i = 0; i < 5; ++i) hand.add(deck.pop());
        System.out.println(hand);
    }
}
