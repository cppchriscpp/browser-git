/*
 * libgit2 "push" example - shows how to push to remote
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
 * This example demonstrates the libgit2 push API to roughly
 * simulate `git push`.
 *
 * This does not have:
 *
 * - Robust error handling
 * - Any of the `git push` options
 *
 * This does have:
 *
 * - Example of push to a named origin, or "master" if not specified.
 * 
 */

/** Entry point for this command */
int lg2_push(git_repository *repo, int argc, char **argv) {
	git_push_options options;
	git_remote* remote = NULL;
	git_reference* repo_head;
	const char *current_branch_name;
	char *refspec;// = "refs/heads/master";
	const git_strarray refspecs = {
		&refspec,
		1
	};

    /* Validate args */
	if (argc > 1) {
		printf ("USAGE: %s\n\nsorry, no arguments supported yet\n", argv[0]);
		return -1;
	}

	check_lg2(git_repository_head(&repo_head, repo), "Failed getting repository head", NULL);
	check_lg2(git_branch_name(&current_branch_name, repo_head), "Failed getting current branch name", NULL);
	refspec = malloc(strlen("refs/heads/") + strlen(current_branch_name));

	if (refspec == NULL) {
		git_reference_free(repo_head);
		printf("Failed allocating memory for branch name\n");
		return -1;
	}
	strcpy(refspec, "refs/heads/");
	strcat(refspec, current_branch_name);

	check_lg2(git_remote_lookup(&remote, repo, "origin" ), "Unable to lookup remote", NULL);
	
	check_lg2(git_push_options_init(&options, GIT_PUSH_OPTIONS_VERSION ), "Error initializing push", NULL);

	check_lg2(git_remote_push(remote, &refspecs, &options), "Error pushing", NULL);

	free(refspec);
	git_reference_free(repo_head);
	printf("pushed\n");
	return 0;
}
