# Introduction
## 1.2 Network edge
### Network edge
- applications (e.g web, email)
- computing devices, **hosts = end systems** running network apps (e.g PC, mobile, xbox)

### Communication links
- fiber. Fiber is the dominating physical network access 
- copper
- radio
- satellite

### Access networks
- physical media: wired, wireless communication links

### Network core
- interconnected routers
- network of networks

### Routers
- fotward packets (chunks of data)

### Network nodes are interconnected by a physical media
- Physical link between transmitter/receiver pairs
	- point to point vs multipoint
	- transmits bits
- Guided media - wired
	- wireline signals propagate in solid media: copper, fiber, coax
	- simplex, duplex
- unguided media - wireless
	- no physical "wire": air, water, vacuum
	- bidirectional
	- signals propagate freely in electromagnetic spectrum
		- reflection
		- obstruction by objects
		- interference

## 1.3 Network core
### Circuit switching end-to-end: No longer an alternative to packet switching
- End-end resources reserved for "call" between source and destination
- Circuit segment idle if not used by call. It is not allowed to share the circuit
- Commonly used in traditional telephone networks

### Packet switching
Network core is a mesh of interconnected routers
- Data sent in discrete "chunks" = packets
- Packets are stored and forwarded from one router to the next, across links on path from source to destination
- Each packet transmittet at full link capacity
	- L-bit packet into link at R bps takes L/R seconds
- The internet is based on packet switching (packet switching is used end-to-end, from sender access via core to receiver access)

### Packet vs circuit switching
1 Mb/s link, user needs 100 Kb/s when "active" (10% of time)
- Circuit switching
	- Resources reserved, also in idle periods in-between data bursts
	- max 10 users simultaneously
- Packet switching
	- with 35 users, P(> 10 active at same time) is less than .0004
	- 99,96% of the time, all 35 users can be served simultaneously

### Internet structure: networks of networks
- End systems
	- connect to internet via access ISPs
		- residental, company and university ISPs
- Access ISPs
	- must be interconnected
	- Trsulting networks of networks is complex
	- Evolution was driven by economics and national policies

Given millions of access ISPs, how to connecnt them together?
Connect each access ISP to every other access ISP?

Option: connect each access ISP to a global transit ISP
Customer and proider ISPs have economic agreement

But if one global ISP is viable business, there will be competitors, which must be interconnected, and regional networks may arise to connect access nets to ISPs. Content providers (e.g Google, Microsoft) may run their own overlay network to bring services/content close to end users.

## 1.4 Packet-switched networks
### Throughput
Throughput is the rate at which bits are transferred between sender and receiver
- Unit is bits per time
- Instantaneous: rate at given point in time
- Average: rate over longer periode of time
- End-to-end throughput cannot exceed bottleneck link

### Delay
1. Nodal processing
	- Check bit errors
	- determine output link
2. Queuing
	- time waiting at output link for transmission
	- depends on congestion level of router
3. Transmission
	- R = link bandwidth (bit/s)
	- L = packet length (bits)
	- time to send bits into link = L/R
4. Propagation
	- Will always have a little bit of propagation delay

### Packet loss: Packet arriving to full queue is dropped
- Queue (buffer) preceding link has finite capacity
- Lost packet may be retransmitted by previous node, by source end system or not at all
- Maximum throughput is affected by **link speed/bandwidth**, **round-trip time** and **Packet loss**

## 1.5 Protocol layers and service models
- Network and end-to-end protocols
	- Between machines
	- All communication activity in internet governed by protocols

### 5 layer network protocol
- A protocol stack is the protocols of the various layers
- Application: supporting distributed applications
	- FTP SMTP HTTP
- Transport: process-process data transfer
	- TCP, UDP
- Network: routing of datagrams from source to destination
	- IP, routing protocols
- Link: data transfer between neighboring network elements
	- PPP, Ethernet
- Physical: bits "on the wire"

The service model of a layer is the services the layer offers to the layer above. The application layer is the the service layer.

## 1.6 Networks under attack
### Network/information security
- Not originally designed with much security in mind
	- original vision: a group of mutually trusting users
	- internet protocol designers playing "catch-up"
	- security considerations in all layers
- The field of network security is about
	- how bad guys can attack computer networks
	- how to defend from attacks
	- how to desgin architectures that are immune to attacks
The internet should follow these principles:
- Confidentiality
	- information kept private and secure between sender and receivers 
- Authentication:
	- sender and receiver want to confirm identity of each other (private and public keys)
- Message integrity
	- sender and receiver want to ensure message not altered in transit, or wafterwards without detection
- Availability:
	- services must be accessible and available to whom requires them

### Friends and enemies
- web browser/server for electronic transactions
- on-line banking client/server
- DNS (domain name server)
- routers exchangig routing table updates

**enemies can**
- eavesdrop: intercept messages
- Actively insert messages into connection
- impersonate: fake (spoof) source address in packet
- Hijack: "tak over" ongoing connection by removing sender or receiver, inserting himself in place
- deny service: prevent service from being used by others (e.g by overloading resources DOS)

### Bad guys can put malware into hosts via Internet
- malware can get into host from a virus, worm, trojan horse or mailcous URLs, often self-replicating: from an infected host seeks entry into other hosts
- Spyware: malware can record keystrokes, web sites visited, upload info to a collection site
- Virus: infection of a object actively executing and self-replicating
- Worm: standalone softawre that gets itself executed independet of other files, self-replicating
- Trojan horse: hidden part of some otherwise useful software, often on a web page
- Ransomware: trojan/worm threatens to publish the victim's data or perpetually block access to it unless a rnasom is paid
- Botnet: self-propagating malware designed to infect a host and connect back to a central server or servers that act as a command and control center for an entire network of compromised devices.

**where can you get these**
- Malicious URL: contained in spam or phishing messages, to download any type of malware to the affected computer
	- Infecteded hosts can be enrolled in a botnet, used for spam and DDOS
- Denial of Service: attackers make resources (server, bandwidth) unavailable to legitimate traffic by overwhelming resource with bougs traffic
- In a broadcast media one can plug into the network and listen to every packet
- promiscuous network interface reads/records all traffic passing by

**Bad guys can record and playback**
- sniff sensitive info and use later
	- e.g password if site not encrypted
	- faker server/links
- from system point of view, password holder is that user
**bad guys can use spoofed source addresses**
- send packet with false source address (mac, ip) or spoof URLs
- IP spoofing used in combination with DOS (denial of service) attack


