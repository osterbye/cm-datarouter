#!/usr/bin/python3

import MessageDefinitions_pb2

type_mapping = {
  'TYPE_BOOL':       'INT',
  'TYPE_BYTES':      'INT',
  'TYPE_DOUBLE':     'FLOAT',
  'TYPE_ENUM':       'INT',
  'TYPE_FIXED32':    'INT',
  'TYPE_FIXED64':    'INT',
  'TYPE_FLOAT':      'FLOAT',
  'TYPE_GROUP':      'BLOB',
  'TYPE_INT32':      'INT',
  'TYPE_INT64':      'INT',
  'TYPE_MESSAGE':    'BLOB',
  'TYPE_SFIXED32':   'INT',
  'TYPE_SFIXED64':   'INT',
  'TYPE_SINT32':     'INT',
  'TYPE_SINT64':     'INT',
  'TYPE_STRING':     'TEXT',
  'TYPE_UINT32':     'INT',
  'TYPE_UINT64':     'INT',
}

type_conversion = {}

statusUpdate = MessageDefinitions_pb2.StatusUpdate()

# build type conversion table for mapping PB type numbers to SQL type strings
for key, value in type_mapping.items():
  num = getattr(statusUpdate.DESCRIPTOR.fields[0], key)
  type_conversion[num] = value

with open('status_updates.schema', 'w') as file:
  file.write('CREATE TABLE measurements (\n')

  for field in statusUpdate.DESCRIPTOR.fields:
    line = '\t{0:<25}{1},\n'.format(field.name, type_conversion[field.type])
    file.write(line)

  file.write('\t{0:<25}{1}\n'.format('pbMessageDump', 'BLOB'))
  file.write(');\n')
