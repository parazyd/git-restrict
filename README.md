git-restrict
============

![Build Status](https://github.com/parazyd/git-restrict/actions/workflows/c-cpp.yml/badge.svg)

A minimal utility that allows repository permission management based on
ssh keys when used with the command directive in ssh's authorized_keys
file.

If used, it will only allow `git-upload-pack` and `git-receive-pack` as
the commands allowed to be ran by a specific user/SSH key.

git-restrict is also compiled as a static binary so it's easy to use it
in chroot environments. This is obviously intentional.


Basic usage
-----------

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
