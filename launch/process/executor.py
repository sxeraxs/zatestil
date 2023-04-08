from subprocess import Popen
from typing import List, Any

class ProcessExecutor(object):
    """
    Process executor
    """

    __process: Popen = None
    __launch: bool = False

    def __init__(self):
        self.__launch = False

    def start(self, name: str, args: List[Any]) -> None:
        """
        Create new process on system
        """
        arguments: str = " ".join(str(arg) for arg in args)
        command: str = f"{name} {arguments}"

        self.__launch = True
        self.__process = Popen(command, shell=True)

        while True:
            com = self.__process.communicate()
            rc = self.__process.returncode
            if (not self.__launch) or rc:
                self.__process.terminate()
                self.__process.wait()
                return

    def stop(self) -> None:
        """
        Stop running process
        """
        self.__launch = False
