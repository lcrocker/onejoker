
=== Cardlist ===

<dl>
<dt>`_johnnymoss`</dt>
    <dd>Set to 0x10ACE0FF on initialization so that functions can check for initialized structures.
    </dd>
<dt>`int allocation`</dt>
    <dd>How many ints are available at the address pointed to by `cards`. Fixed during the lifetime of the structure.
    </dd>
<dt>`int length`</dt>
    <dd>Number of currently active cards. Any values in the `cards` array beyond this are ignored.
    </dd>
<dt>`int pflags`</dt>
    <dd>"Persistent" flags for general use.
    (pflags & OJF_UNIQUE) indicates that this list is intended to contain unique card values (duplicates are allowed by default). This option should be used only if necessary, as it slows down many list functions.
    </dd>
<dt>`int eflags`</dt>
    <dd>"Ephemeral" flags cleared when contents change.
    </dd>
<dt>`uint64_t mask`</dt>
    <dd>Contents bitmap for unique-card lists.
    </dd>
<dt>`void *user_info`</dt>
    <dd>Pointer to whatever extras the client needs.
    </dd>
<dt>`int *cards`</dt>
    <dd>Actual array of card values. Only the first `length` are meaningful.
    </dd>
</dl>

=== Combiner ===

<dl>
<dt>`_johnnymoss`</dt>
    <dd>Set to 0x10ACE0FF on initialization so that functions can check for initialized structures.
    </dd>
<dt>`k`</dt>
    <dd>Subset size; that is, the combiner object represents the set of combinations of `k` cards chosen from `deck`.
    </dd>
<dt>`map`</dt>
    <dd>Scratch array of 0-based indices into deck.
    </dd>
<dt>`deck_invert`</dt>
    <dd>Inverse array to `deck`.
    </dt>
<dt>`total`</dd>
    <dd>Number of combinations, equal to (`deck->length` choose `k`).
    </dd>
<dt>`rank`</dt>
    <dd>Colexicographical rank of current combination.
    </dd>
<dt>`remaining`</dt>
    <dd>Iterations remaining for random combinations.
    </dd>
<dt>`deck`</dt>
    <dd>Total set of cards from which to take combinations. Is never modified, so can be read-only.
    </dd>
<dt>`hand`</dd>
    <dd>Set to current combination. Must be writable, and have room for `k` cards.
    </dd>
<dt>`user_info`</dt>
    <dd>Kept with structure for whatever the client needs.
    </dd>
</dl>
