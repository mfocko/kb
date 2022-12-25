---
title: 2nd week of Advent of Code '22 in Rust
description: Surviving second week in Rust.
date: 2022-12-25T23:15
slug: aoc-2022/2nd-week
authors:
- name: Matej Focko
  title: "a.k.a. @mf"
  url: https://gitlab.com/mfocko
  image_url: https://github.com/mfocko.png
tags:
- aoc-2022
- advent-of-code
- rust
hide_table_of_contents: false
---

Let's go through the second week of [_Advent of Code_] in Rust.

<!--truncate-->

## [Day 8: Treetop Tree House](https://adventofcode.com/2022/day/8)

:::info tl;dr

We get a forest and we want to know how many trees are visible from the outside.
Apart from that we want to find the best view.

:::

Nothing interesting. We are moving around 2D map though. And indexing can get a
bit painful when doing so, let's refactor it a bit ;) During the preparation for
the AoC, I have written `Vector2D` and now it's time to extend it with indexing
of `Vec` of `Vec`s. In my solution I was manipulating with indices in the following
way:

- swapping them
- checking whether they are correct indices for the `Vec<Vec<T>>`
- indexing `Vec<Vec<T>>` with them

:::caution

I'm getting familiar with Rust and starting to „abuse“ it… While doing so, I'm
also uncovering some „features“ that I don't really like. Therefore I will mark
all of my rants with _thicc_ **«↯»** mark and will try to „lock“ them into their
own „box of hell“.

:::

#### Swapping indices

Relatively simple implementation, just take the values, swap them and return new
vector.

```rust
impl<T: Copy> Vector2D<T> {
    pub fn swap(&self) -> Self {
      Self {
        x: self.y,
        y: self.x,
      }
    }
}
```

Pretty straight-forward implementation, but let's talk about the `T: Copy`. We
need to use it, since we are returning a **new** vector, with swapped **values**.
If we had values that cannot be copied, the only thing we could do, would be a
vector of references (and it would also introduce a lifetime, to which we'll get
later on). This is pretty similar with the operations on sets from the first week.

#### Indexing `Vec`

I will start with the indexing, cause bound-checking is a bit more… complicated
than I would like to.

```rust
pub fn index<'a, T, U>(v: &'a [Vec<U>], idx: &Vector2D<T>) -> &'a U
where
    usize: TryFrom<T>,
    <usize as TryFrom<T>>::Error: Debug,
    T: Copy,
{
    let (x, y): (usize, usize) = (idx.x.try_into().unwrap(), idx.y.try_into().unwrap());
    &v[y][x]
}
```

Let's talk about this mess… Body of the function is probably the most easy part
and should not be hard to understand, we just take the `x` and `y` and convert
them both to `usize` type that can be used later on for indexing.

The type signature of the function is where the fun is at :wink: We are trying
to convert unknown type to `usize`, so we must bound the `T` as a type that can
be converted to `usize`, that's how we got `usize: TryFrom<T>` which basically
says that `usize` must implement `TryFrom<T>` trait vand therefore allows us to
convert the indices to actual `usize` indices. Using `.unwrap()` also forces us
to bound the error that can occur when converting `T` into `usize`, that's how
we get `<usize as TryFrom<T>>::Error: Debug` which loosely means

> error during conversion of `T` into `usize` must implement `Debug`,
> i.e. can be printed in some way or other

`T: Copy` is required by `.try_into()` which takes `T` by-value.

And now we are left only with the first line of the definition.

:::note

Skilled Rustaceans might notice that this implemention is rather flaky and can
break in multiple places at once. I'll get back to it…

:::

Let's split it in multiple parts:
- `v: &'a [Vec<U>]` represents the 2D `Vec`, we are indexing, `Vec` implements
  `Slice` trait and _clippy_ recommends using `&[T]` to `&Vec<T>`, exact details
  are unknown to me
- `idx: &Vector2D<T>` represents the _indices_ which we use, we take them by
  reference to avoid an unnecessary copy
