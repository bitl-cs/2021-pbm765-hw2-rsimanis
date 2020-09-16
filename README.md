# Homework 2

## Overall process of submitting HW1
Please follow these steps to develop and submit the HW successfully:
1) **Follow the instructions precisely!** Ask if You don't understand them!
2) Check out this Git repository and commit Your changes often - **often and granular commits** with easy to understand improvements described in comments will both make it easy for me to understand Your though process and allow Yourself to easily overlook Your progress and revert specific changes in cases of errors.
3) I will only evaluate (and comment on/provide suggestions) on Your homework **once** (except if You request specific help beforehand)
4) By default I will evaluate the last commit pushed to this repository before the deadline announced. In case *You request it*, I will evaluate the latest commited code by applying the appropriate point reduction for late commits.
5) You can request help with understanding the homework and in some cases even with code itself - do this by commiting the code to Git and then sending me Your related question to e-mail. I reserve the right to help only with understanding/debugging, but will not provide any code for You.
6) In addition to commiting Your working code, You also **must fill HW2 Questionnaire in Ortus** - this will be ralated to Your completed code, so do it after the code is mostly done.
7) As stated before - academic integrity is key in any task in this course - violators will face serious consequences - DO NOT COPY or REWRITE CODE by other people. DO NOT EVEN LOOK AT OTHER students HW code before both of You have submitted Your final code. I will not try to distinguish who copied from whom - both parties will face equal punishment. Also don't borrow code from online pages and **don't use any C libraries, that I have not explicitly allowed**.

## Overall task in short
In this Homework You will build a family tree printing program.

## Step by step INSTRUCTIONS for HW2
Follow these steps and commit code after each step. The only allowed external libraries/functions are the ones already presented in lectures, like stdio.h or strings.h

**INPUT DATA** The input data will come from `stdin`(this means that input will usually be passed as redirected file with `<`).

The input data will be formatted as follows:

    KEYWORD some person name
    KEYWORD some person name
    KEYWORD some person name

You can assume the input will fit in the RAM so it will not be excessively large, but some tests might have on the scale of 1000 input lines.

Input can contain empty lines - these must be ignored!

No input line will be longer than 256 characters including newline character.

There are three valid `KEYWORD` values: `NAME`, `FATHER`, `MOTHER`. Keyword `NAME` **starts a new person record**. The following `FATHER` and `MOTHER` lines are optional and contain the names of the parents of this person. A person can have information about both parents, a single parent or no parent at all. Even though life is more complex, for ease of the excercise, each person can have **no more than one** `FATHER` and **no more than one** `MOTHER`. 

**ON ERRORS**
Input data can also be erronous. Whenever You detect an error, the program must provide appropriate error message and exit with code 1. If no errors occur - it should exit with code 0.

Errors can be either malformed/erronous data or related to impossible logic. E.g. if the links create loops, or people of different generations have children together.


**TASK1** Read person data from stdin .

In file `TODO` create TODO

    $ gcc hw2.c TODO
    
TODO
