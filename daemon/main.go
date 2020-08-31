package main

import (
	"log"
	"net"
	"os"
	"os/signal"
	"strings"
	"syscall"
)

func runServer(c net.Conn) {
	log.Printf("Client connected [%s]", c.RemoteAddr().Network())
	for {
		buf := make([]byte, 512)
		nr, err := c.Read(buf)
		if err != nil {
			return
		}

		// parse data as a string without trailing data
		data := buf[0:nr]
		parsedData := strings.SplitN(strings.TrimSpace(string(data)), " ", 1)

		// interpret command
		switch parsedData[0] {
		case "ping":
			sendResponse(c, "pong")
		}

	}
}

// send val\n as a response
func sendResponse(c net.Conn, val string) {
	var err error

	_, err = c.Write([]byte(val + "\n"))
	if err != nil {
		log.Fatal("Writing client error: ", err)
	}
}

/** SYNTAX: oblid <socket dir> **/
func main() {
	SockAddr := os.Args[1] + "/oblid.sock"
	log.Print("Starting obli daemon at ", SockAddr)

	if err := os.RemoveAll(SockAddr); err != nil {
		log.Fatal(err)
	}

	listener, err := net.Listen("unix", SockAddr)
	if err != nil {
		log.Fatal("error opening socket:", err)
	}
	defer listener.Close()

	sigc := make(chan os.Signal, 1)
	signal.Notify(sigc, os.Interrupt, syscall.SIGKILL)
	go func(ln net.Listener, c chan os.Signal) {
		sig := <-c
		log.Printf("Caught signal %s: shutting down.", sig)
		ln.Close()
		os.Exit(0)
	}(listener, sigc)

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
