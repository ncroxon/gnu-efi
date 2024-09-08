#!/usr/bin/env bash

## @file
#  Test script for gnu-efi.
#
#  Copyright (c) 2023-2024, Pete Batard <pete@akeo.ie>
#
#  SPDX-License-Identifier: GPL-2.0-or-later
#
##

TEST_DIR=./run
LINE_LEN=64
TIMEOUT=2m
MIN_TEST="${1:-001}"
MAX_TEST="${2:-999}"
NUM_PASS=0
NUM_FAIL=0
NUM_ERROR=0

if [[ -z "$QEMU_CMD" ]]; then
  echo '$QEMU_CMD is not set'
  exit 1
fi

for t in $TEST_DIR/*.dat; do

  base=$(basename "${t%.dat}")
  test_number=${base:0:3}
  test_name=${base:4}
  test_desc="Test #$test_number: ${test_name}... "
  nb_lines=$(wc -l < "$t")
  if [[ $nb_lines -eq 0 ]]; then
    nb_lines=1
  fi

  # Arguments can be provided to run only a specified range of tests
  if [[ 10#$test_number -lt 10#$MIN_TEST ]]; then
    continue
  fi
  if [[ 10#$test_number -gt 10#$MAX_TEST ]]; then
    break
  fi

  # Run pre test script if required
  if [[ -x "$TEST_DIR/$test_number setup.sh" ]]; then
    . "$TEST_DIR/$test_number setup.sh"  > /dev/null 2>&1
  fi

  echo -n "$test_desc"
  num_blanks=$((LINE_LEN - ${#test_desc}))
  if [[ $num_blanks -gt 0 ]]; then
    padding="$(printf '%*s' $num_blanks)"
    echo -n "$padding"
  fi
  # Run qemu with a timeout
  rm -f output.txt error.txt
  timeout --foreground $TIMEOUT bash -c "${QEMU_CMD} 1>output.txt 2>error.txt"

  err=$?
  if [[ $err -eq 124 ]]; then
    echo "[ERROR] (Time out)"
    NUM_ERROR=$((NUM_ERROR + 1))
  elif [[ $err -ne 0 ]]; then
    echo "[ERROR] ($err)"
    cat error.txt
    NUM_ERROR=$((NUM_ERROR + 1))
  else
    tail -n $nb_lines output.txt | diff -Z --strip-trailing-cr -q "$t" - >/dev/null 2>&1
    if [[ $? -eq 0 ]]; then
      echo "[PASS]"
      NUM_PASS=$((NUM_PASS + 1))
    else
      echo "[FAIL]"
      NUM_FAIL=$((NUM_FAIL + 1))
      echo "Output of failed test was:"
      echo "-------------------------------------------------------------------------------"
      tail -n +3 output.txt
      echo "-------------------------------------------------------------------------------"
    fi
  fi

  # Run post test script if required
  if [[ -x "$TEST_DIR/$test_number teardown.sh" ]]; then
    . "$TEST_DIR/$test_number teardown.sh"  > /dev/null 2>&1
  fi

done

echo
echo "==============================================================================="
echo "# Test summary for gnu-efi"
echo "==============================================================================="
echo "# TOTAL: $((NUM_PASS + NUM_FAIL + NUM_ERROR))"
echo "# PASS:  $NUM_PASS"
echo "# FAIL:  $NUM_FAIL"
echo "# ERROR: $NUM_ERROR"
echo "==============================================================================="

if [[ $NUM_FAIL -ne 0 || $NUM_ERROR -ne 0 ]]; then
  exit 1
fi
