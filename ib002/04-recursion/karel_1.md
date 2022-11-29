---
title: Recursion and backtracking with Robot Karel
description: |
  A problem with too many restrictions.
tags:
- python
- karel
- recursion
- backtracking
---

- [Sources](pathname:///files/ib002/karel_1)

## Introduction

In this exercise we will be working with a Robot Karel and with a »very« limited
resources. The point of this exercise is to show how powerful recursion and
backtracking can be even without anything else at your hand.

## Your environment and problem description

### Environment

You are given a robot that is present in a maze and is looking for an exit. Maze
consists of different walls and exit is marked with a single so-called „beeper“.

Walking into a wall results in a permanent damage of the robot.

### Interface

You can control the robot using the following interface:

- actions ​­— you can use them to change the current state of the robot and its
  surroundings
  - `robot.step()` — moves robot one step further
  - `robot.turn_left()` — turns robot 90-degrees counter-clockwise
    - notice that you are not given `turn_right` or `turn_around`, but feel free
      to implement them yourself
  - `robot.pick_beeper()` and opposite operation `robot.put_beeper()` — that
    allows you to either pick or put „beeper“ from or onto the current position
- queries — you can use them to check the current state of the robot and its
  surroundings
  - `robot.beepers_present()` — to check if there are any beepers at the robot's
    current location
  - `robot.left_is_clear()` — to check if you can step to the left
    - analogically for `front` and `right`

:::caution

Helper functions / procedures are allowed. Return values are allowed.

**Variables are prohibited!**

:::

### Problem

Your task is to decide whether there is an exit from the maze or not. You can see
an example of a maze here:

![Image of the maze](/img/ib002/karel_1/maze.png)

## Simple problem to get familiar with the robot

If you feel completely lost after the previous description, let me start you off
with a simpler problem.

You are standing in front of the stairs, your task is to walk up the stairs.

You can see an example of such map here:

![Image of the stairs](/img/ib002/karel_1/stairs.png)

## Brainstorm the idea

As a first step write down any ideas and things that you have noticed or came to
your mind. Ideally:

- Write down a nested list of the problems, e.g.

  :::info Example 

  Problem: I want to find out whether the display on smartphone should rotate.

  - Check if display has been rotated
    - Read data from some sensor
      - From what sensor
    - In what format are the data I have read?
    - How do I communicate with the sensor?
    - What is the meaning of the data that I got?
    - How can I process it?

  :::
- Write down list of problems that can happen, e.g.

  :::info Example continued

  Following the same problem.

  - What if the sensor doesn't work?
  - What if the data doesn't conform to the specification?
  - What if my formulas are wrong?

  :::
- Write down **anything** you consider important to solving the problem, e.g.

  :::info Example continued once again

  - I could probably use gyroscope.
  - I should probably look up the datasheet for that module.
  - I could write some tests to verify that my computations are correct.

  :::

## »Rough« pseudocode

As a next step write a **mock up** of a pseudocode solving the problem, you are
allowed to use comments as placeholders for bigger chunks of code.

Those comments are also a very good hints for decomposition and short, but
descriptive, commnets (if they are short enough and you decide not to factor them
out to separate functions).

:::tip

The smaller the function is, the easier it is to test it and argue about its
correctness.

:::

## »Proper« pseudocode

If you are satisfied with the _»rough« pseudocode_, it's time to convert it into
a proper one. Get rid of the uncertain pieces of functionality and replace them
with proper pseudocode, i.e. list of the things that should happen in its place.

## Library

If you got here, and you **actually** wrote down the pseudocode, you can try your
solution after downloading the sources linked at the beginning. If you download
the ZIP-file, you can there:

- `generate_mazes.py` - that was used to generate the same maze with beepers in
  different locations
- `karel_tk.py` - library which can run Karel given the his world
  - documentation can be found [here](https://www.fi.muni.cz/~xfocko/ib111/10/docs/)
  - also requires Tk Python library to be installed (it should be included in
    majority of Python installations)
- `*.kw` - which represent multiple worlds for Karel I have prepared
- `skeleton.py` - skeleton for your solution, needs to be put in the same directory
  as `karel_tk.py` and takes path to the world as a first argument, example usage:

      $ python3 skeleton.py stairs.kw

  - of course, this file can be renamed ;)

## Solution

Solution to this problem will be released as a second part, so that you can try
it out by yourself without any influence of „example solution“.

If you want to get any feedback, feel free to mail me your solution (including
all the steps that lead to your final solution, if you wish to get feedback on
those too).