from dataclasses import dataclass
import json
from function import Function
import socket
import os

@dataclass
class Server:
    def __install_settings(self) -> list:
        with open('settings.json', 'r', encoding='utf-8') as file:
            return json.load(file)

    def connect(self):
        list_connections = self.__install_settings()
        for connect in list_connections:
            dict_conn = {key: value for (key, value) in connect.items()}

            with socket.create_server((dict_conn.get('ip_address'), dict_conn.get('port'))) as server:
                server.listen(dict_conn.get('listen'))
                while True:
                    conn, address = server.accept()
                    print(f'{address[0]}:{address[1]} is connected.')

                    function = Function()
                    data_user_input = function.ask_activity()

                    if '1' == data_user_input:
                        conn.send(b'1')
                        data = conn.recv(1024)
                        print(data)

                    elif '2' == data_user_input:
                        conn.send(b'2')
                        function.receive_file(conn, f'screen{address[0]}.png')
                        print('Get file.')


