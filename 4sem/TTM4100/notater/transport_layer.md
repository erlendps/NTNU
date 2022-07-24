# Transport layer
## 3.1 Transport-layer services
- transport layer provides communication between end-system processes
	- transfers application data/protocol
- Sender: breaks application messages into segments, passes to network layer
- receiver: reassembles segments into messages, passes to application layer
- transport layer relies on and enhances the network layer service
	- IP datagrams including
		- transport-layer segment as service data
		- source and destination IP address

- Transport layer is accessed from the applications through a socket API

## 3.2 Multiplexing and demultiplexing
- Multiplexing at sender
	- gathering data from sockets, adding protocol header
- demultiplexing
	- delivering received segments to correct socket

TCP socket idenitified by 4-tuple of IP source and destination and Port source and port destination

UDP socket identified by destination IP and destination port, it does not care where the packet came from.

## UDP
- "bare bones" internet transport protocol
- "best effort", UDP segments may be lost or delivered out of order
- Connectionless
	- no handshaking between UDP sender, receiver
	- each UDP segment handled independently of others

UDP checksum detect errors (e.g flipped bits) in transmitted segment

## Principles of reliable data transfer
rdt_send() kalles fra applikasjonslaget og brukes til å sende data (reliable) gjennom transportlaget
udt_send() kalles for å sende data over til en upålitelig kanal til destinasjon (network layer)
rdt_rcv() kalles når pakken ankommer for å pålitelig overføre til transportlaget
deliver_data() leverer data pålitelig til applikasjonslaget.

Pålitelighet handler om at data er overført riktig (ikke er korrupt eller lignende)
- Må vite hva man har sendt og hva man har mottat
- Dette krever forskjellige tilstander
- Finite state machines kan brukes for å beskrive protokollen

### Reliable transfer over a reliable channel
- underlying channel perfectly reliable
	- no bit errors
	- no loss of packets
- sender sends data into underlying channel
- Receiver reads data from underlying channel

Sender:
(Wait for call from above) rdt_send(data)
packet = make_pkt(data)
udt_send(packet)

Receiver:
(wait for call from below) rdt_rcv(packet)
extract(packet, data)
deliver_data(data)

### Reliable transfer over a unreliable channel
- underlying channel may flip data bits in packets
- new mechanism for error detection:
	- checksum
- receiver feedback:
	- control messages (ack, nak) from receiver -> sender
	- ACKs (acknowledgements)
		- receiver explicity tells sender that packet received OK
	- Negative acknowøedge,emts (NAKs):
		- receiver explicity tells sender that packet had errors
			- sender retransmits packet on receipt of NAK

## RDT 3.0 
### Stop and wait operation
- first packet bit transmitted at t=0
- last packet bit ransmitted at $t=\frac{L}{R}$
	- first packet bit arrives
	- last packet bit arrives, send ACK
- ACK arrives, send next packet at $t=RTT + \frac{L}{R}$
- $U_{sender}$: Utilization - fraction of time sender busy sending
	- $U_{sender} = \frac{L/R}{RTT + L/R}$

This is a slow protocol. Better to use pipelined protocols
- packets in flight are yet to be acknowledged
- range of sequence numbers must be increased
- Buffering at sender and/or receiver
- When sending 3 packets at a time:
	- $U_{sender} = \frac{3*L/R}{RTT + L/R}$

### Retransmission strategies
- Go-back-N retransmission
	- sender can have up to N unACKed packets in pipeline
	- Receiver sends cumulative ACKs
		- doesnt ACK packet if theres a gap in received packets
	- sender has timer for oldest unACKed packet
		- if timer expires retransmit all unACKed packets
	- **Senders view of sequence numbers**
		- k-bit sequence number in packet header
		- "Window" of up to N, consecutive unacked packets allowed
		- "Cumulative ACK": ACKs all packets up to, including sequence number n
			- may receive duplicate ACKs
		- timer for each in-flight packet, only one countdown timer
		- timeout(n): retransmit packet n and all higher sequence number packets in window.
	- With n-bit sequence number field, the size of the sequence number space = $2^n$. Maximum window = $2^{n} - 1$
- Selective repeat retransmission
	- sender can have up to N unACKed packets in pipeline
	- Receiver ACKs individual packets
	- Sender maintains timer for each unACKed packet
		- when timer expires, retransmit only unACKed packet
	- Event actions at sender:
		1. Data received from above
			- if next available seq (nextseqnum) # in window, send packet
		2. Timeout(n)
			- resend pkt n, restart timer
		3. ACK(n) in \[sendbase, sendbase+N\]
			- mark pkt n as received (change colour from yellow to green)
			- if n smallest unACKed packet, advance send_base to next unACKed seq #
	- Event actions as receiver
		1. Packet(n) in \[rcvbase, rcvbase+N-1\] - within window
			- send ACK(n)
			- out of order: buffer packet
			- in-order:deliver all buffered, in-order packets, advance rcv_base to next not-yet-received packet
		2. Packet(n) in \[rcvbase-N, rcvbase-1\] - duplicate
			- ACK(n)
		3. otherwise ignore the packet
	- With n-bit sequence number field, the size of the sequence number space = $2^n$. Maximum window = $2^{n-1}$

## 3.5 Connection-oriented transport: Transmission Control Protocol
### TCP
- point to point
	- one sender, one receiver
- connection-oriented
	- handshaking. Initiates sender and receiver state before data exhange
- Error controlled
	- bit error, segment error
- Flow control
	- sender will not overwhelm receiver
- Congestion control
	- sender adjust transmission dependent on network load
- Full duplex data
	- bi-directionaldata flow in same connection
- MSS = maximum segment size

- Reliable, in-order byte stream
	- no "message boundaries"

### MSS - what is it?
The MSS announcement (often mistakenly called a negotiation) is sent during the three.way handshake by both sides saying: "I can accept TCP segments up to size x". 



