---
title: 1st week of Advent of Code '22 in Rust
description: Surviving first week in Rust.
date: 2022-12-15T01:15
slug: aoc-2022/1st-week
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

Let's go through the first week of [_Advent of Code_] in Rust.

<!--truncate-->

:::note

If you wish to have a look at the solutions, you can follow them on my [GitLab].
More specifically in the [`/src/bin/`].

:::

I will try to summarize my experience with using Rust for the AoC. Trying it out
ages ago, I believe it will be _pain and suffering_, but we will see. For each
day I will also try to give a tl;dr of the problem, so that you can better imagine
the relation to my woes or :+1: moments.

## [Day 1: Calorie Counting](https://adventofcode.com/2022/day/1)

:::info tl;dr

As the name suggests, we get the calories of the food contained in the elves
backpacks and we want to choose the elf that has the most food ;)

:::

> Wakey wakey!

Programming in Rust at 6am definitely hits. I've also forgotten to mention how I
handle samples. With each puzzle you usually get a sample input and expected
output. You can use them to verify that your solution works, or usually doesn't.

At first I've decided to put asserts into my `main`, something like
```rust
assert_eq!(part_1(&sample), 24000);
info!("Part 1: {}", part_1(&input));

assert_eq!(part_2(&sample), 45000);
info!("Part 2: {}", part_2(&input));
```

However, once you get further, the sample input may take some time to run itself.
So in the end, I have decided to turn them into unit tests:
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_part_1() {
        let sample = parse_input("samples/day01.txt");
        assert_eq!(part_1(&sample), 24000);
    }

    #[test]
    fn test_part_2() {
        let sample = parse_input("samples/day01.txt");
        assert_eq!(part_2(&sample), 45000);
    }
}
```

And later on I have noticed, it's hard to tell the difference between the days,
so I further renamed to `mod` to reflect the days.

Also first thing I've done after finishing the first day puzzle, I have installed
an [`sccache`] to cache the builds, so that the build time is lower, cause it was
kinda unbearable.

### Solution

Well, it's a pretty simple problem. You just take the input, sum the calories and
find the biggest one. However, if we try to generalize to more than the biggest
one, the fun appears. We have few options:

- keep all the calories, sort them, take what we need
- keep all the calories and use max heap
- use min heap and maintain at most N calories that we need

## [Day 2: Rock Paper Scissors](https://adventofcode.com/2022/day/2)

:::info tl;dr

You want to know what score did you achieve while playing _Rock Paper Scissors_.
And then you want to be strategic about it.

:::

Apart from the technical details of the puzzle, it went relatively smooth.

### Solution

I took relatively naïve approach and then tried to simplify it.

## [Day 3: Rucksack Reorganization](https://adventofcode.com/2022/day/3)

:::info tl;dr

Let's go reorganize elves' backpacks! Each backpacks has 2 compartments and you
want to find the common item among those compartments. Each of them has priority,
you care only about the sum.

:::

This is the day where I started to fight the compiler and neither of us decided
to give up. Let's dive into it \o/

:::tip Fun fact

Fighting the compiler took me 30 minutes.

:::

We need to find a common item among 2 collections, that's an easy task, right?
We can construct 2 sets and find an intersection:
```rust
let top: HashSet<i32> = [1, 2, 3].iter().collect();
let bottom: HashSet<i32> = [3, 4, 5].iter().collect();
```

Now, the first issue that we encounter is caused by the fact that we are using
a slice (the `[…]`), iterator of that returns **references** to the numbers.
And we get immediately yelled at by the compiler, because the numbers are discarded
after running the `.collect`. To fix this, we can use `.into_iter`:
```rust
let top: HashSet<i32> = [1, 2, 3].into_iter().collect();
let bottom: HashSet<i32> = [3, 4, 5].into_iter().collect();
```

This way the numbers will get copied instead of referenced. OK, let's find the
intersection of those 2 collections:
```rust
println!("Common elements: {:?}", top.intersection(&bottom));
```
```
Common elements: [3]
```

:::caution

Notice that we need to do `&bottom`. It explicitly specifies that `.intersection`
**borrows** the `bottom`, i.e. takes an immutable reference to it.

:::

That's what we want, right? Looks like it! \o/

Next part wants us to find the common element among all of the backpacks. OK, so
that should be fairly easy, we have an intersection and we want to find intersection
over all of them.

Let's have a look at the type of the `.intersection`
```rust
pub fn intersection<'a>(
    &'a self,
    other: &'a HashSet<T, S>
) -> Intersection<'a, T, S>
```

OK… Huh… But we have an example there!
```rust
let intersection: HashSet<_> = a.intersection(&b).collect();
```

Cool, that's all we need.
```rust
let top: HashSet<i32> = [1, 2, 3, 4].into_iter().collect();
let bottom: HashSet<i32> = [3, 4, 5, 6].into_iter().collect();
let top_2: HashSet<i32> = [2, 3, 4, 5, 6].into_iter().collect();
let bottom_2: HashSet<i32> = [4, 5, 6].into_iter().collect();

