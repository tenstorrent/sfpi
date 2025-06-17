#! /bin/bash

# $exec OUTPUT-NAME USES-LIST
# Generates a string literal encoding the git repo:
# $UpstreamURL($RemoteBranch) $LocalBranch:$Hash [$N ahead]-opt dirty-opt
# UpstreamURL is either a local pathname, or a git repo

set -e
version="$1"

format () {
    printf "%-17s  %-20s  %-28s  %-12s %s\n" "$@"
}    
get-hash () {
    local dir=$1
    local tag="$(git -C $dir describe --tags --dirty 2>/dev/null)"
    local tagged_head=true
    if echo "$tag" | grep -qe '-[0-9]\+-g[0-9a-f]\+$' ; then
	tagged_head=false
    fi
    local head=$(git -C $dir rev-parse --symbolic-full-name HEAD)
    local branch=
    if [[ $head = "HEAD" ]] ; then
	# detached head
	if ! $tagged_head ; then
	    # Not tagged, figure out a branch name to add
	    local origin=
	    local refs=$(git -C $dir show -s --pretty=%D HEAD 2>/dev/null \
			     | sed -e 's/^HEAD -> //' -e 's/ //g' -e 's/,/ /g')
	    for ref in $refs
	    do
		case $ref in
		    # shouldn't happen
		    tag:*) ;;
		    HEAD) ;;
		    origin/*) origin=${ref#origin/} ;;
		    *) branch=$ref ;;
		esac
	    done
	    if [[ -z $branch ]] ; then
		branch="$origin"
	    fi
	fi
    else
	# not tagged or not main, use branch name
	branch=${head#refs/heads/}
    fi
    local hash="$(git -C $dir rev-parse --short=12 HEAD 2>/dev/null)"
    local url="$(git -C $dir remote get-url origin)"
    format "$dir" "$branch" "$tag" "$hash" "$url"
}

echo "Version: $version"
echo "Built from:"
format Directory Branch Tag-Ref Hash Git-Repo
get-hash .
for submodule in $(git config --file .gitmodules --get-regexp "\.path$" | cut -d' ' -f2)
do
    get-hash ./$submodule
done
