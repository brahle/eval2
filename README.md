Evaluator
=========

This is the repository that we will use to develop the new evaluator.
Please read this README, as it will help you not anger other developers.


Download
--------

To download the repository, you should use the init_git.sh script which can be
found [here](https://github.com/brahle/eval2/tree/master/scripts/init_git.sh).
Before you download it, be sure to contact me at [username]@gmail.com so I can
add you to the list of collaborators. Also, do not forget to have your ssh-key
ready ([instructions here](http://help.github.com/linux-key-setup/)).

The `init_git.sh` script will create a directory `eval2` where the repository
will be located. It should also set the commit template and some other
configuration files as needed.

To get the required packages for development, you should run `get_packages.sh`
and `install_thrift.sh` scirpts. Feel free to update them if they are faulty.


Committing Code
---------------

Please use the template given in `configs/git/commit.template`. The title
should be at most 50 characters long, and there should be a blank line between
it and the rest of the message. No line should be longer than 80 characters.

You should also add yourself to the Developers section. 


Code Conventions
----------------

We will be using
[Google style guide](http://code.google.com/p/google-styleguide/) as our
reference. For more details, please visit language specific sections of it:

* [C++](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
* [Python](http://google-styleguide.googlecode.com/svn/trunk/pyguide.html)


Code Review
-----------

Code review is not yet ready. We are still searching for a server to host
ReviewBoard code review system.


Developers
----------

Sorted in alphabetical order:

* Anton Grbin
* Matija Osrecki
* Filip Pavetic
* Ivan Pilat
* Bruno Rahle

