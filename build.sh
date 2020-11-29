#!/bin/bash

HASH='61bb8b2241d4db08bea4261c82e27cd9797099e7'
start_dir="${PWD}"
build_dir="/tmp/sl_make"

[[ -d "${build_dir}" ]] && rm -rf "${build_dir}"

git clone "https://git.suckless.org/dwm" "${build_dir}"
cd "${build_dir}"
# git clean -df
# git fetch --all
git reset --hard ${HASH}

cd ${build_dir}
echo "Copying config file..."
cp "${start_dir}/config.h" "${build_dir}/"
echo "Copying patch files..."
cp "${start_dir}/"*.diff "${build_dir}/"
echo

i=0
for patchfile in *.diff ; do
	printf '%s\n' "===> applying ${patchfile}..."
	git apply "$patchfile" || exit 1
	i=$((i+1))
done
printf "%q patches applied\n\n" $i

if [[ $1 == install ]]; then
		sudo make install
else
		make
fi
