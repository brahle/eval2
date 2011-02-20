Evaluator
=========

This is the repository that we will use to develop the new evaluator.
Please read this README, as it will help you not anger other developers.


License
-------

All parts of this project which were written for it are licensed under GNU
Affero General Public License, version 3. There may be sources that are
borrowed from other developers, and those are appropiately marked.


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

You should also add yourself to the Developers section as soon as you commit
your first code to the repository.

Whenever you add/change a file, modify the Copyright notice at the top of the
file to incorporate your name. It should be formatted as

    Copyright <year> <first name> <last name>

Please keep the list of the developers sorted first by the year, with newest at
the bottom, then by last name. Do not forget to add the statement of copying
permissions right bellow the notice.

    This file is part of Evaluator.

    Evaluator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Evaluator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public
    License along with Evaluator. If not, see
    <http://www.gnu.org/licenses/>.


Code Conventions
----------------

We will be using
[Google style guide](http://code.google.com/p/google-styleguide/) as our
reference. For more details, please visit language specific sections of it:

* [C++](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
* [Python](http://google-styleguide.googlecode.com/svn/trunk/pyguide.html)


Code Review
-----------

Code review is not ready yet. We are still searching for a server to host
ReviewBoard code review system.


Developers
----------

Sorted in alphabetical order:

* Anton Grbin
* Matija Osrecki
* Filip Pavetic
* Ivan Pilat
* Bruno Rahle - [email](mailto:brahle+eval2@gmail.com)
