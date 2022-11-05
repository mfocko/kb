---
title: Vague postconditions and proving correctness of algorithms
description: |
  Practical example that shows how precise postconditions can help with both
  debugging and testing of the code.
---

## Introduction

[Source code](pathname:///files/ib002/postcondition-ambiguity/test_sort.py) used later on.

## Implementation of select sort from the exercises

To implement select sort from the exercises and make it as easy to read as possible, I have implemented maximum function that returns index of the biggest element from the first $n$ elements.

For the sake of time and memory complexity, I am also using `itertools.islice`, which makes a slice, but does not copy the elements into the memory like normal slice does.

There is also a `check_loop_invariant` function that will be described later.

```py showLineNumbers
def compare_by_value(pair):
    index, value = pair
    return value


def maximum(arr, n):
    first_n_elements = itertools.islice(enumerate(arr), n)
    index, value = max(first_n_elements, key=compare_by_value)
    return index


def select_sort(arr, n):
    assert n == len(arr)

    check_loop_invariant(arr, n, n)
    for i in reversed(range(1, n)):
        j = maximum(arr, i + 1)
        arr[i], arr[j] = arr[j], arr[i]

        check_loop_invariant(arr, n, i)

    return arr
```

## Discussed preconditions, loop invariants and postconditions

You can safely replace `A` with `arr` or array for list.

### Precondition

As a precondition we have established that `A` represents an array of values and $n$ is length of the `A`.

### Loop invariant

As for loop invariant we have established that we require two properties:

1. `A[i + 1 : n]` is sorted
2. all elements of `A[i + 1 : n]` are bigger or equal to the other elements

This invariant is later defined as `check_loop_invariant` function. It is checked before the first iteration and after each iteration.

### Postcondition

For the postcondition the first suggestion was that `A` must be sorted. And later we have added that `A'` must be a permutation of `A`.

> However at the end of the session question arose if it is really required to state in the postcondition that `A'` is a permutation.

## So is the permutation really required?

As I have said it is better to have postconditions explicit and do not expect anything that is not stated explicitly, e.g. _name suggests it_. In reality we could consider it as a smaller mistake (but it has consequences).

On the other hand explicit postconditions can be used to our advantage **and also** help our proof of correctness.

Consequences:

1. Property-based testing

   If we have explicit postconditions we can use them to define properties of the output from our algorithms. That way we can use property-based testing, which does not depend on specific inputs and expected outputs, but rather on randomly generated input and checking if the output conforms to our expectations (the postconditions are fulfilled).

2. Proof of correctness

   If we can prove that algorithm is correct even for algorithm that **is not** correct, we have a problem. That proof has no value and is useless.

For the sake of showcasing the power of postconditions I will introduce "select sort" that is not correct, but will comply with both the loop invariant and our vague postcondition and thus pass the tests.

## Implementation of the broken select sort

To make sure this thing passes everything, but our explicit postcondition with permutation will _blow it up_, I have designed this "select sort" as follows:

1. If I get empty list, there is nothing to do.
2. I find maximum in the array.
3. For each index from the end, I will assign `maximum + index`.
   This will ensure that even if the maximum in the original array was the first element, I will always satisfy that 2nd part of the loop invariant.

```py showLineNumbers
def broken_select_sort(arr, n):
   assert n == len(arr)

   if not arr:
       return

   max_value = max(arr)

   check_loop_invariant(arr, n, n)
   for i in reversed(range(n)):
       arr[i] = max_value + i

       check_loop_invariant(arr, n, i)

   return arr
```

:::tip

There is also an easier way to break this, I leave that as an exercise ;)

:::

## Property-based tests for our sorts

Since we have talked a lot about proofs at the seminar, I would like to demonstrate it on the testing of the sorts. In the following text I will cover implementation of the loop invariant and both postconditions we have talked about and then test our sorts using them.

### Loop invariant

To check loop invariant I have implemented this function:

```py showLineNumbers
def check_loop_invariant(arr, n, i):
    # A[i + 1 : n] is sorted
    for x, y in zip(itertools.islice(arr, i + 1, n), itertools.islice(arr, i + 2, n)):
        assert x <= y

    # all elements of A[i + 1 : n] are bigger or equal to the other elements
    if i + 1 >= n:
        # in case there are no elements
        return

    # otherwise, since the "tail" is sorted, we can assume that it is enough to
    # check the other elements to the smallest value of the tail
    smallest = arr[i + 1]
    for element in itertools.islice(arr, i + 1):
        assert smallest >= element
```

First part checks if the "ending" of the array is sorted.

In second part I have used a _dirty trick_ of taking just the first element that is the smallest and compared the rest of the elements to it. Why is it enough? I leave it as an exercise ;)

### Postcondition(s)

I have defined both the vague and explicit postconditions:

```py showLineNumbers
def check_vague_postcondition(original_arr, arr):
    if not arr:
        return

    # check ordering
    for x, y in zip(arr, itertools.islice(arr, 1, len(arr))):
        assert x <= y


def check_postcondition(original_arr, arr):
    if not arr:
        return

    # check ordering
    for x, y in zip(arr, itertools.islice(arr, 1, len(arr))):
        assert x <= y

    # get counts from original list
    original_counts = {}
    for value in original_arr:
        original_counts[value] = 1 + original_counts.get(value, 0)

    # get counts from resulting list
    counts = {}
    for value in arr:
        counts[value] = 1 + counts.get(value, 0)

    # if arr is permutation of original_arr then all counts must be the same
    assert counts == original_counts
```

