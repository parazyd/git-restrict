#!/bin/sh

export failed=0
fail() { echo "Test $1 failed"; failed=1 ; }
pass() { echo "Test $1 passed" ; }

# Test 0
./git-restrict 2>/dev/null && fail 0 || pass 0

# Test 1
./git-restrict foo 2>/dev/null && fail 1 || pass 1

# Test 2
SSH_ORIGINAL_COMMAND="cat /etc/passwd" \
	./git-restrict foo 2>/dev/null && fail 2 || pass 2

# Test 3
SSH_ORIGINAL_COMMAND="git-upload-pack bar" \
	./git-restrict foo bar baz 2>/dev/null && fail 3 || pass 3

# Test 4
SSH_ORIGINAL_COMMAND="git-upload-pack 'bar' ; cat /etc/passwd" \
	./git-restrict bar 2>/dev/null && fail 4 || pass 4

# Test 5 (code 128 given by git-shell when repository isn't available)
SSH_ORIGINAL_COMMAND="git-upload-pack 'bar'" \
	./git-restrict foo bar baz 2>/dev/null
[ $? = 128 ] || fail 5 && pass 5

# Test 6
SSH_ORIGINAL_COMMAND="git-upload-pack 'bar.git'" \
	./git-restrict foo bar baz 2>/dev/null
[ $? = 128 ] || fail 6 && pass 6

# Test 7
SSH_ORIGINAL_COMMAND="git-upload-pack 'foo'" \
	./git-restrict bar 2>/dev/null && fail 7 || pass 7

exit "$failed"