- `-> &'a U` means that we are returning a _reference_ to some value of type `U`.
  Now the question is what does the `'a` mean, we can also see it as a generic
  type declared along `T` and `U`. And the answer is _relatively_ simple, `'a`
  represents a _lifetime_. We take the `v` by a reference and return a reference,
  borrow checker validates all of the _borrows_ (or references), so we need to
  specify that our returned value has _the same lifetime_ as the vector we have
  taken by a reference, i.e. returned reference must live at least as long as the
  `v`. This way we can „be sure“ that the returned reference is valid.

##### Issues

First issue that our implementation has is the fact that we cannot get a mutable
reference out of that function. This could be easily resolved by introducing new
function, e.g. `index_mut`. Which I have actually done while writing this part:
```rust
pub fn index_mut<'a, T, U>(v: &'a mut [Vec<U>], idx: &Vector2D<T>) -> &'a mut U
where
    usize: TryFrom<T>,
    <usize as TryFrom<T>>::Error: Debug,
    T: Copy,
{
    let (x, y): (usize, usize) = (idx.x.try_into().unwrap(), idx.y.try_into().unwrap());
    &mut v[y][x]
}
```

:::caution **«↯»** Why can't we use one function?

When we consider a `Vec<T>`, we don't need to consider containers as `T`, Rust
implements indexing as traits `Index<T>` and `IndexMut<T>` that do the dirty work
behind syntactic sugar of `container[idx]`.

However, implementing of traits is not allowed for _external_ types, i.e. types
that you haven't defined yourself. This means that you can implement indexing
over containers that you have implemented yourself, but you cannot use your own
types for indexing „built-in“ types.

Another part of this rabbit hole is trait `SliceIndex<T>` that is of a relevance
because of
```rust
impl<T, I> Index<I> for [T]
where
    I: SliceIndex<[T]>

impl<T, I, A> Index<I> for Vec<T, A>
where
    I: SliceIndex<[T]>,
    A: Allocator

impl<T, I, const N: usize> Index<I> for [T; N]
where
    [T]: Index<I>
```

In other words, if your type implements `SliceIndex<T>` trait, it can be used
for indexing. As of now, this trait has all of its required methods experimental
and is marked as `unsafe`.

:::

Another problem is a requirement for indexing either `[Vec<T>]` or `Vec<Vec<T>>`.
This requirement could be countered by removing inner type `Vec<T>` and constraining
it by a trait `Index` (or `IndexMut` respectively) in a following way
```rust
pub fn index<'a, C, T>(v: &'a [C], idx: &Vector2D<T>) -> &'a C::Output
where
    usize: TryFrom<T>,
    <usize as TryFrom<T>>::Error: Debug,
    T: Copy,
    C: Index<usize>
{
    let (x, y): (usize, usize) = (idx.x.try_into().unwrap(), idx.y.try_into().unwrap());
    &v[y][x]
}
```

Given this, we can also give a more meaningful typename for indexing type, such
as `I`.

#### Checking bounds

Now we can get to the boundary checks, it is very similar, but a more… dirty.
First approach that came up was to convert the indices in `Vector2D` to `usize`,
but when you add the indices up, e.g. when checking the neighbors, you can end
up with negative values which, unlike in C++, causes an error (instead of underflow
that you can use to your advantage; you can easily guess how).

So how can we approach this then? Well… we will convert the bounds instead of
the indices and that lead us to:
```rust
pub fn in_range<T, U>(v: &[Vec<U>], idx: &Vector2D<T>) -> bool
where
    usize: TryInto<T>,
    <usize as TryInto<T>>::Error: Debug,
    T: PartialOrd + Copy,
{
    idx.y >= 0.try_into().unwrap()
        && idx.y < v.len().try_into().unwrap()
        && idx.x >= 0.try_into().unwrap()
        && idx.x
            < v[TryInto::<usize>::try_into(idx.y).unwrap()]
                .len()
                .try_into()
                .unwrap()
}
```

You can tell that it's definitely a shitty code. Let's improve it now! We will
get back to the original idea, but do it better. We know that we cannot convert
negative values into `usize`, **but** we also know that conversion like that
returns a `Result<T, E>` which we can use to our advantage.
```rust
pub fn in_range<T, U>(v: &[Vec<U>], idx: &Vector2D<T>) -> bool
where
    T: Copy,
    usize: TryFrom<T>,
{
    usize::try_from(idx.y)
        .and_then(|y| usize::try_from(idx.x).map(|x| y < v.len() && x < v[y].len()))
        .unwrap_or(false)
}
```

