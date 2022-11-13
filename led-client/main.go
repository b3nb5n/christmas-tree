package main

import (
	"log"
	"net"
)

const LEDS_COUNT = 50;
var leds [LEDS_COUNT]byte

func main() {
	tcpAddr, err := net.ResolveTCPAddr("tcp", "192.168.1.32:80")
	if err != nil {
		log.Fatalln("Error resolving tcp address: ", err)
	}

	conn, err := net.DialTCP("tcp", nil, tcpAddr)
	if err != nil {
		log.Fatalln("Error dialing tcp address: ", err)
	}

	defer conn.Close()

	_, err = conn.Write(leds[:])
	if err != nil {
		log.Fatalln("Error writing to tcp connection: ", err)
	}
}