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

Input **may contain empty lines** - these must be ignored!

No input line will be longer than 256 characters including newline character.

There are three valid `KEYWORD` values: `NAME`, `FATHER`, `MOTHER`. Keyword `NAME` **starts a new person record**. The following `FATHER` and `MOTHER` lines are optional and contain the names of the parents of this person. A person can have information about both parents, a single parent or no parent at all. Even though life is more complex, for ease of the excercise, each person can have **no more than one** `FATHER` and **no more than one** `MOTHER` and does not change their gender.

You can assume, that all persons are **strictly divided in generations** and persons of different generations do not pair up.

Single person can have **multiple kids** - either from the same or from a **different partner**!

There can be multiple family trees that are **not connected**!

Example input:

    NAME Son son
    FATHER Father
    MOTHER Mother
    
    NAME Father
    MOTHER Granny
    NAME OtherSon
    MOTHER OtherMother
    FATHER OtherFather
    
**ON ERRORS**
Input data can also be erronous. Whenever You detect an error, the program must provide appropriate error message and exit with code `-1`. If no errors occur - it should exit with code `0`.

Errors can be either malformed/erronous data (wrong keyword, multiple mothers of fathers for a person) or related to impossible logic (links create loops, or people of different generations have children together).

**OUTPUT DATA** The output data must go to `stdout` so that it can be tested by redirecting output to any file with `>`.

In the output You must print person names in the **order from oldest to youngest** (parents first).

The output data can contain multiple family trees which are not linked. In that case these are printed separately and divided by an empty line. Order of the trees does not matter:

    FamilyTree1
    
    FamilyTree2
    
    FamilyTree3
    
Be careful - when building a family trees, some might look like unconnected and later become connected through common ancestor or common sibling.

Each family tree itself is printed one name per line without empty lines in between in order from oldest to youngest person. In cases where two people are of the same age, the all possible orders are correct, e.g. if A is `FATHER` of B and A is `FATHER` of C, then both ABC and ACB are correct orders.

Example output for the example input provided above is:

    Granny
    Father
    Mother
    Son son
    
    OtherMother
    OtherFather
    OtherSon

**TASK1** Read person data from stdin.

This and the following tasks will not be given in such detail as in HW1, so that You have to think more about the work structure of how to approach the task.

Create a function `int read_line(int fd, char* buffer)` that takes a file descriptor and does the following:

1) Tries to read a single line that is not empty (skips the empty lines). If it fails (no non-empty line available) it returns `-1` right away.
2) Otherwise, on non-empty line it checks wether it starts with a correct `KEYWORD`. If it does not, it returns `-2` right away.
3) If the line contains a correct `KEYWORD`, it copies the `some person name` to the string `buffer` so that it can be accessed from the calling function. Don't forget the `\0` at the end, and do not put `\n` at the end or beginning.
4) Then when the name is extracted and put in the buffer, the function returns one of the following values depending on the `KEYWORD` value: `0` if `NAME`, `1` if `MOTHER` and `2` if `FATHER`.

**TASK2** Decide on a data structure where You will store the family trees. This can be any structure, but be sure that it supports multiple separate trees, it can have them joined into single tree later and can contain people with single or no parent as well as multiple children as per task restrictions above. You will probably want to later implement functions to search for a specific person in the data structure.

**TASK3** Create a stub for new function `int add_person(char* name, char* father, char* mother, POINTER_TO_YOUR_DATA_STRUCTURE)`.

You will later implement more complex logic, but for now, just make it so that it checks wether a correct name (not `NULL`) is passed, and sets up two local variables: `int known_mother` and `int known_father` which are set to `0` if the respective parameter is `NULL` or empty string and `1` otherwise.

For now the function should return `0`, but you should assume in Your main code, that it will return a negative value in case of error (different value for each error You might think of).

Later You should return and implement how this function actually adds the new person to Your data structure - this includes logic for finding the person and its parents. Crating them if they didn't already exist or linking them if they do - even if they are in separate family trees for now. If this joins two family trees, these trees must be combined into one.

**TASK4** Create a function `int print_persons(int fd, POINTER_TO_YOUR_DATA_STRUCTURE)` which takes Your data structure and prints it to `fd`, as required. To test it You can temporarily fill the structure with fake data, so that You can check it works, before moving to the next task. This will allow You to debug the next step using Your `print_persons` function and not worry that it itself is incorrect.

**TASK5** Create function `int read_persons(int fd, POINTER_TO_YOUR_DATA_STRUCTURE)` that does the following:
It is passed the pointer to Your new data structure and file descriptor. It uses `read_line()` to read all data in a loop until the file ends (`read_line()==-1`). If at any time it encounters errors like multiple parents of the same gender for a single person or if the `read_line()` returns error `<-1`, this function also returns a negative error value, otherwise it does the following, when it has read a full record for person (ends with record for another person or end of file):
1) It calls `add_person()` with the appropriate parameters or `NULL` if those parameters were not defined for this entry.
2) It returns negative error value if `add_person()` returns a negative error value, otherwise it continues adding peron records until end of file and then after the last person is added returns `0`.

**TASK6** Finish up the main program, so that it sets up Your data structure and passes `stdin` to `read_persons()`. Afterwards, if no error occurs, it calls the `print_persons()` with `stdout`. You should also process the error codes here (or have a `print_error(int error_code)` function).

Hopefully the program now works as described above.
