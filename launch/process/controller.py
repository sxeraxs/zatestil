from typing import List

from .process import Process


class ProcessController(object):
    """
    Centralized process management
    """
    __process_list: List[Process] = []

    def append_process(self, process: Process) -> None:
        """
        Append new process on manager
        """
        self.__process_list.append(process)

    def start(self, processes: List[Process] = None) -> None:
        """
        Start processes
        """
        if processes is not None:
            for process in processes:
                self.__process_list.append(process)

        for process in self.__process_list:
            process.start()

    def stop(self) -> None:
        """
        Stop processes
        """
        for process in self.__process_list:
            process.stop()
