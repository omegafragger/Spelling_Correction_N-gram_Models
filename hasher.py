import sys
import os
import socket

class Server:
    def __init__(self):
        self.size = 1024
        self.sock = None
        self.port = 5001
        self.host = ""

    def loadQuadragram(self, filename):
        """A function to load the four-gram.txt file to a hashmap"""
        fp = open(filename, "r+")
        fp2 = open("testq", "w+")
        res = {} #This little shit IS the MAP !!! -_-
        i = 1
        odd = 0
        print "Reading quadragram file.... Please wait...."
        for l in fp:
            if i == 1:
                i += 1
                continue
            l = l[:-1]
            l = l.strip()
            l = l.lower()
            parts = l.split('\t')
            if len(parts) < 2:
                #print "Odd format encountered at iteration: " + str(i)
                i += 1
                odd += 1
                continue
            #x = parts[0][0:9]
            try:
                val = float(parts[0])
            except ValueError:
                #print "Odd format encountered at iteration: "  + str(i)
                i += 1
                odd += 1
                continue
            key = parts[1]
            strp = parts[1].split(' ')
            if (strp[0] == 'a' and strp[1][0] == 'p') :
                fp2.write(str(strp) + "\n")
            res[key] = val
            i += 1
        print "Quadragram File read successfully ..."
        fp.close()
        fp2.close()
        return res #Returning the MAP !! -_-
	
    def loadTrigram(self, filename):
        """A function to load the trigram tuples into a hashmap"""
        fp = open(filename, "r+")
        res = {} #This little shit IS the MAP !!! -_-
        i = 1
        odd = 0
        print "Reading trigram file.... Please wait...."
        for l in fp:
            if i == 1:
                i += 1
                continue
            l = l[:-1]
            l = l.strip()
            l = l.lower()
            parts = l.split('\t')
            if len(parts) < 2:
                #print "Odd format encountered at iteration: " + str(i)
                i += 1
                odd += 1
                continue
            #x = parts[0][0:9]
            try:
                val = float(parts[0])
            except ValueError:
                #print "Odd format encountered at iteration: "  + str(i)
                i += 1
                odd += 1
                continue
            key = parts[1]
            res[key] = val
            i += 1
        print "Trigram File read successfully ..."
        fp.close()
        return res #Returning the MAP !! -_-

    def loadBigram(self, filename):
        """A function to load the bigram tuples into a hashmap"""
        fp = open(filename, "r+")
        res = {} #This little shit IS the MAP !!! -_-
        i = 1
        odd = 0
        print "Reading bigram file.... Please wait...."
        for l in fp:
            if i == 1:
                i += 1
                continue
            l = l[:-1]
            l = l.strip()
            l = l.lower()
            parts = l.split(' ')
            if len(parts) < 2:
                print "Odd format split error encountered at iteration: " + str(i)
                i += 1
                odd += 1
                continue
            #x = parts[0][0:9]
            try:
                val = float(parts[0])
            except ValueError:
                print "Odd format floating point not encountered encountered at iteration: "  + str(i)
                i += 1
                odd += 1
                continue
            key = ""
            for x in parts[1:]:
                key = key + x + " "
            key = key.strip()
            res[key] = val
            #print "key: " + str(key) + " val: " + str(val)
            i += 1
        print "Bigram File read successfully ..."
        fp.close()
        return res #Returning the MAP !! -_-
        
    def openSocket(self, map_bigram, map_trigram, map_quadragram):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.host, self.port))
        self.sock.listen(5)

        client_sock, addr = self.sock.accept()
        print "Client has connected..."
        mode = 1 #Mode=1 for Bigram, 2 for Trigram, and 3 for Quadragram
        while True:
            strn = client_sock.recv(self.size)
            if strn == "":
                continue
            if strn == "mode bigram":
                print "Bigram mode activated..."
                mode = 1
                continue
            elif strn == "mode trigram":
                print "Trigram mode activated..."
                mode = 2
                continue
            elif strn == "mode quadragram":
                print "Quadragram mode activated..."
                mode = 3
                continue
                
            print "Requested search string: " + strn
            strn.strip()
            try:
                if mode == 1:
                    val = map_bigram[strn]
                elif mode == 2:
                    val = map_trigram[strn]
                elif mode == 3:
                    val = map_quadragram[strn]
            except KeyError:
                val = 0
            val = str(val)
            print "Requested probability: " + val
            client_sock.send(val)
        self_sock.close()
   
    
if __name__ == '__main__':
    s = Server()
    quad_map = s.loadQuadragram("four-gram.txt")
    tri_map = s.loadTrigram("out.txt")
    bi_map = s.loadBigram("Bigram.txt")
    print "Server ready for client requests..."
    s.openSocket(bi_map, tri_map, quad_map)
