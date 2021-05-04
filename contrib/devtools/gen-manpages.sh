#!/usr/bin/env bash
# Copyright (c) 2016-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

NEOND=${NEOND:-$BINDIR/neond}
NEONCLI=${NEONCLI:-$BINDIR/neon-cli}
NEONTX=${NEONTX:-$BINDIR/neon-tx}
WALLET_TOOL=${WALLET_TOOL:-$BINDIR/neon-wallet}
NEONQT=${NEONQT:-$BINDIR/qt/neon-qt}

[ ! -x $NEOND ] && echo "$NEOND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a NEONVER <<< "$($NEONCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }')"

# Create a footer file with copyright content.
# This gets autodetected fine for neond if --version-string is not set,
# but has different outcomes for neon-qt and neon-cli.
echo "[COPYRIGHT]" > footer.h2m
$NEOND --version | sed -n '1!p' >> footer.h2m

for cmd in $NEOND $NEONCLI $NEONTX $WALLET_TOOL $NEONQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${NEONVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${NEONVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
