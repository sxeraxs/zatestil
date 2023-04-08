#!/usr/bin/env python3

import os
import time
import shutil

from argparse import ArgumentParser
from process import ProcessController, Process

if __name__ == '__main__':
    os.system("clear")

    parser = ArgumentParser()
    parser.add_argument("--build", type=str, default="debug", help="Build type (debug/release)", required=False)

    args, additional_args = parser.parse_known_args()

    print("=" * 80 + "\nStarting system\n" + "=" * 80 + "\n")

    build_type = args.build
    root = os.path.abspath("..")
    binary_path = f"{root}/cmake-build-{build_type}/bin"
    zatestil = f"{binary_path}/zatestil"

    bot = Process(f"{zatestil}.bot", [])
    service = Process(f"{zatestil}.service", [])

    process_controller = ProcessController()
    process_controller.start([
        service,
        bot
    ])

    print("=" * 80 + "\nSystem is started\n" + "=" * 80 + "\n")

    try:
        while True:
            time.sleep(1)

    except KeyboardInterrupt:
        print("\n" + "=" * 80 + "\nStopping system\n" + "=" * 80 + "\n")

        process_controller.stop()

    print("=" * 80 + "\nSystem is stopped\n" + "=" * 80 + "\n")
