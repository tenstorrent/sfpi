#! /bin/bash

pre=${1-}
post=${2-}

# match and exclude are globs, not regexes. This is close enough.
if ! version=$(git describe --tags --match '[0-9]*.[0-9]*.[0-9]*' \
		   --exclude '*-*' 2>/dev/null) ; then
    # legacy version numbering with a 'v' prefix.
    version=$(git describe --tags --match 'v[0-9]*.[0-9]*.[0-9]*' \
		  --exclude 'v*-*' 2>/dev/null \
		  | sed 's/^v//' || true)
    if ! [[ $version ]]; then
	version=0
    fi
fi
tagged_head=true
if echo "$version" | grep -qe '-[0-9]\+-g[0-9a-f]\+$' ; then
    tagged_head=false
fi
head=$(git rev-parse --symbolic-full-name HEAD)
branch=
if [[ $head = "HEAD" ]]; then
    # detached head
    if ! $tagged_head ; then
	# Not tagged, figure out a branch name to add
	origin=
	refs=$(git show -s --pretty=%D HEAD 2>/dev/null \
		   | sed -e 's/^HEAD -> //' -e 's/ //g' -e 's/,/ /g' \
		   || true)
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
	if [[ -z $branch ]]; then
	    branch="$origin"
	fi
    fi
elif ! $tagged_head || [[ $head != refs/heads/main ]]; then
    # not tagged or not main, use branch name
    branch=${head#refs/heads/}
fi

if [[ -n "$branch" ]]; then
    # just use the final component of a branch name
    version="${version%-*-g*}-${branch##*/}"
fi

url=$(git config --get remote.origin.url \
	  | sed -e 's/[^:]*://' -e 's+//[^/]*/++' \
		-e 's+/[^/]*\(\.git\)\?$++')
if [[ $url != 'tenstorrent' ]]; then
    version="$url-$version"
fi

echo $pre${pre:+ }$version${post:+ }$post