let intersection: HashSet<_> = top.intersection(&bottom).collect();
println!("Intersection: {:?}", intersection);
```
```
Intersection: {3, 4}
```

Cool, so let's do the intersection with the `top_2`:
```rust
let top: HashSet<i32> = [1, 2, 3, 4].into_iter().collect();
let bottom: HashSet<i32> = [3, 4, 5, 6].into_iter().collect();
let top_2: HashSet<i32> = [2, 3, 4, 5, 6].into_iter().collect();
let bottom_2: HashSet<i32> = [4, 5, 6].into_iter().collect();

let intersection: HashSet<_> = top.intersection(&bottom).collect();
let intersection: HashSet<_> = intersection.intersection(&top_2).collect();
println!("Intersection: {:?}", intersection);
```

And we get yelled at by the compiler:
```
error[E0308]: mismatched types
  --> src/main.rs:10:58
   |
10 | let intersection: HashSet<_> = intersection.intersection(&top_2).collect();
   |                                             ------------ ^^^^^^ expected `&i32`, found `i32`
   |                                             |
   |                                             arguments to this function are incorrect
   |
   = note: expected reference `&HashSet<&i32>`
              found reference `&HashSet<i32>`
```

/o\ What the hell is going on here? Well, the funny thing is, that this operation
doesn't return the elements themselves, but the references to them and when we pass
the third set, it has just the values themselves, without any references.

:::tip

It may seem as a very weird decision, but in fact it makes some sense… It allows
you to do intersection of items that may not be possible to copy. Overall this is
a „tax“ for having a borrow checker ~~drilling your ass~~ having your back and
making sure you're not doing something naughty that may cause an **undefined**
**behaviour**.

:::

To resolve this we need to get an iterator that **clones** the elements:
```rust
let top: HashSet<i32> = [1, 2, 3, 4].into_iter().collect();
let bottom: HashSet<i32> = [3, 4, 5, 6].into_iter().collect();
let top_2: HashSet<i32> = [2, 3, 4, 5, 6].into_iter().collect();
let bottom_2: HashSet<i32> = [4, 5, 6].into_iter().collect();

let intersection: HashSet<_> = top.intersection(&bottom).cloned().collect();
let intersection: HashSet<_> = intersection.intersection(&top_2).cloned().collect();
let intersection: HashSet<_> = intersection.intersection(&bottom_2).cloned().collect();
println!("Intersection: {:?}", intersection);
```
```
Intersection: {4}
```

### Solution

The approach is pretty simple, if you omit the _1on1 with the compiler_. You just
have some fun with the set operations :)

## [Day 4: Camp Cleanup](https://adventofcode.com/2022/day/4)

:::info tl;dr

Elves are cleaning up the camp and they got overlapping sections to clean up.
Find how many overlap and can take the day off.

:::

[`RangeInclusive`] is your friend not an enemy :)

### Solution

Relatively easy, you just need to parse the input and know what you want. Rust's
`RangeInclusive` type helped a lot, cause it took care of all abstractions.

## [Day 5: Supply Stacks](https://adventofcode.com/2022/day/5)

:::info tl;dr

Let's play with stacks of crates.

:::

Very easy problem with very annoying input. You can judge yourself:
```
    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
