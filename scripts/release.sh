#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! [[ "$BIN" -ef "scripts" ]]; then
    echo "run this script from repo's top directory" >&2
    exit 1
fi

BUILD=build
ci=false
force=false
tt_built=false
txz_only=false
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--ci) ci=true ;;
	--dir=*) BUILD="${1#*=}" ;;
	--force) force=true ;;
	--tt-built) tt_built=true ;;
	--txz-only) txz_only=true ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

if [[ -r $BUILD/version ]]; then
     tt_version=$(cat $BUILD/version)
else
    echo "No $BUILD/version file present" >&2
    exit 1
fi
echo "INFO: Version: $tt_version"

if ! $ci ; then
    # extract git hashes for here and each submodule
    $BIN/git-hash.sh "$tt_version" > $BUILD/README.post
    if ! cmp -s $BUILD/sfpi/README.txt $BUILD/README.post ; then
	echo "*** WARNING: Source tree has changed since build started ***" >&2
	if ! $force ; then
	    exit 1
	fi
    fi
fi

# Copy include tree
tar cf - include | tar xf - -C $BUILD/sfpi

find $BUILD/sfpi/compiler -type f -executable -exec file {} \; | \
    sed -e '/ELF ..-bit /{s/:.*//;p}' -e d | xargs strip -g

eval $($BIN/sfpi-info.sh RELEASE $tt_version)

rm -rf $BUILD/release
mkdir $BUILD/release

tar cJf $BUILD/release/$sfpi_filename.txz -C $BUILD sfpi
echo "INFO: Tarball: $BUILD/release/$sfpi_filename.txz"

if ! $txz_only ; then
    # Get the set of shared objects we use and figure what packages they come from.
    # tuples of debian:fedora names
    pkgs=
    for so in $(find $BUILD/sfpi/compiler -type f -executable -exec file {} \; | \
		    sed -e '/ELF ..-bit /{s/:.*//;p}' -e d | xargs ldd | \
		    sed -e '/ => /{s/^'"\t"'\([^ ]*\).*/\1/;p}' -e d | sort -u)
    do
	case $so in
	    /*/ld-linux-*.so*) ;; # loader
	    linux-vdso.so*) ;;  # kernel vdso
	    libc.so*) pkgs+='libc6:glibc' ;; # c library
	    libexpat.so*) pkgs+=" libexpat1:expat";;
	    libgcc_s.so*) ;; # compiler support
	    libgmp.so*) pkgs+=" libgmp10:gmp";;
	    libisl.so*) pkgs+=" libisl23:isl";;
	    liblzma.so*) pkgs+=" liblzma5:xz-libs";;
	    libm.so*) ;; # c library
	    libmpc.so*) pkgs+=" libmpc3:libmpc";;
	    libmpfr.so*) pkgs+=" libmpfr6:mpfr";;
	    libncursesw.so*) pkgs+=" libncursesw6:ncurses-libs";;
	    libstdc++.so*) pkgs+=" libstdc++6:libstdc++" ;; # C++ std lib 
	    libtinfo.so*) pkgs+=" libtinfo6:ncurses-libs";;
	    libz.so*) pkgs+=" zlib1g:zlib-ng-compat";;
	    libzstd.so*) pkgs+=" libzstd1:libzstd";;
	    *) echo "WARNING: unknown shared object $so" >&2 ;;
	esac
    done

    case $sfpi_dist in
	linux)
	    echo "INFO: Building packages for common linux distro kinds"
	    fpm_options=(-s dir -n sfpi -C $BUILD/sfpi --prefix /opt/tenstorrent/sfpi)
	    fpm_options+=(--architecture native)
	    fpm_options+=(--license "Apache 2.0/GPL 2,3/LGPL 2.1")
	    fpm_options+=(--url "https://github.com/tenstorrent/sfpi")
	    fpm_options+=(--description "Tenstorrent SFPI compiler tools")
	    if $tt_built ; then
		fpm_options+=(--maintainer "Tenstorrent <support@tenstorrent.com>" \
					   --vendor "Tenstorrent")
	    else
		fpm_options+=(--maintainer Unmaintained --vendor Unknown)
	    fi

	    deb_deps=()
	    rpm_dep=()

	    for pkg in $pkgs
	    do
		case $sfpi_releaser in
		    debian)
			if version=$(dpkg-query -f '${Version}' -W ${pkg/:*/} 2>/dev/null) ; then
			    if [[ $version =~ ^([0-9]+:)?([0-9.]*) ]]; then
				version=${BASH_REMATCH[2]}
			    else
				version=
			    fi
			fi
			;;
		    fedora)
			if version=$(rpm -q --qf '%{VERSION}' ${pkg/*:/} 2>/dev/null) ; then
			    :
			fi
			;;
		    *)
			version=
			;;
		esac
		if [[ -z "$version" ]]; then
		    echo "WARNING: Cannot determine $pkg version used" >&2
		else
		    echo "INFO: $pkg >= $version"
		    deb_deps+=(--depends "${pkg/:*/} >= $version")
		    rpm_deps+=(--depends "${pkg/*:/} >= $version")
		fi
	    done

	    # _ in version is verboten
	    fpm -t deb -v "${tt_version//_/-}" "${fpm_options[@]}" "${deb_deps[@]}" \
		-p $BUILD/release/$sfpi_filename.deb
	    echo "INFO: Debian: $BUILD/release/$sfpi_filename.deb"

	    # - in version is verboten
	    fpm -t rpm -v "${tt_version//-/_}" "${fpm_options[@]}" "${rpm_deps[@]}" \
		-p $BUILD/release/$sfpi_filename.rpm \
		--rpm-auto-add-directories --rpm-rpmbuild-define "_build_id_links none"
	    echo "INFO: RPM: $BUILD/release/$sfpi_filename.rpm"
	    ;;
	*)
	    echo "WARNING: Unknown packaging system on $sfpi_releaser" >&2
	    ;;
    esac
fi

(cd $BUILD/release ; md5sum -b $sfpi_filename.* > $sfpi_filename.md5)
echo "INFO: MD5: $BUILD/release/$sfpi_filename.md5"
