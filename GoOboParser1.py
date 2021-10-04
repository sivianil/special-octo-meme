#!/usr/bin/python3
import sys
import os
import re
import gzip

def main(args):
    if len(sys.argv) <= 3:
        help()
        
    if not os.path.isfile(sys.argv[1]):
        print(f''' The file {sys.argv[1]} does not exist ''')
        sys.exit(0)
    
    if not re.search('.gz$', sys.argv[1]):
        print("This is not a gz file")
        sys.exit(0)
    
    if not sys.argv[2] in ["getEntry", "getParent", "getXref"]:
        print("The command is not valid")
        sys.exit(0)
        
    
    if (sys.argv[2] in ["getEntry", "getParent"]):
        if not re.search('^GO:[0-9]{7}', sys.argv[3]):
            print("This is not a valid GO Id")
            sys.exit(0)
    else:
        if not re.search('^[a-zA-Z0-9_]+', sys.argv[3]):
            print("This is not a valid GO ID")
            sys.exit(0)
            
    gob = GoObo(sys.argv[1])
    
    if sys.argv[2] == "getEntry":
        res = gob.getEntry(sys.argv[3])
        print(res)        
    elif sys.argv[2] == "getParent":
        res = gob.getParent(sys.argv[3])
        print(res)    
    else:
        res = gob.getXref(sys.argv[3])
        print(res)
        

class GoObo():
    """ class initialiser """
    
    def __init__ (self,filename):
        """ Class initialiser """
        self.filename = filename  
              
    def getEntry(self, GO_ID = ''):
        file = gzip.open(self.filename, 'rt')
        found = False
        go_id = re.compile('^id: '+GO_ID)
        res = ''
        for line in file:
            if re.search('^id: (GO:[0-9]{7})', line):
                if re.search(go_id, line):
                    found = True
                elif found:
                    break
            if found:
                res += line
        file.close()
        return(res)
        
    def getParent(self, GO_ID = ''):
        file = gzip.open(self.filename, 'rt')
        found = False
        go_id = re.compile('^id: '+GO_ID)
        res = ''
        for line in file:
            if re.search('^id: (GO:[0-9]{7})', line):
                if re.search(go_id, line):
                    found = True
                elif found:
                    break
            if found:
                if re.search('is_a: (GO:[0-9]{7})', line):
                    res = re.sub('.+(GO:[0-9]{7}).*\n$',"\\1",line)
        file.close()
        return(res)  
                                                       
    def getXref(self, Xref_ID = ''):
        file = gzip.open(self.filename, 'rt')
        #found = False
        #xref_id  = re.compile('xref: '+Xref_ID)
        res = ''
        for line in file:
            if re.search('^id: (GO:[0-9]{7})', line):
                id = re.sub('.+(GO:[0-9]{7}).*\n$',"\\1",line)
            if re.search('^xref: '+Xref_ID, line):
                res = id
        file.close()
        return(res)           
            
def help():
    print("GoUtils tool by Anil, 2021")
    print("Usage: app.py obofilename getEntry|getParent|getXref Go_ID")
    
            
                
if __name__ == "__main__":
    main(sys.argv)
        
