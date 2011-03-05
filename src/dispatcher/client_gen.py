#
# Copyright 2011 Matija Osrecki
#

import sys
import re


class TType:
    
    def __init__(self, type, add=None):
        if type == 'list':
            self.type = 'list'
            self.ltype = add
        else:
            self.type = type

    def __str__(self):
        if self.type == 'list':
            return 'list<' + str(self.ltype) + '>'
        else:
            return self.type

    # parses and returns thrift type. no space allowed
    @staticmethod
    def parse(raw):

        if raw.find('list') != -1:
            lt = re.findall(r'list<([<>0-9a-zA-Z_]+)>', raw)
            return TType('list', TType.parse(lt[0]))

        return TType(raw)


class Function:
    
    def  __init__(self, type=None, name=""):
        self.type = type
        self.name = name
        self.params = []


class Service:
    
    def __init__(self, name=""):
        self.name = name
        self.functions = []        


def read_services(data):
    raw_servs = re.findall(r'service\s(\w+)\s{([^}]*)}', data, re.DOTALL)
    
    for rs in raw_servs:
        s = Service(rs[0])

        raw_funs = re.findall(r'([a-zA-Z0-9_.<> ]+)\s(\w+)\(([^\)]*)\)', 
                              rs[1], re.DOTALL)
        
        for rw in raw_funs:
            f = Function(TType.parse(rw[0].translate(None, ' ')), rw[1])
            
            print f.type, f.name
            raw_params = [p.strip().translate(None, '\n\r') 
                          for p in rw[2].split(',')]

            for rw in raw_params:
                if rw == '':
                    break

                rw = rw.split(':')[1].split(' ')
                param = (TType.parse(''.join(rw[:-1])), rw[-1])

                f.params.append(param)


def main():
    lang = sys.argv[1]
    in_file = open(sys.argv[2], 'r')
    out_file = open(sys.argv[3], 'w')

    data = "".join(in_file.readlines())
    
    services = read_services(data)



if __name__ == '__main__':
    main()
