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