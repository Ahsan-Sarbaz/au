# Performance in Release build 

### 13th Gen Intel® Core™ i7-1355U × 12
#### Commit: `6cbe028`

```
=== Push/Pop Performance ===
Push head  1000000 elements:   25.192 ms (25.192 ns/op)
Push tail  1000000 elements:    9.748 ms ( 9.748 ns/op)
Pop head   1000000 elements:   10.225 ms (10.225 ns/op)
Pop tail   1000000 elements:   11.288 ms (11.288 ns/op)

=== Middle Operations (Quadratic Complexity) ===
Insert mid   10000 elements:   27.162 ms (2716.175 ns/op)
Pop mid      10000 elements:   29.708 ms (2970.839 ns/op)

=== Random Access Performance ===
Access mixed          (size 100000): 21695.817 ns/access
Access mixed          (size 1000000): 315239.447 ns/access
```