`Result<T, E>` is a type similar to `Either` in Haskell and it allows us to chain
multiple operations on correct results or propagate the original error without
doing anything. Let's dissect it one-by-one.

`try_from` is a method implemented in `TryFrom` trait, that allows you to convert
types and either successfully convert them or fail (with a reasonable error). This
method returns `Result<T, E>`.

We call `and_then` on that _result_, let's have a look at the type signature of
`and_then`, IMO it explains more than enough:
```rust
pub fn and_then<U, F>(self, op: F) -> Result<U, E>
where
    F: FnOnce(T) -> Result<U, E>
```

OK… So it takes the result and a function and returns another result with
different value and different error. However we can see that the function, which
represents an operation on a result, takes just the value, i.e. it doesn't care
about any previous error. To make it short:

> `and_then` allows us to run an operation, which can fail, on the correct result

We parsed a `y` index and now we try to convert the `x` index with `try_from`
again, but on that result we use `map` rather than `and_then`, why would that be?

```rust
pub fn map<U, F>(self, op: F) -> Result<U, E>
where
    F: FnOnce(T) -> U
```

Huh… `map` performs an operation that **cannot** fail. And finally we use
`unwrap_or` which takes the value from result, or in case of an error returns the
default that we define.

How does this work then? If `y` is negative, the conversion fails and the error
propagates all the way to `unwrap_or`, if `y` can be a correct `usize` value, then
we do the same with `x`. If `x` is negative, we propagate the error as with `y`,
and if it's not, then we check whether it exceeds the higher bounds or not.

### Solution

Relatively simple, you just need follow the rules and not get too smart, otherwise
it will get back at you.

## [Day 9: Rope Bridge](https://adventofcode.com/2022/day/9)

:::info tl;dr

We get a rope with knots and we want to track how many different positions are
visited with the rope's tail.

:::

By this day, I have come to a conclusion that current skeleton for each day
generates a lot of boilerplate. And even though it can be easily copied, it's
just a waste of space and unnecessary code. Let's „simplify“ this (on one end
while creating monster on the other end). I've gone through what we need in the
preparations for the AoC. Let's sum up our requirements:
- parsing
- part 1 & 2
- running on sample / input
- tests

Parsing and implementation of both parts is code that changes each day and we
cannot do anything about it. However running and testing can be simplified!

Let's introduce and export a new module `solution` that will take care of all of
this. We will start by introducing a trait for each day.
```rust
pub trait Solution<Input, Output: Display> {
    fn parse_input<P: AsRef<Path>>(pathname: P) -> Input;

    fn part_1(input: &Input) -> Output;
    fn part_2(input: &Input) -> Output;
}
```

This does a lot of work for us already, we have defined a trait and for each day
we will create a structure representing a specific day. That structure will also
implement the `Solution` trait.

Now we need to get rid of the boilerplate, we can't get rid of the `main` function,
but we can at least move out the functionality.
```rust
fn run(type_of_input: &str) -> Result<()>
where
    Self: Sized,
{
    tracing_subscriber::fmt()
        .with_env_filter(EnvFilter::from_default_env())
        .with_target(false)
        .with_file(true)
        .with_line_number(true)
        .without_time()
        .compact()
        .init();
    color_eyre::install()?;

    let input = Self::parse_input(format!("{}s/{}.txt", type_of_input, Self::day()));

    info!("Part 1: {}", Self::part_1(&input));
    info!("Part 2: {}", Self::part_2(&input));

    Ok(())
}

fn main() -> Result<()>
where
    Self: Sized,
{
    Self::run("input")
}
```

This is all part of the `Solution` trait, which can implement methods while being
dependant on what is provided by the implementing types. In this case, we just
need to bound the `Output` type to implement `Display` that is necessary for the
`info!` and format string there.

