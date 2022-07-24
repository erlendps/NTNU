# Eksamen

## Teori

### Von-Neumann vs Harvard arkitektur

* Forskjellen ligger i hvor dataen og hvor programmet ligger
* I Von Neumann ligger de i samme minne
  * I Harvard er det to forskjellige busser

### RISC vs CISC

* To forskjellige typer instruksjonsett(assembly)
* RISC har færre instruksjoner totalt
* RISC-instruksjonene gjør en enkelt ting
  * du trenger altså flere instruksjoner i RISC for å gjøre det samme som en CISC intruksjon
* CISC operer direkte på minnne, RISC må først hente det i et register.
* De fleste RISC unstruksjoner tar en klokkesykkel

### Minnegapet

* Tar lenger tid å hente data enn å gjøre noe med det
* Da får man ikke utnyttet CPU optimalt
* Instruksjoner som henter data fra minnet bruker ofte lang tid.
  * Løsningen er cache, som er et minne med veldig rask aksesstid.

## Digitalteknikk

### Logiske porter

* Handler egentlig bare om at spenningen i transistorer (logisk høy/logisk lav) blir propagert gjennom transistorerene og output bestemmes av hvordan kjeden av transistorer er bygget opp.

* Signalet propageres, det vil si at det tar litt tid før man setter input til man får gyldig output.

#### Digitale komponenter

##### Mux

* Har input, kontrollsignaler og en output
* Kontrollsignalene velger hvilken input som blir output.

##### Demux

* Motsatt av mux
* har en input, kontrollsignaler og mange output
* Dersom input er 0 blir alt 0, men om det er 1 bestemmer kontrollsignalene hvilken output som får input som verdi.

##### Register

* Brukes til å lagre en verdi
* Output endres kun til input når klokkesignalet går høyt.

## Assembly

### Assembly-intruksjoner

* Instruksjoner til CPU-en i maskinen din
* Når man kompilerer et program endet det opp som assembly
* Finnes flere varianter av assembly (ref RISC og CISC)
* Eksempel er:
  * Intel x86_64 assembly
  * ARM assembly

### Minne

* Både instruksjoner og data lagres her
* Hver plass i minnet har en adresse
  * Denne brukes til å refere til den pkassen
* Det er dataen i programmet det lagres

### ARM-assembly

* RISC assembly
* Load/store arkitektur
* opererer på registre i CPU
* immediates er tall som er en del av intruksjonen, og er en måte å hardkode inn tall i programmet

#### Minneinstruksjoner

* kun to instruksjoner, load og store (LDR/STR)
* Benytter indirekte adressering
* MOV flytter rundt i register
* LDR/STR operer på minneadressen som ligger på f.eks R1
  * Dvs. LDR R8 R9 laster inn det som ligger på minneaddressen R9 og flytter inn i registeret R8
  * STR R8 R9 skriver det som ligger på R8 til minneadressen som ligger på R9

#### branches

* Flytter stedet programmet er, ofte avhengig av instruksjonen før
* Tar som regel inn en minneadresse
  * Ofte brukes i steden "labels", som er et navn som blir til en minneadresse når programmet blir kompilert.

## IJVM

### IJVM-arkitektur

#### ALU

* Aritmetisk-logisk enhet
* Det som gjør utregninger

##### ALU-flagg

* Settes etter en instruksjon
* Sier noe om hva output var
* Z står for zero og N for negative
* Brukes til branching

#### Kommunikasjon med minnet

* MAR - memory address register, inneholder adressen til dataen man vil hente ut
* MDR - memoery data register, inneholder verdien til dataen
* PC - program counter, har instruksjonsadressen.
* MBR har instruksjon som utføres, altså den faktiske binære instruksjonen
* 3 måter: write read fetch
* PC må inkrementeres for hver makroinstruksjon

##### Busser

* Samling av flere 1 bits linjer.
* Har en dekoder som sikrer at kun en av registerne som skriver til bussen.