### Putting it together

Now that we have everything implement, we can move on to the implementation of the tests:

```py showLineNumbers
from hypothesis import given, settings
from hypothesis.strategies import integers, lists
import pytest

@given(lists(integers()))
@settings(max_examples=1000)
@pytest.mark.parametrize(
    "postcondition", [check_vague_postcondition, check_postcondition]
)
@pytest.mark.parametrize("sorting_function", [select_sort, broken_select_sort])
def test_select_sort(sorting_function, postcondition, numbers):
    result = sorting_function(numbers[:], len(numbers))
    postcondition(numbers, result)
```

Since it might seem a bit scary, I will disect it by parts.

1. Parameters of test function

   ```py showLineNumbers
   def test_select_sort(sorting_function, postcondition, numbers):
   ```

   We are given 3 parameters:

   - `sorting_function` - as the name suggests is the sorting function we test
   - `postcondition` - as the name suggests is the postcondition that we check
   - `numbers` - is random list of numbers that we will be sorting

2. Body of the test

   ```py showLineNumbers
   result = sorting_function(numbers[:], len(numbers))
   postcondition(numbers, result)
   ```

   We pass to the sorting function **copy** of the numbers we got, this ensures that once we are checking the more strict postcondition, we can gather the necessary information even after sorting the list in-situ, i.e. we can check if the `result` is really a `permutation` of the `numbers` even though the sorting functions has modified the passed in list.

:::caution

Now we get to the more complicated part and it is the _decorators_.

:::

3. 1st `parametrize` from the bottom

   ```py showLineNumbers
   @pytest.mark.parametrize("sorting_function", [select_sort, broken_select_sort])
   ```

   This tells pytest, that we want to pass the values from the list to the parameter `sorting_function`. In other words, this lets us use the same test function for both the correct and incorrect select sort.

4. 2nd `parametrize` from the bottom is similar, but works with the postcondition.
   The reason why they are separated is pretty simple, this way they act like cartesian product: for each sorting function we also use each postcondition.

5. `@settings` raises the count of tests that hypothesis runs (from default of 100(?)).

6. `@given(lists(integers()))`
   This means hypothesis is randomly creating lists of integers and passing them to the function, which has only one parameter left and that is `numbers`.

### Let's run the tests!

In case you want to experiment locally, you should install `pytest` and `hypothesis` from the PyPI.

```
% pytest -v test_sort.py
=================================== test session starts ====================================
platform linux -- Python 3.6.8, pytest-3.8.2, py-1.7.0, pluggy-0.13.1 -- /usr/bin/python3
cachedir: .pytest_cache
rootdir: /home/xfocko/git/xfocko/ib002/postcondition-ambiguity, inifile:
plugins: hypothesis-5.16.1
collected 4 items

test_sort.py::test_select_sort[select_sort-check_vague_postcondition] PASSED         [ 25%]
test_sort.py::test_select_sort[select_sort-check_postcondition] PASSED               [ 50%]
test_sort.py::test_select_sort[broken_select_sort-check_vague_postcondition] PASSED  [ 75%]
test_sort.py::test_select_sort[broken_select_sort-check_postcondition] FAILED        [100%]

========================================= FAILURES =========================================
_________________ test_select_sort[broken_select_sort-check_postcondition] _________________

sorting_function = <function broken_select_sort at 0x7fac179308c8>
postcondition = <function check_postcondition at 0x7fac1786d1e0>

    @given(lists(integers()))
>   @settings(max_examples=1000)
    @pytest.mark.parametrize(
        "postcondition", [check_vague_postcondition, check_postcondition]
    )
    @pytest.mark.parametrize("sorting_function", [select_sort, broken_select_sort])
    def test_select_sort(sorting_function, postcondition, numbers):

test_sort.py:132:
_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
test_sort.py:139: in test_select_sort
    postcondition(numbers, result)
_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _

original_arr = [0, 0], arr = [0, 1]

    def check_postcondition(original_arr, arr):
        if not arr:
            return

        # check ordering
        for x, y in zip(arr, itertools.islice(arr, 1, len(arr))):
            assert x <= y

        # get counts from original list
        original_counts = {}
        for value in original_arr:
            original_counts[value] = 1 + original_counts.get(value, 0)

        # get counts from resulting list
        counts = {}
        for value in arr:
            counts[value] = 1 + counts.get(value, 0)

        # if arr is permutation of original_arr then all counts must be the same
>       assert counts == original_counts
E       assert {0: 1, 1: 1} == {0: 2}
E         Differing items:
E         {0: 1} != {0: 2}
E         Left contains more items:
E         {1: 1}
E         Full diff:
E         - {0: 1, 1: 1}
E         + {0: 2}

test_sort.py:128: AssertionError
----------------------------------- Captured stdout call -----------------------------------
Falsifying example: test_select_sort(
    sorting_function=<function test_sort.broken_select_sort>,
    postcondition=<function test_sort.check_postcondition>,
    numbers=[0, 0],
)
============================ 1 failed, 3 passed in 6.84 seconds ============================
```

We can clearly see that our broken select sort has passed the _vague postcondition_, but the explicit one was not satisfied.

## Summary

For proving the correctness of the algorithm it is better to be explicit than prove that algorithm is correct even though it is not. Being explicit also allows you to test smaller _chunks_ of code better.
