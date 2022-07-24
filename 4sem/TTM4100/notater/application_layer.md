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

## P2P
- no always on server
- arbitrary end systems directly communicate
- peers request service from other peers, provide service in return to other peers
	- self scalability - new peers bring new service capacity and new service demands
- peers are intermittently connected and change IP addresses

### Client-server vs P2P
$u_s, u_i, d_i, F$ are server upload bandwidth, peer-i upload bandwidth, peer-i download bandwidth and file size

- Server sequentially sends N copies in
	- $NF/u_s$
- Client-i takes $F/d_i$ time to download
	- If peer-i download speed is higher than server upload, this formula is used, otherwise $NF/u_s$ is used.
- Time to distribute F to N clients using client/server approach
	- $D_{cs} >= max(NF/u_s, F/d_{min})$

**now peer to peer**
- Server must send at least one copy ($F/u_s$)
- client i takes $F/d_i$ time to download
- $NF$ bits must be downloaded

fastest possible upload rate: $u_s + \sum u_i$
In which we get:
$$D_{P2P} >= max(F/u_s, F/d_{min}, NF/(u_s + \sum u_i))$$

- P2P is easier scaleable

## Video streaming and content distribution networks
### Multimedia: video
- sequence of images displayed at constant rate
- digital image: array of pixels
	- each pixel represented by bits
- coding: use redundancy within and between images to decrease number of bits used to encode image
	- spatial (within image)
	- temoral (from on image to next)
- CBR (constant bit rate): video encoding rate fixed
- VBR (variable bit rate): video encoding rate changes as amount of spatial, temoiral coding changes

### streaming stored video
**simple scenraio**
stored video on server accesses the internet and then the client.
Main challenges
- server-to-client bandwidth will vary over time, with changing network congestion levels (in house, in access network, in network core, at video server)
- packet los and delay due to congestion will delay playput, or result in poor video quality

Other challenges
- continuous playout constraint: once client playout begins, playback must match original timing
	- but network delays are variable, so will need client-side buffer to match playout requirements
- client interactivity: pause, fast-forward, rewind, jump through video
- video packets may be lost, retransmitted (TCP)

### Streaming multimedia: DASH
DASH = Dynamic. Adaptive Streaming over HTTP
- server
	- divides video file into multiple chunks
	- each chunk sotred, encoded at different rates
	- manifest file: provides URLs for different chunks
- client
	- periodically measures server-to-client bandwidth
	- consulting manifest, requests one chunk at a time
		- chooses maximum coding rate sustainable given current bandwidth
		- can choose different coding rates at different points in time.
- "intelligence" at client: client determines
	- when to request chunk
	- what encoding rate to request
	- where to request chunk

### Content distribution networks (CDNs)
- challenge: how to stream content (selected from millions of videos) to hundres of thousands of simultaneous
- option 1: singe, large mega-server
	- SPF (single point of failure)
	- point of network congestion
	- long path to distant clients
	- multiple copies of video sent over outgoing link
- option 2: store/serve multiple copies of videos at multiple geographically distributed sited
	- enter deep: push CDN servers deep into many access networks
		- close to users
	- bring home: smaller number of larger clustsers in Pop

- CDN stores scopies of content at CDN nodes
- subscriber request content from CDN
	- directed to nearby copy, retrieves content
	- may choose different copy if network path congested
- OTT (over the top) challenges: coping with a congested Internet
	- from which CDN node to retrieve content
	- viewer behavior in presence of congestion
	- what content to place in which CDN node?

- 