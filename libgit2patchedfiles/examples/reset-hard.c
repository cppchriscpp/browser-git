/*
 * libgit2 "reset-hard" example - hard resets the repo to whatever commit is in HEAD
 * discarding all local changes.
 *
 * Written by the libgit2 contributors
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include "common.h"

/**
 * This example demonstrates the libgit2 push API to remove all local changes.
 * Good for undoing merges with conflicts to let the user manage them.
 * This does not have:
 *
 * - Any control over what to reset to
 *
 * This does have:
 *
 * - Example of a reset --hard to the current HEAD commit.
 * 
 */

/** Entry point for this command */
int lg2_reset_hard(git_repository *repo, int argc, char **argv) {
    git_object *head_commit_obj = NULL;
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;

    /* Validate args */
	if (argc > 1) {
		printf ("USAGE: %s\n\nsorry, no arguments supported yet\n", argv[0]);
		return -1;
	}
    opts.checkout_strategy = GIT_CHECKOUT_FORCE;
    check_lg2(git_revparse_single(&head_commit_obj, repo, "HEAD"), "Failed getting HEAD commit", NULL);
    check_lg2(git_reset(repo, head_commit_obj, GIT_RESET_HARD, &opts), "Failed resetting repo", NULL);
    check_lg2(git_checkout_head(repo, &opts), "Failed checking out HEAD", NULL);

	printf("Reset complete\n");
	return 0;
}
