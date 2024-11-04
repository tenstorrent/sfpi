#! /bin/bash

# $exec OUTPUT-NAME USES-LIST
# Generates a string literal encoding the git repo:
# $UpstreamURL($RemoteBranch) $LocalBranch:$Hash [$N ahead]-opt dirty-opt
# UpstreamURL is either a local pathname, or a git repo


set -e

get-hash () {
    local dir=$1
    local warning=true

    local branches="$(git -C $dir log --pretty=format:%d -1 | sed -e 's/[(),]//g' -e 's/: /:/g')"
    local branch
    local found
    local output
    for branch in $branches ; do
	case "$branch" in
	    origin/*)
		found="$branch"
		warning=false
		break ;;
	    tag:*) # have to guess at origin here
		found=origin/"${branch#tag:}"
		warning=false
		break ;;
	    "->") ;;
	    HEAD) ;;
	    *) found="$branch" ;;
	esac
    done
    output+="$found:$(git -C $dir rev-parse --short=8 HEAD 2>/dev/null)"
    if test "$(git -C $dir status --porcelain=v1 | wc -l)" != 0 ; then
       output+=" dirty"
       warning=true
    fi

    echo "$dir $output"
    if $warning ; then
	echo "*** WARNING: $dir $output IS NOT UPSTREAM ***" 1>&2
    fi
}

get-hash .
for submodule in $(git config --file .gitmodules --get-regexp "\.path$" | cut -d' ' -f2)
do
    get-hash $submodule
done