Now we can get to first of the nasty things we are going to do… And it is the
`day()` method that you can see being used when constructing path to the input
file. That method will generate a name of the file, e.g. `day01` and we know that
we can _somehow_ deduce it from the structure name, given we name it reasonably.

```rust
fn day() -> String {
    let mut day = String::from(type_name::<Self>().split("::").next().unwrap());
    day.make_ascii_lowercase();

    day.to_string()
}
```

:::caution `type_name`

This feature is still experimental and considered to be internal, it is not
advised to use it any production code.

:::

And now we can get to the nastiest stuff :weary: We will **generate** the tests!

We want to be able to generate tests for sample input in a following way:
```rust
test_sample!(day_01, Day01, 42, 69);
```

There's not much we can do, so we will write a macro to generate the tests for us.

```rust
#[macro_export]
macro_rules! test_sample {
    ($mod_name:ident, $day_struct:tt, $part_1:expr, $part_2:expr) => {
        #[cfg(test)]
        mod $mod_name {
            use super::*;

            #[test]
            fn test_part_1() {
                let sample =
                    $day_struct::parse_input(&format!("samples/{}.txt", $day_struct::day()));
                assert_eq!($day_struct::part_1(&sample), $part_1);
            }

            #[test]
            fn test_part_2() {
                let sample =
                    $day_struct::parse_input(&format!("samples/{}.txt", $day_struct::day()));
                assert_eq!($day_struct::part_2(&sample), $part_2);
            }
        }
    };
}
```

We have used it in a similar way as macros in C/C++, one of the things that we
can use to our advantage is defining „type“ of the parameters for the macro. All
parameters have their name prefixed with `$` sign and you can define various „forms“
of your macro. Let's go through it!

We have following parameters:
- `$mod_name` which represents the name for the module with tests, it is typed
  with `ident` which means that we want a valid identifier to be passed in.
- `$day_struct` represents the structure that will be used for tests, it is typed
  with `tt` which represents a _token tree_, in our case it is a type.
- `$part_X` represents the expected output for the `X`th part and is of type `expr`
  which literally means an _expression_.

Apart from that we need to use `#[macro_export]` to mark the macro as exported
for usage outside of the module. Now our skeleton looks like:
```rust
use aoc_2022::*;

type Input = String;
type Output = String;

struct DayXX;
impl Solution<Input, Output> for DayXX {
    fn parse_input<P: AsRef<Path>>(pathname: P) -> Input {
        file_to_string(pathname)
    }

    fn part_1(input: &Input) -> Output {
        todo!()
    }

    fn part_2(input: &Input) -> Output {
        todo!()
    }
}

fn main() -> Result<()> {
    // DayXX::run("sample")
    DayXX::main()
}

// test_sample!(day_XX, DayXX, , );
```

### Solution

Not much to talk about, it is relatively easy to simulate.

## [Day 10: Cathode-Ray Tube](https://adventofcode.com/2022/day/10)

:::info tl;dr

Emulating basic arithmetic operations on a CPU and drawing on CRT based on the
CPU's accumulator.

:::

In this day I have discovered an issue with my design of the `Solution` trait.
And the issue is caused by different types of `Output` for the part 1 and part 2.

Problem is relatively simple and consists of simulating a CPU, I have approached
it in a following way:
```rust
fn evaluate_instructions(instructions: &[Instruction], mut out: Output) -> Output {
    instructions
        .iter()
        .fold(State::new(), |state, instruction| {
            state.execute(instruction, &mut out)
        });

    out
}
```

We just take the instructions, we have some state of the CPU and we execute the
instructions one-by-one. Perfect usage of the `fold` (or `reduce` as you may know
it from other languages).

You can also see that we have an `Output` type, so the question is how can we fix
that problem. And the answer is very simple and _functional_. Rust allows you to
have an `enumeration` that can _bear_ some other values apart from the type itself.

:::tip

We could've seen something like this with the `Result<T, E>` type that can be
defined as
```rust
enum Result<T, E> {
  Ok(T),
  Err(E)
}
```

###### What does that mean though?

