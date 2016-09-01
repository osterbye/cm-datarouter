#!/usr/bin/python3

import MessageDefinitions_pb2
import struct

filename = 'sample_pb.bin'

FRAME_PREAMBLE = b'\x50\x41'
FRAME_TYPE = b'\x01'

# generate 10 messages with statusUpdate data
frames = []
for i in range(10):
  container = MessageDefinitions_pb2.ContainerMessage()
  container.statusUpdate.timestamp = 600 + i
  container.statusUpdate.speedDisplayed = 42 - i
  container.statusUpdate.voltageLV = 13 + i / 10

  message_serialized = container.SerializeToString()

  print(''.join('\\x%02X' % n for n in message_serialized))
  frame_crc_serialized = b'\xFF\xFF' # TODO
  frame_length = 2 + len(message_serialized) + len(frame_crc_serialized) + len(FRAME_PREAMBLE) + len(FRAME_TYPE)
  frame_length_serialized = struct.pack('<H', frame_length)
  frame_serialized = b''.join([FRAME_PREAMBLE, FRAME_TYPE, frame_length_serialized, message_serialized, frame_crc_serialized])
  frames.append(frame_serialized)

# write generated messages as binary file
with open(filename, 'wb') as fd:
  fd.write(b''.join(frames))
  print('constructed messages written to', filename)

# Example of going other way:
#
# statusUpdate = MessageDefinitions_pb2.StatusUpdateUpdate()
# statusUpdate.ParseFromString(b'\xA0\x06\x9A\x05\x4D\x00\x00\x28\x42')
# print(statusUpdate.timestamp)