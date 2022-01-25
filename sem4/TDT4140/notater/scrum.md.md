# Scrum-metoden
## scrum-rammeverket
**Roller**
- Produkteier
- Scrum fasilisator (scrum master)
- Teammedlemmer

**Prosesser**
- Sprint planlegging
- Daglig møte
- sprint gjennomgang

**Artefakter**
- Produktkø
- sprintkø
- brenndiagram

### Hvordan bestemme lengde på sprint
- Balanse mellom:
	- mulighet til å gjøre ferdig oppgaver som gir verdi
	- hvor tidkrevende testing og andre oppgaver rundt hver avslutning av iterasjon
	- hvor ofte krav endres

**Rammeverket**
- Produktkø
	- Samling av alle brukerhistorier som ønskes å bli implementert
- Iterasjonsplanlegging
	- planlegger hva (hvilke brukerhistorier) som skal bli implementert i denne iterasjonen
- Iterasjonskø
	- Inneholder en kø av features som er planlagt å bli implementert denne sprinten
- sprint
	- arbeid i team med iterasjonskøen
	- daglig møte
		- gjennomgang av hva hvert medlem gjorde dagen før, og hva de planlegger å gjøre idag. Om det er noe de lurer på/synes er vanskelig kan det tas opp her
- iterasjons-gjennomggang
	- gå over hva som ble gjort i iterasjonen, gjerne en demo av iterasjonen
- iterasjonsretrospektiv
	- reflektere over hva som gikk bra og hva som gikk dårlig. Hva kan man gjøre bedre til neste gang
- produktinkrement

## Produktkø
### Releaseplanlegging
1. Prioriter produktkø- rangert etter viktighet. Noen bruker MoSCoW
	- M - must
	- S - should
	- C - can
	- W - wont
2. Estimer arbeidsmengde på viktigste elementer i produktkø
3. Estimer utviklingshastighet
4. Fordel arbeid på releaser

## Sprint-planlegging
- Sprint-prioritering
	- analyser og evaluer produktkø
	- velg sprint mål
- Sprint planlegging
	- bestem hvordan man skal oppnå sprint-målet
	- lag sprintkø fra produktkø
	- estimer sprintkø i timer

- ufra kapasitet velger teamet i samråd med produkteier oppgaver til neste iterasjon
- iterasjonskø lages ved å dekomponere oppgaver fra produktkø
	- oppgaver identifisereres og estimeres (1-16 timer)
	- et felles arbeid, ikke noe scrum master skal gjøre alene
- Teamet må ta hensyn til overordnet teknisk arkitektur

### estimering med planning poker
- Bruk en kjent oppgave som referanse, eller gå grundig gjennom en av oppgavene og bruk den som referanse
- Gi den kjente oppgaven et antall poeng, for eksempel 2 eller 5
- Noen forklarer oppgaven som skal estimeres
- alle gjør seg opp en individuell mening om arbeidsmengde
- alle viser frem sitt estimat ved å vise ett kort
- de som har laveste og høyeste estimat argumenterer for sin estimat
- gjenta estimering til konsensus

## Sprint
**Når er en oppgave ferdigstilt?**
Når tester er bestått, peer-reviewed, demoet osv.

En sprintkø er dynamisk. Kanskje man forventer at en oppgave tar 5 timer, neste dag kan man forvente at den samme oppgaven tar 6 timer, selv om den var jobbet med igår.

### Sprint-gjennomgang
Viser mest mulig fungerende kode. Inviterer interessenter, teammedler. Teamet presenterer den neste iterasjonen. Møtet skal helst ikke være for langt.

### Team A erfaringsrapport
1. Hva tenker du er de viktigste erfaringene fra TEAM A
	De opplevde gode resultater. De brydde seg om produktet, og feiret seg selv og de andre på teamet.
2. Hvilke råd er relevant for et studentprosjekt som skal bruke scrum
	Scrum er et rammeverk, ikke en oppskrift man må følge punkt til prikke. Finn det som passer for gruppen, men gjerne prøv de fleste ideene i scrum før man bestemmer seg nærmere om man ønsker å beholde dem. 
	
	## Oppgave A: distribuert retrospektiv
	1. Hva er relevante råd fra artikler og bøker om retrospektiv?
		Det er lett å hoppe over retrospektiver, men retrospektiver er en av de viktigste aktivitetene i scrum. Det anbefales sterkt å ikke hopper over disse.
		I stedenfor å innføre endringer i arbeidsvaner med en gang et problem kommer opp, kan det ofte være lurt å vente med å gjøre noe, og se hvordan problemet utspiller seg. Ofte kan slike problem seg selv.
		Del opp i 3 kolonner: bra, kunne blitt gjort bedre og forbedringer/til neste gang.
		Sitt i et uforstyrret lokale.
		En som er sekretær.
	2. Man møtes på Google Meet, Zoom eller andre videokonferansetjenester. Teamet, produkteier og scrum master deltar på møtet som varer i ca 1 time. Bruker metroretro (https://metroretro.io). Tilbakemeldinger fra teammedlemmer hvor de enten ønsker å være anonyme, eller at det er et så stort problem at det krever et eget punkt blir sendt inn til scrum master før møtet.
		Tidsplan:
		- Introduksjon (10 min)
			- Scrum master går gjennom sprinten. Hva som har blitt gjort, med hjelp fra teamet. Tar med viktige hendelser og beslutninger som ble gjort i sprinten.
		- Bra/kunne vært bedre/til neste gang (20 min)
			- Alle får sjansen til å si hva de synes gikk bra, hva som kunne vært bedre, og hva man ville gjort neste gang. Sekretær skriver ned punktene, og legger til i metroretro. 
		- Diskusjon av punkter (25 min)
			- Scrum master går gjennom punktene, og det er diskusjon i plenum om hva som burde bli gjort annerledes til neste gang. Det skal også komme frem om teamet er enige i hva som ble gjort bra og hva som var mindre bra.
			- Alle får et visst antall poeng hvor de stemmer på hvilke ting de ønsker teamet skal forbedre seg på i løpet av neste sprint.
		- Øvrige punkter (10 min, vil ikke alltid være med)
			- dersom det er kommet inn andre punkter blir de tatt opp her og diskutert. Scrum master tar en beslutning
		- Avslutning (5 min)
			- scrum master oppsummerer de viktigste punktene og hva som teamet skal fokusere på for å gjøre det bedre i neste sprint. 