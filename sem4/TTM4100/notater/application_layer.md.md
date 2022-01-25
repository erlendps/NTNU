# Application overview
## Creating a network app
- write programs that 
	- run on different end systems
	- communicate over network
	- e.g, web server software communicates with browser software
**no need to write software for network-core devices**
- network-core devices do not run user applications
- applications on end systems allows for rapid app development, propagation

## Client-server paradigm
**server**
- always on-host
- permanent IP address
- often in data centers

**Clients**
- contact, communicate with server
- may be intermittently connected
- may have dynamic IP addresses
- do not communicate directly with each other

## Peer-peer architecture
- no always-on server
- arbitrary end systems directly communicate
- peers request service from other peers, provide service in return to other peers
- peers are intermittently connected and change IP addresses
	- complex management

## processes communicating
**process**: program running within a hsot
- within same host, two processes communicate using inter-process communication
- processes in different hosts communicate by exchangig messages

client process: process that initiates communication
server process: process that waits to be contacted

## Socekets
- process esnds/receives messages to/from its socket
- socket analogous to door
	- sending process shoves message out door
	- sending process relies on transport infrastructure on other side of door to deliver message to socket at receiving process
	- two sockets involved: one on each side

## Addressing processes
- to receive messages, process must hae identifier
- host device has unique 32-bit IP-address
- Does IP address of host on which process runs suffice for udentifying the process?
	- No, many processes can be running on same host
- identifier includes both IP address and port numbers associated with process on host

## An application-layer protocol defines:
- types of messages exchanged (request, response)
- message syntax:
	- what fields in messages and how fields are delineated
- message semantics
	- meaning of information in fields
- rules for when and how processes send and respond to messages

**open protocol**
- defined in RFCs, everyone has access to protocol definition
- allows for interoperability

## What transport service does an app need?
**data integrity**
- some apps require 100% reliable data transfer
- other apps can tolerate some loss

**timing**
- some apps require low delay to be effective

** throughput**
- some apps require minimum amount of througput to be effective
- other apps make use of whatever throughput they get

**security**
- encryption, data integrity
