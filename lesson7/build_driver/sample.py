ETH_P_ALL = 0x0003
import socket
import sys

if __name__ == "__main__":
    interface = "fake_net_driver"
    filename = "f.txt"

    with open(filename, "r") as file:
        hexlist = [int(x, 16) for x in file.read().split()]

    packet = bytearray(hexlist)

    with socket.socket(socket.AF_PACKET, socket.SOCK_RAW) as rs:
        rs.bind((interface, ETH_P_ALL))
        sentbytes = rs.send(packet)

    print("Sent packet of length %d bytes" % sentbytes)