When we have an `Ok` value, it has the result itself, and when we get an `Err`
value, it has the error. This also allows us to handle _results_ in a rather
pretty way:
```rust
match do_something(x) {
  Ok(y) => {
    println!("SUCCESS: {}", y);
  },
  Err(y) => {
    eprintln!("ERROR: {}", y);
  }
}
```

:::

My solution has a following outline:
```rust
fn execute(&self, i: &Instruction, output: &mut Output) -> State {
    // execute the instruction

    // collect results if necessary
    match output {
      Output::Part1(x) => self.execute_part_1(y, x),
      Output::Part2(x) => self.execute_part_2(y, x),
    }

    // return the obtained state
    new_state
}
```

You might think that it's a perfectly reasonable thing to do. Yes, **but** notice
that the `match` statement doesn't _collect_ the changes in any way and also we
pass `output` by `&mut`, so it is shared across each _iteration_ of the `fold`.

The dirty and ingenious thing is that `x`s are passed by `&mut` too and therefore
they are directly modified by the helper functions. To sum it up and let it sit

> We are **collecting** the result **into** an **enumeration** that is **shared**
> across **all** iterations of `fold`.

### Solution

Similar to _Day 9_, but there are some technical details that can get you.

## [Day 11: Monkey in the Middle](https://adventofcode.com/2022/day/11)

:::info tl;dr

Simulation of monkeys throwing stuff around and measuring your stress levels
while your stuff is being passed around.

:::

I think I decided to use regular expressions here for the first time, cause
parsing the input was a pain.

Also I didn't expect to implement Euclidean algorithm in Rust…

### Solution

Again, we're just running a simulation. Though I must admit it was very easy to
make a small technical mistakes that could affect the final results very late.

## [Day 12: Hill Climbing Algorithm](https://adventofcode.com/2022/day/12)

:::info tl;dr

Finding shortest path up the hill and also shortest path down to the ground while
also rolling down the hill…

:::

As I have said in the _tl;dr_, we are looking for the shortest path, but the start
and goal differ for the part 1 and 2. So I have decided to refactor my solution
to a BFS algorithm that takes neccessary parameters via functions:
```rust
fn bfs<F, G>(
    graph: &[Vec<char>], start: &Position, has_edge: F, is_target: G
) -> Option<usize>
where
    F: Fn(&[Vec<char>], &Position, &Position) -> bool,
    G: Fn(&[Vec<char>], &Position) -> bool
```

We pass the initial vertex from the caller and everything else is left to the BFS
algorithm, based on the `has_edge` and `is_target` functions.

This was easy! And that is not very usual in Rust once you want to pass around
functions. :eyes:

### Solution

Looking for the shortest path… Must be Dijkstra, right? **Nope!** Half of the
Reddit got jebaited though. In all fairness, nothing stops you from implementing
the Dijkstra's algorithm for finding the shortest path, **but** if you know that
all connected vertices are in a unit (actually $d = 1$) distance from each other,
then you know that running Dijkstra is equivalent to running BFS, only with worse
time complexity, because of the priority heap instead of the queue.

## [Day 13: Distress Signal](https://adventofcode.com/2022/day/13)

:::info tl;dr

Processing packets with structured data from the distress signal.

:::

You can implement a lot of traits if you want to. It is _imperative_ to implement
ordering on the packets. I had a typo, so I also proceeded to implement a `Display`
trait for debugging purposes:
```rust
impl Display for Packet {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Packet::Integer(x) => write!(f, "{x}"),
            Packet::List(lst) => write!(f, "[{}]", lst.iter().map(|p| format!("{p}")).join(",")),
        }
    }
}
```

### Solution

A lot of technical details… Parsing is nasty too…

## [Day 14: Regolith Reservoir](https://adventofcode.com/2022/day/14)

:::info tl;dr

Let's simulate falling sand grain-by-grain.

:::

Again, both parts are relatively similar with minimal changes, so it is a good
idea to refactor it a bit. Similar approach to the [BFS above]. Also this is the
first day where I ran into efficiency issues and had to redo my solution to speed
it up just a bit.

### Solution

Tedious.

[_Advent of Code_]: https://adventofcode.com
[BFS above]: #day-12-hill-climbing-algorithm
