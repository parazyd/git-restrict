/* Copyright (c) 2021 Ivan J. <parazyd@dyne.org>
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

int main(int argc, char *argv[])
{
	char *orig_cmd, *cmd, *repo, *buf;
	int i, authorized = 0;

	if (argc < 2) {
		fprintf(stderr, "usage: git-restrict repo0 repo1 ...\n");
		return 1;
	}

	if ((orig_cmd = getenv("SSH_ORIGINAL_COMMAND")) == NULL) {
		fprintf(stderr, "fatal: No $SSH_ORIGINAL_COMMAND in env.\n");
		return 1;
	}

	repo = strdup(orig_cmd);

	if ((cmd = strsep(&repo, " ")) == NULL)
		return 1;

	if (strcmp("git-upload-pack", cmd) && strcmp("git-receive-pack", cmd)) {
		fprintf(stderr, "fatal: Unauthorized command.\n");
		return 1;
	}

	/* Remove ' prefix and suffix */
	repo++; repo[strlen(repo)-1] = 0;

	for (i = 1; i < argc; i++) {
		buf = malloc(strlen(repo)+4);
		sprintf(buf, "%s.git", argv[i]);

		if (!strcmp(argv[i], repo) || !strcmp(buf, repo)) {
			authorized = 1;
			free(buf);
			break;
		}

		free(buf);
	}

	if (authorized)
		if (execlp("git-shell", " ", "-c", orig_cmd, (char *)NULL) < 0)
			perror("execlp");

	fprintf(stderr, "fatal: Access to repository denied.\n");
	return 1;
}
