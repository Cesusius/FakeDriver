# FakeDriver
Execute anything in a legit memory region by attacking a windows driver 
### What are we attacking?
A windows driver named atapi.sys, digitally code signed and automatically running on every Windows 10/11 machines.
This driver calls multiple functions which do not call any external ones and thus could easily be swapped/hooked
### What are the potential detection vectors?
The driver still hooks something so it may be that, but once you're in the function, except if you trigger another flag, you can (almost) do whatever you want.
### Risks of BSOD'ing other than PG?
The function itself is entirely emulated so, no. And no issues will be caused on the behalf of atapi.sys
![image](https://github.com/patchbull/FakeDriver/assets/125939943/41f02d91-8b32-4570-b6e4-efb17f95469a)
