package main

import (
	sshrw "github.com/mosolovsa/go_cat_sshfilerw"
    "fmt"
	"log"
	"bytes"
	"bufio"
	"strings"
	"golang.org/x/crypto/ssh"
	"io/ioutil"
    "time"
)

func main() {
	cfg := ssh.ClientConfig{
		User: "rusal",
		Auth: []ssh.AuthMethod{
			ssh.Password("rusal_11"),
		},
        HostKeyCallback: ssh.InsecureIgnoreHostKey(),
	}
	c, err := sshrw.NewSSHclt("10.1.96.81:22", &cfg)
	if err != nil {
		log.Panicln("Can't start ssh connection, err:", err.Error())
	}
	client, err := ssh.Dial("tcp", "10.1.96.81:22", &cfg);
    for true{
	    session, err := client.NewSession()
	    if err != nil {
	        log.Fatalln("Failed to create session: " + err.Error())
	    }
	    var buff bytes.Buffer
	    w := bufio.NewWriter(&buff)
	    directories, err := session.Output("ls /var/www/html/datas/")
		var fixedDirectories []string
		var tmp []byte
		for _, letter := range(directories){
			if letter == '\n'{
				//fmt.Println("dumping " + string(tmp[1:]))
				fixedDirectories = append(fixedDirectories, string(tmp))
				tmp = nil
			}
			tmp = append(tmp, byte(letter))
		}
	    for _, file := range(fixedDirectories){
	        if err = c.ReadFile(w, "/var/www/html/datas/" + strings.Trim(file, "\n")); err != nil {
				fmt.Println("/var/www/html/datas/" + strings.Trim(file, "\n"));
	            log.Println("Error on file read: ", err.Error())
	        }else{
				w.Flush()
				fmt.Println(buff.String())
				err = ioutil.WriteFile("./datas/" + strings.Trim(file, "\n"), buff.Bytes(), 0644)
				buff.Reset()
			}
	    }
	    time.Sleep(1000 * time.Millisecond)
	}
}
