# Kernel intro

Files associated with the talk for Root-Me about Linux kernel exploitation

*TODO - Add youtube link*

## How to run

```bash
qemu-system-x86_64 -no-reboot \
    -m 256M\
    -kernel bz-image \
    -initrd $2  \
    -cpu kvm64,-smep,-smap \
    -append "console=ttyS0 oops=panic panic=1 nopti nokaslr quiet" \
    -monitor /dev/null \
    -serial mon:stdio \
    -virtfs local,path=/tmp/qmemu-share/,mount_tag=host0,security_model=passthrough,id=foobar \
    -nographic
```

The two kernel modules 

## How to run an exploit

If you used the command line above, a shared folder between your host and the virtual machines might be up.  
Compile your exploit statically and copy it to `/tmp/qmemu-share/`.  
Then run in inside the vm `/tmp/mount/exploit`