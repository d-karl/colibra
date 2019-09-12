# colibra

**co**nstexpr **li**near alge**bra**.

This is a pet project, linear algebra library that is currently very much work
in progress. I intend to license this under the MIT license very soon.

## Goals
- [ ] Master constexpr and template programming
- [ ] Be able to answer coordinate transformation questions - where do I end up
  in Frame A if I add a vector in Frame B to my current position - at compile
  time and using a comfortable syntax.
- [ ] Explore C++20 parallelism TS.

## Task overview
- [ ] Basic Vector class of arbitrary arithmetic types - WIP.
- [ ] Matrix class of arbitrary arithmetic types.
- [ ] Matrix - Vector arithetic operations.
- [ ] Introduce the Concept of "Reference Frames".
- [ ] Allow transformations between Frames.
- [ ] Allow vector operations on vectors in different frames.
- [ ] "Frame Tree" lookup and conversion along a number of frames when
  requested.
- [ ] All constexpr (?).
