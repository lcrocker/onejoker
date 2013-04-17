Cardlist
--------

<dl>
<dt><code>int _johnnymoss</code></dt>
    <dd>Set to 0x10ACE0FF on initialization so that functions can check for initialized structures.
    </dd>
<dt><code>int allocation</code></dt>
    <dd>How many ints are available at the address pointed to by `cards`. Fixed during the lifetime of the structure.
    </dd>
<dt><code>int length</code></dt>
    <dd>Number of currently active cards. Any values in the `cards` array beyond this are ignored.
    </dd>
<dt><code>int pflags</code></dt>
    <dd>"Persistent" flags for general use.
    (pflags & OJF_UNIQUE) indicates that this list is intended to contain unique card values (duplicates are allowed by default). This option should be used only if necessary, as it slows down many list functions.
    </dd>
<dt><code>int eflags</code></dt>
    <dd>"Ephemeral" flags cleared when contents change.
    </dd>
<dt><code>uint64_t mask</code></dt>
    <dd>Contents bitmap for unique-card lists.
    </dd>
<dt><code>void *user_info</code></dt>
    <dd>Pointer to whatever extras the client needs.
    </dd>
<dt><code>int *cards</code></dt>
    <dd>Actual array of card values. Only the first `length` are meaningful.
    </dd>
</dl>

Combiner
--------

<dl>
<dt><code>int _johnnymoss</code></dt>
    <dd>Set to 0x10ACE0FF on initialization so that functions can check for initialized structures.
    </dd>
<dt><code>int k</code></dt>
    <dd>Subset size; that is, the combiner object represents the set of combinations of `k` cards chosen from `deck`.
    </dd>
<dt><code>int map[]</code></dt>
    <dd>Scratch array of 0-based indices into deck.
    </dd>
<dt><code>int deck_invert[]</code></dt>
    <dd>Inverse array to `deck`.
    </dt>
<dt><code>long long total</code></dd>
    <dd>Number of combinations, equal to (`deck->length` choose `k`).
    </dd>
<dt><code>long long rank</code></dt>
    <dd>Colexicographical rank of current combination.
    </dd>
<dt><code>long long remaining</code></dt>
    <dd>Iterations remaining for random combinations.
    </dd>
<dt><code>oj_cardlist_t *deck</code></dt>
    <dd>Total set of cards from which to take combinations. Is never modified, so can be read-only.
    </dd>
<dt><code>oj_cardlist_t *hand</code></dd>
    <dd>Set to current combination. Must be writable, and have room for `k` cards.
    </dd>
<dt><code>void * user_info</code></dt>
    <dd>Kept with structure for whatever the client needs.
    </dd>
</dl>
