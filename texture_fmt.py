#!/usr/bin/env python3
#
# Basically the same algorithm as this one here: https://youtu.be/aF1Yw_wu2cM
# with some minor deviations and lots of bugs
#
# TODO: describe flags, delta encoding and RLE in Readme or something
from sys import argv, exit
from os import makedirs, path, listdir
from math import log2,floor
from random import randrange
import pyjson5	#pyjson5
import png		#pypng

def decompress(tx:str,wholesize:int) -> str:
	mode=tx[0:2]
	tx=tx[2:]
		
	result = RLEdecode(tx,0)
	
	if mode=="01":
		result = delta_encode(result)
	elif mode=="10":
		result = delta_decode(result)
	elif mode=="11":
		result = delta_decode(delta_decode(result))
	
	return str.ljust(result,wholesize,'a')

def compress(tx:str) -> str:
	
	#removing trailing zeroes because they're later filled in via width/height of the texture
	#tx=str.rstrip(tx,'0')
	
	encs=([],[],[],[])#TODO: refactor. This is garbage and inefficient and I don't wanna see it
	
	#array for playing favourites
	for block_size in (2,3,4,5,6,7,8):
		encs[0].append(
			RLEencode(tx,block_size)
		)
		encs[1].append(
			RLEencode(
				delta_decode(tx),
				block_size
			)
		)
		encs[2].append(
			RLEencode(
				delta_encode(tx),
				block_size
			)
		)
		encs[3].append(
			RLEencode(
				delta_encode(delta_encode(tx)),
				block_size
			)
		)
	for a in encs:
		print(*[f"{100*(1-(2+len(cx))/len(tx)):+03.0f}%" for cx in a])
	chosen = min(min(encs[0],key=len),min(encs[1],key=len),min(encs[2],key=len),min(encs[3],key=len),key=len)
		
	if chosen in encs[0]:
		return "00"+chosen
	elif chosen in encs[1]:
		return "01"+chosen
	elif chosen in encs[2]:
		return "10"+chosen
	elif chosen in encs[3]:
		return "11"+chosen
	else:
		raise Exception("Unreachable")

"""decodes binary string to binary string per RLE"""
def RLEdecode(tx:str,wholesize:int) -> str:
	decoded=[]
	i=3
	l=len(tx)
	block_size=2+(int(tx[0:3],2))
	assert 2<=block_size<=10#we need at least a block size of 2 and can only encode up to 10, since we only have 3 bits for that in the flag
	assert block_size<=8#the current decoder implementation doesn't support more than 8 due to the helper functions
	while i<=l:
		curBlock=tx[i:i+block_size]
		i+=block_size
		if curBlock=="0"*block_size:
			packet_length=get_RLE_packet_length(tx[i:])
			if i+packet_length>l:#safety measure in case we end on an 0x00
				break
			decoded.append("0"*block_size*decode_RLE_packet(tx[i:i+packet_length],packet_length))
			i+=packet_length
		else:#non-RLE packets are just raw data. No decoding here.
			decoded.append(curBlock)
	return str.ljust("".join(decoded),wholesize,'a')

"""encodes list of hexadecimal values to binary string per RLE"""
def RLEencode(tx:str,block_size:int) -> str:
	encoded=""
	i=0 # index in encoded texture
	curRLE=0
	l=len(tx)
	assert 2<=block_size<=10#see RLEdecode
	while i<l:
		curBlock=tx[i:i+block_size]
		i+=block_size
		if curBlock=="0"*block_size:
			if curRLE==0:
				encoded+="0"*block_size
			curRLE+=1
		else:
			if curRLE:
				encoded+=encode_RLE_packet(curRLE)
				curRLE=0
			encoded+=curBlock
	if curRLE:
		encoded+=encode_RLE_packet(curRLE)
	return f"{block_size-2:03b}"+encoded

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

def get_png_from_numlist(numl,w,h):
	return get_png_from_binstring(get_binstring_from_numlist(numl),w,h)
	
def get_png_from_binstring(numl,w,h):
	return png.from_array(([numl[x+y*w]=='1' for x in range(w)] for y in range(h)),"L;1",{"height":h,"width":w})

def get_binstring_from_png_filename(fp):
	x=""
	for row in png.Reader(filename=fp).asRGBA8()[2]:
		x+="".join("1" if sum(row[i:i+4])//(4*255) else "0" for i in range(0,len(row),4))
	return x

if __name__=="__main__":
	with open("./textures.json") as f:
		data = f.read()
	textures = pyjson5.decode(data)
	del data
	if len(argv)!=2 or argv[1] not in ("d","decompress","c","compress","t","test","dump","load"):
		print("invoke this script with d/decompress c/compress t/test dump or load as its first argument, and it will read from textures.json and print to stdout")
		exit()
	elif argv[1]=="dump":
		print("dumping to ./dump/")
		if not path.isdir("./dump"):
			makedirs("./dump/")
		for name,tx in textures.items():
			get_png_from_numlist(tx[2],tx[0],tx[1]).save(f"./dump/{name}.png")
		exit()
	elif argv[1]=="load":
		if path.isdir("./load"):
			print("loading from ./load/")
			for fn in listdir("./load/"):
				fp=path.join("./load/", fn)
				tx=get_binstring_from_png_filename(fp)
				print(f"{fn}\n{get_json_from_binstring(tx)}")
		else:
			print("./load/ doesn't exist")
		exit()
	elif argv[1] in ("t","test"):
		print("testing RLE packet encoding")
		for i in range(1,2**16):
			j=decode_RLE_packet(encode_RLE_packet(i))
			if i!=j:
				print(f"packet not gut: {i}!={j}")
		print("testing RLE encoding")
		for name, tx in textures.items():
			tx=get_binstring_from_numlist(tx[2])
			for block_size in range(2,9):
				tx2=RLEdecode(RLEencode(tx,block_size),len(tx))
				if tx!=tx2:
					print(f"RLE not gut: {name} with block_size {block_size} is fuckd\n{tx}\n{tx2}")
		for i in range(1000):
			tx=f"{randrange(2**16):016b}"
			for block_size in range(2,9):
				cx=RLEencode(tx,block_size)
				tx2=RLEdecode(cx,len(tx))
				if tx!=tx2:
					print(f"RLE not gut: random with block_size {block_size} is fuckd\n{cx}\n{tx}\n{tx2}")
		print("testing delta encoding")
		for i in range(1000):
			randbin=f"{randrange(2**16):016b}"
			deltbin=delta_decode(delta_encode(randbin))
			if(randbin!=deltbin):
				print(f"delta not gut:\n{randbin}\n{deltbin}")
		print("testing final compression")
		for name, tx in textures.items():
			tx=get_binstring_from_numlist(tx[2])
			cx=compress(tx)
			tx2=decompress(cx,len(tx))
			if tx!=tx2:
				print(f"final {name} not gut:\n{cx}\n{tx}\n{tx2}")
			else:
				print(f"{100*(1-len(cx)/len(tx)):-3.0f}% compression: {name} ({len(cx)-len(tx):+d}b)",end="\n\n")
		exit()
	elif argv[1] in ("c","compress"):
		func = compress
	else:
		func = decompress

	for name, tx in textures.items():
		tx=get_binstring_from_numlist(tx[2])
		cx=func(tx)
		print(f"{name}: {100*(1-len(cx)/len(tx)):.0f}% compression ({len(cx)-len(tx):+d}b)")
		print(get_json_from_binstring(cx),end="\n\n")