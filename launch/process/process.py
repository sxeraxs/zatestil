from threading import Thread
from typing import Any, List

from .executor import ProcessExecutor

class Process(object):
    __name: str = ""
    __args: List[Any] = []
    __thread: Thread = None
    __executor: ProcessExecutor

    def __init__(self, name: str, args: List[Any]):
        self.__name = name
        self.__args = args
        self.__executor = ProcessExecutor()

    def append_arg(self, arg: Any) -> None:
        """
        Append new arg for process
        """
        self.__args.append(arg)

    def get_process_name(self) -> str:
        """
        Return current process name
        """
        return self.__name

    def get_process_args(self) -> List[Any]:
        """
        Return all args
        """
        return self.__args

    def start(self) -> None:
        """
        Start new thread, and create process
        """
        self.__thread = Thread(target=self.__executor.start, args=(self.__name, self.__args))
        self.__thread.start()
        print("-" * 80 + f"\nStarting process {self.__name} {self.__args} in thread {self.__thread}\n" + "-" * 80 + "\n")

    def stop(self) -> None:
        """
        Terminate process
        Stop thread
        """
        print("-" * 80 + f"\nStopping process {self.__name} {self.__args}\n" + "-" * 80 + "\n")
        self.__executor.stop()

        self.__thread.join()