```

Good luck transforming that into something reasonable :)

### Solution

For the initial solution I went with a manual solution (as in _I have done all_
_the work_. Later on I have decided to explore the `std` and interface of the
`std::vec::Vec` and found [`split_off`] which takes an index and splits (duh)
the vector:
```rust
let mut vec = vec![1, 2, 3];
let vec2 = vec.split_off(1);
assert_eq!(vec, [1]);
assert_eq!(vec2, [2, 3]);
```

This helped me simplify my solution a lot and also get rid of some _edge cases_.

## [Day 6: Tuning Trouble](https://adventofcode.com/2022/day/6)

:::info tl;dr

Finding start of the message in a very weird protocol. Start of the message is
denoted by $N$ unique consecutive characters.

:::

### Solution

A lot of different approaches, knowing that we are dealing with input consisting
solely of ASCII letters, I bit the bullet and went with sliding window and
constructing sets from that window, checking if the set is as big as the window.

One possible optimization could consist of keeping a bitvector (i.e. `usize`
variable) of encountered characters and updating it as we go. However this has
a different issue and that is removal of the characters from the left side of the
window. We don't know if the same character is not included later on.

Other option is to do similar thing, but keeping the frequencies of the letters,
and again knowing we have only ASCII letters we can optimize by having a vector
of 26 elements that keeps count for each lowercase letter.

## [Day 7: No Space Left On Device](https://adventofcode.com/2022/day/7)

:::info tl;dr

Let's simulate [`du`] to get some stats about our filesystem and then pinpoint
directories that take a lot of space and should be deleted.

:::

> I was waiting for this moment, and yet it got me!
> *imagine me swearing for hours*

### Solution

We need to „_build_“ a filesystem from the input that is given in a following form:
```
$ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
```

There are few ways in which you can achieve this and also you can assume some
preconditions, but why would we do that, right? :)

You can „slap“ this in either [`HashMap`] or [`BTreeMap`] and call it a day.
And that would be boring…

:::tip

`BTreeMap` is quite fitting for this, don't you think?

:::

I always wanted to try allocation on heap in Rust, so I chose to implement a tree.
I fought with the `Box<T>` for some time and was losing…

Then I looked up some implementations of trees or linked lists and decided to try
`Rc<Cell<T>>`. And I got my _ass whopped_ by the compiler once again. /o\

:::tip

`Box<T>` represents a dynamically allocated memory on heap. It is a single pointer,
you can imagine this as `std::unique_ptr<T>` in C++.

`Rc<T>` represents a dynamically allocated mmemory on heap. On top of that it is
_reference counted_ (that's what the `Rc` stands for). You can imagine this as
`std::shared_ptr<T>` in C++.

Now the fun stuff. Neither of them lets you **mutate** the contents of the memory.

`Cell<T>` allow you to mutate the memory. Can be used reasonably with types that
can be copied, because the memory safety is guaranteed by copying the contents
when there is more than one **mutable** reference to the memory.

`RefCell<T>` is similar to the `Cell<T>`, but the borrowing rules (how many mutable
references are present) are checked dynamically.

So in the end, if you want something like `std::shared_ptr<T>` in Rust, you want
to have `Rc<RefCell<T>>`.

:::

So, how are we going to represent the filesystem then? We will use an enumeration,
hehe, which is an algebraic data type that can store some stuff in itself :weary:
```rust
type FileHandle = Rc<RefCell<AocFile>>;

#[derive(Debug)]
enum AocFile {
    File(usize),
    Directory(BTreeMap<String, FileHandle>),
}
```

Let's go over it! `FileHandle` represents dynamically allocated `AocFile`, not
much to discuss. What does the `#[derive(Debug)]` do though? It lets us to print
out the value of that enumeration, it's derived, so it's not as good as if we had
implemented it ourselves, but it's good enough for debugging, hence the name.

Now to the fun part! `AocFile` value can be represented in two ways:
- `File(usize)`, e.g. `AocFile::File(123)` and we can pattern match it, if we
  need to
- `Directory(BTreeMap<String, FileHandle>)` will represent the directory and will
  contain map matching the name of the files (or directories) within to their
  respective file handles

I will omit the details about constructing this filesystem, cause there are a lot
of technicalities introduced by the nature of the input. However if you are
interested, you can have a look at my solution.

We need to find small enough directories and also find the smallest directory that
will free enough space. Now the question is, how could we do that. And there are
multiple ways I will describe.

I have chosen to implement [_tree catamorphism_] :weary:. It is basically a fold
over a tree data structure. We descent down into the leaves and propagate computed
results all the way to the root. You can also notice that this approach is very
similar to _dynamic programming_, we find overlapping sections of the computation
and try to minimize the additional work (in this case: we need to know sizes of
our descendants, but we have already been there).

Another approach that has been suggested to me few days later is running DFS on
the graph. And, funnily enough, we would still need to combine what we found in
the branches where we descent. So in the end, it would work very similarly to my
solution.

One of the more exotic options would be precomputing the required information at
the same time as parsing. That could be done by adding additional fields to the
nodes which would allow storing such information and updating it as we construct
the filesystem.

[_Advent of Code_]: https://adventofcode.com
[GitLab]: https://gitlab.com/mfocko/advent-of-code-2022
[`/src/bin/`]: https://gitlab.com/mfocko/advent-of-code-2022/-/tree/main/src/bin
[`sccache`]: https://github.com/mozilla/sccache
[`RangeInclusive`]: https://doc.rust-lang.org/std/ops/struct.RangeInclusive.html
[`split_off`]: https://doc.rust-lang.org/std/vec/struct.Vec.html#method.split_off
[`du`]: https://www.man7.org/linux/man-pages/man1/du.1.html
[`HashMap`]: https://doc.rust-lang.org/std/collections/struct.HashMap.html
[`BTreeMap`]: https://doc.rust-lang.org/std/collections/struct.BTreeMap.html
[_tree catamorphism_]: https://en.wikipedia.org/wiki/Catamorphism#Tree_fold
