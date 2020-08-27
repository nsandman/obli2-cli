package main

import (
	"io"
	"log"
	"net"
	"os"
)

func runServer(c net.Conn) {
	log.Printf("Client connected [%s]", c.RemoteAddr().Network())
	io.Copy(c, c)
	c.Close()
}

/** SYNTAX: oblid <socket dir> **/
func main() {
	SockAddr := os.Args[1] + "/oblid.sock"

	if err := os.RemoveAll(SockAddr); err != nil {
		log.Fatal(err)
	}

	listener, err := net.Listen("unix", SockAddr)
	if err != nil {
		log.Fatal("error opening socket:", err)
	}
	defer listener.Close()

	for {
		// Accept new connections, dispatching them to runServer
		// in a goroutine.
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal("accept error:", err)
		}

		go runServer(conn)
	}
}
