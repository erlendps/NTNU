# ER-modellering
## ER-modellen
Består av:
- Entiteter (objekter) - entitetsklasser
- Attributter - verdier/felter
- Relasjoner - relasjonsklasser
- Nøkler
	- Understrekning tyder at det er en unik nøkkel internt i klassen (som regel)
- Restriksjoner
- Svake entitetsklasser
	- Identifiserende relasjonsklasse + delvis nøkkel
- Rekursive relasjonsklasser
	- Roller
	
## Fra miniverden til realisert database
- Databasen som modell av en del av virkeligheten
- Miniverden er den relevante delen av virkeligheten
- Miniverden gir oss:
	- Relevante (data-)objekter
	- Relevante sammenhenger mellom objekter
	- Relevante regler som gjelder

Vi går fra miniverden til en konseptuell modell (ER) til en logisk modell og til slutt den fysiske modellen.

## Entiteter
- Entitet er et objekt eller "noe" som eksisterer i miniverden
- Beskriver egenskaper ved entiteter ved hjelp av attributter
- Attributt henter sine mulige verdier fra et domene
- Ulike typer attributter:
	- Enkle, sammensatte
	- En eller flere verdier
	- Avledet (vha en regel)
	- Nøkkelattributter (identifikatorer)

### Entitetsklasse
- Mengden av alle likeartede entiteter som er av samme klasse og har samme egenskaper
- Alle entitetsklasser må ha en unik identifikator (nøkkel)
- Datatyper tas vanligvis ikke med i diagrammer
- Entitetsklasser er firkantet, attributter er elipser. Nøkkelattributter er streket under, avledede attributter har stiplet elipse, flerverdi attributt har dobbel elipse og sammensatte attributter består av flere enkle attributter.

## Relasjoner
- **Relasjon** - sammenheng (assosiasjon) mellom to eller flere entiteter
- Modellerer informasjon som viser en sammenheng mellom to eller flere entiteter:
	- "en student har tatt eksamen i et emne"
	- "en person eier en bil"
- Kan ha egenskaper på samme måte som entiteter, altså egne attributter
	- "karakteren som en student fikk på eksamen i et emne"
- Relasjonen eksisterer ikke uten de entitetene som deltar

### Relasjonsklasser
- Mengden av likeartede relasjoner mellom samme entitetsklasser

### Om relasjoner
- Relasjoner kan ha egenskaper som modelleres som attributter
- Relasjonens **grad** er antall entiteter som inngår. Har alltid grad 2 eller høyere.
- Relasjoner eksister ikke uten de entitetene som inngår

### Relasjonsklasser: restriksjoner
#### Kardinalitetsforhold
- Begrenser hvor mange entiteter en entitet kan ha samme relasjon til.

#### Deltakelseskrav
- Kan spesifisere at en entitet må ha en relasjon til minst en entitet.

#### Strukturelle restriksjoner
- Spesifiserer både kardinalitets og eksistens-restriksjoner i et min-maks-par.
	- F.eks så kan en bil ha 0 eller 1 eier (en (0, 1)-relasjon). Likeså kan en person eie 0 eller flere biler (en (0, n)-relasjon).
- Vi har en-til-en (1, 1), null-til-en (0, 1), null-til-mange (0, n), en-til-mange (1, n) og mange til mange (n, n).

#### Sammenligning
- Alternative måter å spesifisere det samme
- Maks bytter side

## Trinn i utvikling av datamodell
1. Finn de nødvendige entitetsklassene
2. Finn attributter for entitetsklassene
3. Finn de nødvendige relasjonsklassene
4. Finn attributter for relasjonsklassene
5. Bestem nøkler for entitetsklassene
6. Bestem restriksjoner for relasjonsklassene
7. Vurder om du har en god modell, gjør ev. nødvendige endringer

## Rekursive relasjonsklasser
- Relasjonsklasser der samme entitetsklasse deltar flere ganger
- En entitetsklasse kan ha flere roller i relasjonsklassen

## Svake entitetsklasser
- En entiteetsklasse er en mengde entiteter
	- Vi kan altså ikke ha to like entiteter i en entitetsklasse
	- Alle entitetsklasser må ha en unik identifikator
	- Eksempel:
		- Kommuner har et unik KommuneNr og et kommunenavn
		- Kommuner har gater som har unike gatenavn innenfor kommunen
		- Problem: ENtitetsklassen Gate har ikke noen (naturlig) nøkkel.
- En løsning er å legge til et ekstra nøkkelattributt (gateNr)
	- Da blir Gate en regulær entietsklasse
	- Ulempen er at vi mp "administere" nøkkelattributtet og sørge for at det er unik på tver av kommuner
- En entitetsklasse som mangler en "naturlig" nøkkel, kan av og til identifiseres gjennom en identifiserende relasjonsklasse til en annen (identifiserende) entitetsklasse
- Dette kalles en svak entietsklasse
	- Mangler nøkkel
	- Den må være eksistensavhengig (1, 1)/(1, n) av deltakelse i den indentifiserende relasjonsklassen
	- Den må ha ett eller flere attributt som identifiserer entiteter unikt sammen med nøkkelen til den indetifiserende entitetsklassen.
- Fordelen er at vi unngår å legge til et "unødvendig" nøkkelattributt
- En identifiserende relasjonsklasse blir tegnet med dobbel rombe. Den svake entitetsklassen er tegnet med en dobbelt boks og den delvise nøkkelen er stiplet under. Den identifiserende entitetsklassen er en regulær entitetsklasse.