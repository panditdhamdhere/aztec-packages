## Lookups

**Current state**

- Lookups are supported, permutations are not. Lookups are "easy" to use when looking up into a precomputed table, and there is an example. There is no example for looking up into a dynamic (runtime) table, and it's a bit more difficult.
- Lookups work but you need to manually create a LookupInto class and add it to the tracehelper. This is a bit annoying because you need to specify the column names both in the pil file, and in C++ and they will not be automatically in sync.
- Counts are computed for you, but you need to specify a way (`find_dst_row`) to find a row in the destination table. This should be easy and fast for precomputed tables, but you will need to either do a search or create an index for runtime tables (not done yet).
- Calculation of inverses is actually very inefficient for lookups into big tables, in particular for precomputed tables. This is not new in this design: the inverses are calculated for every row with either the source or destination selector active. See possible improvements (INVERSES_SELECTOR).
- Calculation of inverses probes the whole circuit (not new in this design): the logderiv library probes every row and computes the inverse when needed. See possible improvements (INVERSES_PROBING)

**Future work**

Mostly notes for myself.

- Add example of efficient `find_dst_row` for lookup into dynamic.
- Add example of lookup into dynamic.
- Automatically derive a Lookup class from a codegenerated Relation.
- (INVERSES_SELECTOR) We should be able to compute inverses only for rows that either (1) have the source selector active or, (2) have a non-zero count. The easies way would be to add a new selector column sel_count_nonzero and use that as selector insted of the dst selector. We would also add a new constrain `sel_count_nonzero == 1 <=> count != 0`. We could possibly just bake this into the lookup relation (see predicates) and avoid the extra column.
- (INVERSES_PROBING) The current implementation probes the whole circuit but the trace object lets you efficiently visit non-zero rows. Since we now set dummy values to the inverse column, we can just visit the non-zero elements and compute the inverses for them.
