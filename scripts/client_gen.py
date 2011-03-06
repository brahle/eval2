#!/usr/bin/env python
#
# Copyright 2011 Matija Osrecki
#
# This file is part of Evaluator.
# 
# Evaluator is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# Evaluator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public
# License along with Evaluator. If not, see
# <http://www.gnu.org/licenses/>.

######################################################################
# In short, the script takes one two arguments: language (cpp for now) and
# thrift input file. It then tries to find all the services and their functions
# and for each service it creates a %service_name%_client.{h} file with
# all the code. Watch out cause it will rewrite a file if you have such.

# Example: python client_gen.py cpp services.thrift
# 
# For now, it generates only one service - the first that is found.
# Any commented functions will also be generated.
#
# Interactive mode will be implemented soon.
# 
# Bugs are probable.

import sys
import re
import textwrap

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
            lt = re.findall(r'list<([<>0-9a-zA-Z_\.]+)>', raw)
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


# parses thrift file

def read_services(data):
    raw_servs = re.findall(r'service\s(\w+)\s{([^}]*)}', data, re.DOTALL)
    
    services = []
    
    for rs in raw_servs:
        s = Service(rs[0])

        raw_funs = re.findall(r'([a-zA-Z0-9_.<> ]+)\s(\w+)\(([^\)]*)\)', 
                              rs[1], re.DOTALL)
        
        for rw in raw_funs:
            f = Function(TType.parse(rw[0].translate(None, ' ')), rw[1])

            raw_params = [p.strip().translate(None, '\n\r') 
                          for p in rw[2].split(',')]

            for rw in raw_params:
                if rw == '':
                    break

                rw = rw.split(':')[1].split(' ')
                param = (TType.parse(''.join(rw[:-1])), rw[-1])
                
                f.params.append(param)

            s.functions.append(f)

        services.append(s)

    return services

# **************** GENERATORS ****************

def is_struct(ttype):
    return ttype.type == 'list' or ttype.type == 'string'

def thrift_to_cpp_type(ttype):
    if ttype.type == 'i16':
        return 'short int'
    elif ttype.type == 'i32':
        return 'int'
    elif ttype.type == 'i64':
        return 'long long'
    elif ttype.type == 'byte':
        return 'char'
    elif ttype.type == 'boolean':
        return 'bool'
    elif ttype.type == 'double':
        return 'double'
    elif ttype.type == 'string':
        return 'std::string'
    elif ttype.type == 'list':
        return 'std::vector< %s >' % thrift_to_cpp_type(ttype.ltype)

    return ttype.type



# cpp generator
def generate_cpp(service):
    filename = re.sub(r'\b([A-Z])', r'_\1', service.name.lower()) + '_client'

    tw = textwrap.TextWrapper()
    tw.width = 80
    tw.subsequent_indent = '    '
    tw.break_long_words = False

    out = open(filename + '.h', 'w')
    
    guard = 'EVAL_%s_H_' % filename.upper()

    includes = ['string', 'protocol/TBinaryProtocol.h', 'transport/TSocket.h',
                'transport/TBufferTransports.h', 'transport/TTransportUtils.h',
                'boost/shared_ptr.hpp']
    
    includes_local = ['gen-cpp/%s.h' % service.name]

    namespace = ['eval']

    att = 'apache::thrift::transport'
    atp = 'apache::thrift::protocol'
    
    transport = '%s::TTransport' % att
    socket = '%s::TSocket' % att
    btran = '%s::TBufferedTransport' % att
    protocol = '%s::TProtocol' % atp
    bprot = '%s::TBinaryProtocol' % atp
    
    classname = '%sClient' % service.name
    
    lines = ['class %s {' % classname,
        ' public:\n', 
        '  %s(const std::string &ip, const int port) {' % classname,
        '    boost::shared_ptr<%s> socket(\n      new %s(ip, port));\n' % 
             (transport, socket),
        '    boost::shared_ptr<%s> transport(\n      new %s(socket));\n' % 
             (transport, btran),
        '    boost::shared_ptr<%s> protocol(\n      new %s(transport_));\n' % 
             (protocol, bprot),
        '    boost::shared_ptr< ::%s> client(\n      new ::%s(protocol));' %
             (classname, classname),
        '',
        '    client_ = client;',
        '    transport_ = transport;',
        '    transport_->open();',
        '  }\n',
        '  ~%s() {' % classname,
        '    try {',
        '      transport_->close();',
        '    } catch(const apache::thrift::TException &tx) {', 
        '      // TODO(someone): log this', 
        '    }',
        '  }\n',
        ]

    for f in service.functions:
        return_type = thrift_to_cpp_type(f.type)

        args = []
        
        if is_struct(f.type):
            args.append(('%s &' % (return_type), 'return_'))
            return_type = 'void'
        
        for p in f.params:
            cctype = 'const ' + thrift_to_cpp_type(p[0])

            if is_struct(p[0]):
                cctype = cctype + '&'
                
            args.append((cctype, p[1]))

        fun = '  %s %s(' % (return_type, f.name)
        fun += ', '.join([a[0] + ' ' + a[1] for a in args]) + ') {'

        fun = tw.fill(fun) + '\n'
        
        if return_type != 'void':
            fun += '    return ';
        else:
            fun += '    ';

        fun += 'client_->%s(%s);\n  }\n' %\
            (f.name,', '.join([a[1] for a in args]))
        
        lines.append(fun)

    lines += [
        ' private:',
        '  boost::shared_ptr< ::%s> client_;' % (classname),
        '  boost::shared_ptr<%s> transport_;' % (transport),
        '\n};'
        ]

    out.write('#ifndef %s\n' % guard)
    out.write('#define %s\n' % guard)
    out.write('\n')
    
    out.write('\n'.join(['#include <%s>' % i for i in includes]))
    out.write('\n\n')

    out.write('\n'.join(['#include "%s"' % i for i in includes_local]))
    out.write('\n\n')

    out.write(''.join(['namespace %s { ' % n for n in namespace]))
    out.write('\n\n')
 
    out.write('\n'.join(lines) + '\n\n')
    out.write('} ' * len(namespace) + ' // namespace\n\n');
    out.write('#endif  // %s' % guard + '\n');


def main():
    lang = sys.argv[1]
    in_file = open(sys.argv[2], 'r')

    data = "".join(in_file.readlines())
    
    services = read_services(data)
    
    if len(services) == 0: 
        exit(0)

    if lang == 'cpp':
        generate_cpp(services[0])
    elif lang == 'python':
        generate_python(services[0])
    else:
        print 'Unsupported language %s. Try cpp or python.' % lang
        exit(1)


if __name__ == '__main__':
    main()
