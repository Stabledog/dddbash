dddbash
=======

Streamlining the build+install+running of ddd and bashdb together: a useful combination for debugging shell scripts.   
Targeting Debian/Ubuntu, RHEL/Centos, and Cygwin/X.

The inputs to this project are bashdb (http://bashdb.sourceforge.net/) and DDD (http://www.gnu.org/software/ddd/). 
In theory, these two are able to work together to provide a multi-window debugger for bash scripts.

But in practice, there are problems:  DDD 3.3.12 (the current latest version) chokes when using bashdb 
as the inferior debugger on Ubuntu 12.x or Cygwin/X.   In addition, the command line launch using this combination
is cumbersome, and there's little guidance available on the web about how to get the most out of this pair if you
do happen to get them working.

This project is intended to rectify that: to provide a relatively clean way to integrate bashdb+ddd so that those wishing
to debug bash scripts in a GUI don't have to re-learn everything I have learned in my quest to make them
behave.

======
To install, clone the repository, and then 

  cd path/to/working-copy
  
  ./build

