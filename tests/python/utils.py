import sys
import subprocess
import test_env as env


def run_shell(cmd, cwd):
    proc = subprocess.Popen(cmd,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT,
                            cwd=cwd)
    buf = []

    while True:
        line = proc.stdout.readline()
        buf.append(line.decode('utf-8'))

        if not line and proc.poll() is not None:
            break

    return ''.join(buf).strip()


def run_test(name, cmd, expect):
    res = run_shell(cmd, env.BUILD_DIR)
    assert res == expect, \
        f'[{name}] failed...'
