#!/usr/bin/env python3

import argparse
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

VALGRIND_ERROR_EXIT_CODE = 99
PROGRAM = Path(__file__).parent / "ex17p"
PROGRAM = PROGRAM.resolve()
VERBOSE = False


class Ex17pTest(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp_dir.cleanup)
        self.db = Path(self.temp_dir.name) / "test.db"

    def run_ex17p(self, *args, expected_exit=0):
        command = [
            "valgrind",
            "--leak-check=full",
            "--show-leak-kinds=all",
            "--errors-for-leak-kinds=all",
            "--track-origins=yes",
            f"--error-exitcode={VALGRIND_ERROR_EXIT_CODE}",
            str(PROGRAM),
            str(self.db),
            *map(str, args),
        ]
        result = subprocess.run(command, text=True, capture_output=True, check=False)

        details = (
            f"command: {' '.join(command)}\n"
            f"exit code: {result.returncode}\n"
            f"stdout:\n{result.stdout}\n"
            f"stderr:\n{result.stderr}"
        )
        if VERBOSE:
            print(f"\n{details}\n", file=sys.stderr)

        self.assertNotEqual(
            result.returncode,
            VALGRIND_ERROR_EXIT_CODE,
            "Valgrind detected an error:\n" + details,
        )
        self.assertEqual(result.returncode, expected_exit, details)
        self.assertIn("ERROR SUMMARY: 0 errors", result.stderr, details)
        self.assertRegex(result.stderr, r"in use at exit: 0 bytes in 0 blocks")
        return result

    def create_database(self, rows=10):
        self.run_ex17p("c", rows)

    def test_database_lifecycle(self):
        self.create_database()

        result = self.run_ex17p("l")
        self.assertEqual(result.stdout, "")

        self.run_ex17p("s", 2, "Alice Zhang", "alice@example.com")
        self.run_ex17p("s", 7, "Bob", "bob@example.com")

        result = self.run_ex17p("g", 2)
        self.assertEqual(
            result.stdout,
            "id=2, name=Alice Zhang email=alice@example.com\n",
        )

        result = self.run_ex17p("l")
        self.assertEqual(
            result.stdout,
            "id=2, name=Alice Zhang email=alice@example.com\n"
            "id=7, name=Bob email=bob@example.com\n",
        )

        self.run_ex17p("d", 2)
        result = self.run_ex17p("l")
        self.assertEqual(
            result.stdout,
            "id=7, name=Bob email=bob@example.com\n",
        )

        self.run_ex17p("s", 2, "Carol", "carol@example.com")
        result = self.run_ex17p("g", 2)
        self.assertEqual(
            result.stdout,
            "id=2, name=Carol email=carol@example.com\n",
        )

    def test_rejected_operations_are_memory_safe(self):
        self.create_database(3)

        result = self.run_ex17p("g", 1, expected_exit=1)
        self.assertIn("Database_get: ID is not set", result.stderr)

        self.run_ex17p("s", 1, "Alice", "alice@example.com")
        result = self.run_ex17p(
            "s", 1, "Replacement", "replacement@example.com", expected_exit=1
        )
        self.assertIn("Database_set: Already set", result.stderr)

        for invalid_id in (-1, 3):
            with self.subTest(invalid_id=invalid_id):
                result = self.run_ex17p("g", invalid_id, expected_exit=1)
                self.assertIn("main: There's no such record", result.stderr)


def main():
    global VERBOSE

    parser = argparse.ArgumentParser(
        description="Run ex17p integration tests entirely under Valgrind."
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="print every command and its complete output",
    )
    args = parser.parse_args()
    VERBOSE = args.verbose

    if shutil.which("valgrind") is None:
        parser.error("valgrind was not found in PATH")
    if not PROGRAM.is_file():
        parser.error(f"program does not exist: {PROGRAM}")

    unittest.main(argv=[Path(__file__).name], verbosity=2)


if __name__ == "__main__":
    main()
