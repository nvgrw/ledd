# ledd

It's a work in progress.

Once completed, `ledd` will act a daemon that reads instructions from a pipe
(currently stdin, which I'll probably redirect from the shell) and then executes
them to change the colour of the connected LED strip.

This is primarily made for my own setup, so I've spent little time making it
work for anything else.

Together with `ledd`, I also intend to create `ledc`, which generates `ledd`
intructions with correct sequencing. Currently I'm debating whether to have
`ledd` read data at a rate independent from the input pipe or whether to tie it
to the rate of the pipe. Pattern holding (NOOP) functionality is in place should
I decide to process instructions at a constant rate.

_All rights reserved_
