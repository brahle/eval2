#!/bin/sh

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
