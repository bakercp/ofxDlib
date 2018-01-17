#!/usr/bin/env bash
set -e

# Here we assume that the bootstrap script is in the ADDON_ROOT/scripts directory.
pushd ${ADDON_ROOT:-$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)} > /dev/null
git submodule update --init --recursive
/usr/bin/env bash scripts/ofx/ofx bootstrap
popd > /dev/null
