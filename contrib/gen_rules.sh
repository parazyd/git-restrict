#!/bin/sh

misc="no-port-forwarding,no-X11-forwarding,no-agent-forwarding,no-pty"

gen_for_keys() {
	while read -r key; do
		printf "command=\"/usr/bin/git-restrict %s\",%s %s\n" "$@" "$misc" "$key"
	done
}

# Admin, let's say they don't need "command" guards.
cat keys/user0

# Users which we want to cap repos for.
repos_user1="some-repository another-repo"
gen_for_keys "$repos_user1" < keys/user1
