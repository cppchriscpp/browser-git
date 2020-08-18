/*
 * libgit2 "revert" example - reverts a commit
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
 * This example demonstrates the libgit2 revert API. It does not support
 * most of the options of "git revert"
 * 
 * This does not have:
 * - Any command line options
 * - Support for non-commit objects (like HEAD)
 *
 * This does have:
 * - Support for reverting a commit given an exact id
 *
 */

static void print_usage(void)
{
		fprintf(stderr, "usage: revert <commit_id>\n");
		exit(1);
}


/** Entry point for this command */
int lg2_revert(git_repository *repo, int argc, char **argv) {
    git_oid oid;
	git_commit *commit = NULL;
    git_repository_state_t state;

	/* Validate args */
	if (argc <= 1) {
		print_usage();
	}

	/** Make sure we're not about to checkout while something else is going on */
	state = git_repository_state(repo);
	if (state != GIT_REPOSITORY_STATE_NONE) {
		fprintf(stderr, "repository is in unexpected state %d\n", state);
		exit(1);
	}

    check_lg2(git_oid_fromstrp(&oid, argv[1]), "Could not get given commit id into an oid for revert", NULL);
	check_lg2(git_commit_lookup(&commit, repo, &oid), "Could not look up commit from oid for revert", NULL);

    check_lg2(git_revert(repo, commit, NULL), "Failed reverting given commit.", NULL);

    printf("Revert complete!\n");

	return 0;
}
