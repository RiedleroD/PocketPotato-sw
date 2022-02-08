#!/usr/bin/env python3
#
# Basically the same algorithm as this one here: https://youtu.be/aF1Yw_wu2cM
# with blocks of 8 bits instead of just 2 because I am lazy.
# and also some other minor deviations
#
# TODO: delta variants (heh)
# TODO: block variants
from sys import argv, exit
from math import log2,floor
from random import randrange
import pyjson5

with open("./textures.json") as f:
	data = f.read()
textures = pyjson5.decode(data)
del data

def decompress(tx:list) -> str:
	return delta_decode(RLEdecode(tx))

def compress(tx:list) -> str:
	return RLEencode(get_numlist_from_binstring(delta_encode(tx)))

"""decodes binary string to binary string per RLE"""
def RLEdecode(tx:str) -> str:
	decoded=[]
	i=0
	curRLE=False
	l=len(tx)
	while i<l:
		curByte=tx[i:i+8]
		i+=8
		if curByte=="0"*8:
			if i>=l:#safety measure in case we end on an 0x00
				break
			packet_length=get_RLE_packet_length(tx[i:])
			decoded.append("0"*8*decode_RLE_packet(tx[i:i+packet_length],packet_length))
			i+=packet_length
		elif len(curByte)<8:
			break #last non-RLE packet can be complete, but is discarded then
		else:#non-RLE packets are just raw data. No decoding here.
			decoded.append(curByte)
	return "".join(decoded)

"""encodes list of hexadecimal values to binary string per RLE"""
def RLEencode(tx:list) -> str:
	curRLE=False
	encoded=0b00
	i=0 # index in encoded texture
	j=0 # length of current RLE packet
	for hx in tx: # going over the texture in hexadecimal blocks
		if hx==0:
			if not curRLE:
				i+=8#end non-RLE packet with a wall of zeroes
				encoded<<=8
				curRLE=True
			j+=1
		else:
			if curRLE:
				encpack=encode_RLE_packet(j)
				encpacklen=len(encpack)
				encoded=(encoded<<encpacklen) | int(encpack,2)
				i+=encpacklen
				j=0
				curRLE=False
			encoded=(encoded << 8) | hx
			i+=8
	if curRLE:#flush unfinished RLE packet if it exists
		encpack=encode_RLE_packet(j)
		encpacklen=len(encpack)
		encoded=(encoded<<encpacklen)+int(encpack,2)
		i+=encpacklen
	return f"{{:0{i}b}}".format(encoded)

"""encodes a RLE packet"""
def encode_RLE_packet(num:int) -> str:
	assert num>0
	num+=1	#otherwise we can't encode the number 1
	l=get_bitwise_num_length(num)
	length_pack=(1 << l-1)-2	#e.g. 110 for l==3 and 1110 for l==4
	value_pack=num - (1 << l-1)#taking away the first digit since it's obviously 1
	
	#putting em besides each other, in a string bc python
	return f"{{:0{l-1}b}}{{:0{l-1}b}}".format(length_pack,value_pack)

"""decodes a RLE packet - trailing data is ignored"""
def decode_RLE_packet(pack:str,l=None) -> int:
	#counting int length
	if l==None:
		l=get_RLE_packet_length(pack)
	#getting value and puttin it all together
	return int(pack[l//2:l],2)+(1<<(l//2))-1

"""encoding 001100 as 001010 - i.e. a 1 wherever the're's a diff to the prev val"""
def delta_encode(tx:str) -> str:
	result=[0,]*len(tx)
	prevbit=0
	for i,bit in enumerate(tx):
		bit=int(bit,2)
		result[i]=int(prevbit^bit)
		prevbit=bit
	return "".join(f"{i:1b}" for i in result)

"""see delta_encode"""
def delta_decode(tx:str) -> str:
	result=[0,]*len(tx)
	curBit=0
	for i,bit in enumerate(tx):
		if bit=="1":
			curBit=curBit ^ 1
		result[i]=curBit
	return "".join(f"{i:1b}" for i in result)

def get_RLE_packet_length(pack:str) -> int:
	l=0
	for char in pack:
		l+=1
		if char=='0':
			break
	return l*2

def get_bitwise_num_length(num):
	return floor(log2(num))+1

def get_numlist_from_binstring(bstr):
	return [int(f"{bstr[i:i+8]:08s}",2) for i in range(0,len(bstr),8)]

def get_json_from_binstring(bstr):
	return "["+(", ".join(f"0x{i:02x}" for i in get_numlist_from_binstring(bstr)))+"]"

def get_binstring_from_numlist(j):
	return "".join(f"{hx:08b}" for hx in j)

if __name__=="__main__":
	if len(argv)!=2 or argv[1] not in ("d","decompress","c","compress","t","test"):
		print("invoke this script with d/decompress or c/compress as its first argument, and it will read from textures.json and print to stdout")
		exit()
	elif argv[1] in ("t","test"):
		print("testing RLE packet encoding")
		for i in range(1,2**16):
			j=decode_RLE_packet(encode_RLE_packet(i))
			if i!=j:
				print(f"packet not gut: {i}!={j}")
		print("testing RLE encoding")
		for name, tx in textures.items():
			tx2=RLEdecode(RLEencode(tx))
			if get_binstring_from_numlist(tx)!=tx2:
				print(f"RLE not gut: {name} is fuckd\n{tx}\n{tx2}")
		print("testing delta encoding")
		for i in range(1000):
			randbin=f"{randrange(2**16):b}"
			deltbin=delta_decode(delta_encode(randbin))
			if(randbin!=deltbin):
				print(f"delta not gut:\n{randbin}\n{deltbin}")
		print("testing final compression")
		for name, tx in textures.items():
			tx=get_binstring_from_numlist(tx)
			cx=compress(tx)
			tx2=decompress(cx)
			if tx!=tx2:
				print(f"final not gut:\n{tx}\n{tx2}")
			else:
				print(f"{100*(1-len(cx)/len(tx)):3.0f}% compression: {name}")
			
		exit()
	elif argv[1] in ("c","compress"):
		func = compress
	else:
		func = decompress

	for name, tx in textures.items():
		print(f"{name}:")
		print(get_json_from_binstring(func(get_binstring_from_numlist(tx))),end="\n\n")