git-restrict
============

![Build Status](https://github.com/parazyd/git-restrict/actions/workflows/c-cpp.yml/badge.svg)

A minimal utility that allows repository permission management based on
ssh keys when used with the command directive in ssh's authorized_keys
file.

If used, it will only allow `git-upload-pack` and `git-receive-pack` as
the commands allowed to be ran by a specific user/SSH key.

git-restrict is C99 portable and compiled as a static binary so it's
easy to use it in chroot environments. This is obviously intentional.


Basic usage
-----------

Set up SSH and a `git` user on a host machine that will serve the git
repositories. Ideally the repos should be stored in the user's `$HOME`.

After this, compile and install git-restrict and use something like
the following to set repo permissions for specific SSH keys:

```
$ cat ~/.ssh/authorized_keys
command="/usr/bin/git-restrict repo0 repo1 repo2" ssh-ed25519 AAA...1 user0@machine
command="/usr/bin/git-restrict repo3 repo0" ssh-ed25519 AAA.Z user1@machine
```

It is also useful to see the `contrib` directory to see an automation
example.


License
-------

GNU Affero General Public License version 3.
