from pwn import *
context(log_level='debug',arch='amd64',os='linux')
context.terminal = ['tmux', 'splitw', '-h']
p=process("./main")
def dbg():
    gdb.attach(p)
    pause()

dbg()
p.interactive()
