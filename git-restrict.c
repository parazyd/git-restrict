/* Copyright (c) 2021-2022 Ivan J. <parazyd@dyne.org>
 *
 * This file is part of git-restrict
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void die(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

static char *strdup(const char *s)
{
	size_t sz = strlen(s)+1;
	char *d = malloc(sz);
	if (!d) return NULL;
	return memcpy(d, s, sz);
}

int main(int argc, char *argv[])
{
	char *orig_cmd, *cmd, *repo, *buf;
	char git_cmd[4096];
	int i, authorized = 0;

	if (argc < 2)
		die("usage: git-restrict repo0 repo1 ...");

	if ((orig_cmd = getenv("SSH_ORIGINAL_COMMAND")) == NULL)
		die("fatal: No $SSH_ORIGINAL_COMMAND in env.");

	if ((repo = strdup(orig_cmd)) == NULL) die("fatal: Internal error.");
	if ((cmd = strtok(repo, " ")) == NULL) die("fatal: Invalid command.");
	repo = strtok(NULL, " ");

	if (strcmp("git-upload-pack", cmd) && strcmp("git-receive-pack", cmd))
		die("fatal: Unauthorized command.");

	/* Repository name should at least be: 'a' */
	if (repo == NULL || (strlen(repo) < 3))
		die("fatal: Invalid repository name.");

	/* Remove ' and / prefix and ' suffix */
	repo++; if (repo[0] == '/') repo++; repo[strlen(repo) - 1] = 0;

	for (i = 1; i < argc; i++) {
		/* This is so both "foo" and "foo.git" are supported */
		if ((buf = malloc(strlen(repo) + 4)) == NULL) {
			perror("malloc");
			return 1;
		}

		snprintf(buf, strlen(repo) + 4, "%s.git", argv[i]);

		if (!strcmp(argv[i], repo) || !strcmp(buf, repo)) {
			authorized = 1;
			free(buf);
			break;
		}

		free(buf);
	}

	if (!authorized)
		die("fatal: Access to repository denied.");

	snprintf(git_cmd, strlen(cmd) + strlen(repo) + 4, "%s '%s'", cmd, repo);

	if (execlp("git-shell", "git-shell", "-c", git_cmd, (char *)NULL) == -1)
		perror("execlp");

	return 1;
}
