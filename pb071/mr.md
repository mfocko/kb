---
title: Submitting merge requests
---

# Submitting merge requests for review

This tutorial aims to show you how to follow basic git workflow and submit changes
through _Merge Requests_ for review.

The rudimentary idea behind aims for changes to be present on a separate branch
that is supposedly _merged_ into the default branch. Till then changes can be reviewed
on _Merge Request_ and additional changes may be made based on the reviews. Once
the changes satisfy requirements, the merge request is merged.

## Tutorial

> Use this tutorial only for bonus assignments **made by your tutors** or in case
> you need to make up for the absence.

### Step #1 - Starting from the clean repository

In your repository (either locally or on aisa) type `git status` and check if your
repository is clean and you are present on the main branch (`master`, `main` or
`trunk`). If you do not know what your default branch is, it is probably `master`
and you should not be on any other branch.

Output of the command should look like this:

```
aisa$ git status
On branch master        # Or main or trunk.
Your branch is up to date with 'origin/master'.

nothing to commit, working tree clean
```

> In case you are on different branch or there are uncommitted changes,
> **do not continue!!!** Clean your repository (commit the changes or discard
> them), before you continue.

### Step #2 - Create new branch

In your repository write command:

```
aisa$ git checkout -b BRANCH
Switched to a new branch 'BRANCH'
```

Instead of `BRANCH` use some reasonable name for the branch. For example if you
are working on the seminar from 3rd week, name the branch `seminar-03`.

### Step #3 - Do the assignment

Download the skeleton for the seminar assignment, extract and program. For example
if you are working on 3rd seminar, you can do so by:

```
aisa$ wget https://www.fi.muni.cz/pb071/seminars/seminar-03/pb071-seminar-03.zip
aisa$ unzip pb071-seminar-03.zip
# Now you should have directory 'seminar-03'.
aisa$ rm pb071-seminar-03.zip
aisa$ cd seminar-03
# You can work on the assignment.
```

### Step #4 - Commit and upload the changes to GitLab

The same way you _add_ and _commit_ files for the homework assignments, you do for
the seminar.

Now you can upload the changes to GitLab. `git push` is not enough, since repository
on GitLab does not know your new branch. You can solve this by adding arguments:

```
aisa$ git push origin BRANCH
...
remote: To create a merge request for BRANCH, visit:
remote:   https://gitlab.fi.muni.cz/login/pb071/merge_requests/new?merge_request%5Bsource_branch%5D=BRANCH
...
```

In the output you should have a link for creating a merge request. If you see this
link, open it and skip next step.

### Step #5 - Creating a merge request manually

1. Open your repository on GitLab.
2. On the left panel click on _Merge Requests_.
3. Click on _New Merge Request_.
4. In _Source branch_ select `login/pb071` and `BRANCH`, which you created.
5. In _Target branch_ select `login/pb071` and your default branch you have seen
   in the output of the first command. (most likely `master`)
6. Click on _Compare branches and continue_.

### Step #6 - Set assignees

On the page that is opened, please check at the top that you are creating merge
request **from** your new branch **to** your default branch (one of `master`, `main`
or `trunk`).

Then in the field _Assignees_ set your tutors based on the seminar group. You can
use login for a quick look up.

In the end click on _Submit merge request_.

### Step #7 - Return to default branch

Homework assignments can be submitted only from branches specified in the rules
for the course. Because of that, before you do anything else, you should switch
back to your default branch.

First of all, same as in step #1, check that your repository is clean with `git status`.
For the sake of safety, do not continue without clean repository. Then with command
`git checkout BRANCH` switch to your default branch `BRANCH`.

If you do not know which branch is your default, try `git branch` that outputs all branches in your repository. Default branch is typically `master`, but can
be `main` or `trunk`.

```
aisa$ git status
# Check if repository is clean

# If you know, what is your default branch, you can skip next command.
aisa$ git branch
# Find the default branch in the list; should be one of the `master`, `main` or
# `trunk` and you should not have more than one of those.
# In case the list clears the terminal and you cannot see shell prompt, you can
# press `q` to quit the pager.

aisa$ git checkout master
```

---

Adapted from: https://www.fi.muni.cz/~xlacko1/pb071/mr.html
