#!/bin/bash
# Copyright 2011 Bruno Rahle
#
# This file is part of Evaluator.
#
# Evaluator is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# Evaluator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public
# License along with Evaluator.  If not, see
# <http://www.gnu.org/licenses/>.

# Get the data from user
echo -n "Username: "
read username

echo -n "Email: "
read email

# First, init the repository
git clone git@github.com:brahle/eval2.git
name=eval2

# Point the hooks and ignored files to the common config directory
rm -rf $name/.git/hooks
ln -s ../configs/git/hooks/unix $name/.git/hooks
rm -rf $name/.git/info/
ln -s ../configs/git/info $name/.git/info

# Git configuration
cd $name
dir=`pwd`
git config user.name $username
git config user.email $email
git config commit.template $dir/configs/git/commit.template
git config alias.st status
git config alias.co checkout
git config alias.br branch
git config color.branch auto
git config color.diff auto
git config color.interactive auto
git config color.status auto
