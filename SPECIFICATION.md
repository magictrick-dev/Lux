# Lux Language Specification

### Strict Typing

Types in Lux are strictly typed with sizes. It uses a common shorthand format for
each of the types. 

- Integer Types

    `i8, i16, i32, i64, u8, u16, u32, u64`

    Integers are fixed sized and signed based on the `u` or `i` prefixes.

- Boolean Types

    `b8, b16, b32, b64`

    Booleans are signed integers and also have fixed sized associations.

- String Types

    `s8`

    The most common string format is `s8`, which is the same as C's `char` type.
    Additional sizes will be added when support for wider character systems is
    added. (Don't expect this.)

- Floating Point

    `r32, r64`

    Floating point is either 32-bit or 64-bit depending on precision. They correspond
    to C's `float` and `double` formats.

You can define types explicitly or implicitly (during definition) in variable
declarations.

```
foo: i32;
bar: u64;
baz: i32 := 10; // Declared & defined.
bax := baz; // Implicitly declared & defined.
```

### Arrays

Arrays in Lux are defined similar to types and are not zero-initialized.

```
my_array: i32[10];
my_array[0] = 1;
my_array[1] = 2;
my_array[2] = 3;
// ... so and so forth.

zero_array: i32[4] = {};
set_array: i32[3] = {1, 2, 3};
partial_set_array: i32[3] = { 1, 2 }; // third slot is zero initialized.
```

### Pointers

Pointers in Lux behave exactly like C's pointers.

```
foo: i32 := 100;
bar: i32 := 200;
baz: i32* := &foo;
bax: i32* := &bar;
*baz := 10;
*bax := 20;
test: i32 := 30;
baz := &test; // Pointer reassignment.
```

### While Loops, For Loops

Looping structures are effectively their C-counterparts.

```
sum: i32 := 0;
for (i: i32 = 0; i < 1000; ++i)
{
    sum += i;
}

while (sum >= 0)
{
    sum -= 1;
}
```

### If/Else Statements

If statements really aren't a mystery either.

```
if (a < b)
{
    // Do something.
}
else if (c > d)
{
    // Do something.
}
else
{
    // Do something.
}
```

### Functions

Function notation demarks return type, parameter list, and body. Functions can
have multiple return types, but the specification requires that all return types
are caught if they are defined. A basic example of a standard function call:

```
add_numbers:i32 (a: i32, b: i32)
{
    return(a + b);
}

result := add_number(4, 3);
```

Multi return values as an example. The types of these values must match the
return types of the function. Implicit typing works the same here.

```
high_low: u32, u32 (number: u64)
{
    low := (number & 0xFFFFFFFF);
    high := ((number & 0xFFFFFFFF00000000) >> 31);
    return (high, low);
}

high: u32, low: u32 := high_low(0xDEADBEEF19195959);
```

### Importing Files

File imports handle the magic of path resolution for you. Absolute path is relative
pathing, meaning that the absolute path is based on the location of the main file.

```
import math/linear.lux
import math/quarternions.lux
import engine/renderer.lux

main()
{
    // Access to everything above is based on main.
    return(0);
}
```

For example, if `main.lux` is in `source/`, then all paths are absolutely relative
to main, regardless of folder depth. This makes it easier to include files since you
don't need to math navigation. To enforce true relative pathing, place the path in
quotes and use relative notation and the path will resolve relatively.
