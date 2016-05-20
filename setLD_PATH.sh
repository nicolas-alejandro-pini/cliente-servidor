export TP=/home/utnso/workspace/client-server

export CLIENT_TCP=$TP/clientTCP/Debug
export SERVER_TCP=$TP/serverTCP/Debug
export SOCKET_TCP=$TP/socket_tcp/Debug

export LD_LIBRARY_PATH=$CLIENT_TCP${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=$SERVER_TCP${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=$SOCKET_TCP${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}

echo $LD_LIBRARY_PATH
