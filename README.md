Richard Heng
---

How to run file
------------

1. Clone `https://github.com/rheng001/rshell.git`
2. `cd rshell`
3. `git checkout hw2`
4. `make`
5. `bin/rshell`

-----
Overview

To demostrate the basic abilities of a command shell. 

Known Bugs
---

Unable to do the following:

1. Can't run command with quotes
    e.g `"pwd"`
2. Cant handle `&` as connector
3. Cant have different multiple connectors in one line
    e.g `pwd || pwd && ls; ls;`
4. Cant combine commands with exit
    e.g `ls && ls && exit`
        however single command exit works fine
        e.g `exit`
5. test command is unable to handle brackets
6. Not able to get parenthesis function working
7. Precedance operators not working.
8. Must have a test/path/file test path otherwise make dir/file seperately.
