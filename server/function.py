from dataclasses import dataclass
import socket
import struct

@dataclass
class Function:
    def ask_activity(self) -> chr:
        print('If you want to get user activity enter 1.\nIf you want to get screen enter 2.\n')
        return input()

    def __receive_file_size(self, sck: socket.socket) -> int:
        fmt = '<Q'
        expected_bytes = struct.calcsize(fmt)
        received_bytes = 0
        stream = bytes()
        while received_bytes < expected_bytes:
            chunk = sck.recv(expected_bytes - received_bytes)
            stream += chunk
            received_bytes += len(chunk)
        filesize = struct.unpack(fmt, stream)[0]

        return filesize

    def receive_file(self, sck: socket.socket, filename):
        filesize = self.__receive_file_size(sck)

        with open(filename, 'wb') as f:
            received_bytes = 0

            while received_bytes < filesize:
                chunk = sck.recv(1024)
                if chunk:
                    f.write(chunk)
                    received_bytes += len(chunk)