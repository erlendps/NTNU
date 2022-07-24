# Normaliseringsteori
**Fotografier**

| <ins>BildeID</ins> | Tittel | FotografID | Navn |
| --- | --- | --- | --- |
| 1 | Geilo | 1 | Wilse |
| 2 | Beach | 2 | Erwitt |
| 3 | Korgen | 1 | Wilse |
| 4 | Nidaros | NULL | NULL |
| 5 | Central Park | 2 | Erwitt |

- ved problemer (lav normalform) splitter vi opp i "mindre" tabeller (færre attributter og høyere normalform)
	- blander ikke "unødig mye" data i samme tabell
- Unngår redundans og innsettings-, oppdaterings- og sletting-anomalier
- Opnår fordeler med hensyn til representasjon av data, fare for inkosistens og oppdaterings-arbeid
- MEN får et design med flere tabeller, økt behov for å joine tabeller og ofter mer ressurskrevende spørringer

## Restriksjoner: Generelt
- Datatilstand - database-forkomsten (dataene) på en gitt tid
- databasetilstanden er konsistent når alle reglene i miniverdenen er oppfylt
- restriksjoner begrenser hvilke data som kan finnes i en konsistent databasetilstand
- Inherent constraints - som er en del av datamodellen og derfor alltid håndheves av DBMS
	- eksempel: ikke to like rader som følger av at tabellen er en mengde rader
- Explicit constraints - som kan uttrykkes i datamodellen. Hpndheves av DBMS
	- eksempler: primærnøkkel, fremmednøkkel, datatyper, verdi-begrensinger
- Applikasjonsbaserte restriksjoner - som må håndteres utenfor datamodellen
	- eksempel: ingen kan tjene mer enn sjefen.

### Funksjonelle avhengigheter
- en delmengde av alle restriksjoner kan uttrykkes som funksjonelle avhengigheter
	- grunnlag for nøkler og viktige designregler
- **FotografID -> Navn**
	- alle rader som har samme verdi for FotografID må ha samme verdi for Navn
	- Det er bare ett Navn knyttet til en bestemt FotografID, men en fotograf kan bytte navn
	- Funksjonelle avhengigheter uttrykker en sterk sammenheng mellom attributter
	- legg merge til det kan være flere FotografID som har samme Navn og da vil vi ikke ha: **Navn -> FotografID**
- **X -> Y** der X, Y er en del av R uttrykker en restriksjon på alle lovlige tabellforekomster for R:
	- alle rader, t_i og t_j i en forekomst r(R)
		- som har samme verdier for attributtene i X
		- må ha samme verdier for attributtene i Y

#### Hvilke FA-er kan gjelder?
- FAer må gjelde for alle mulige tabell-forekomster
- En (tilfeldig) tabellforekomst kan bare avkrefte at en FA kan gjelde
	- A -> A er triviell, vil alltid være oppfylt.

#### Utledningsregler
- Refleksiv: Hvis Y del av X, så X -> Y
- Augmentering: X -> Y gir XZ -> YZ
- Transitiv: X -> Y, Y -> Z git X -> Z
- Dekompisisjon: X -> YZ gir X -> Y
- Additiv: X -> Y og X -> Z gir X -> YZ
- pseudotransitiv: X -> Y og WY -> Z gir WX -> Z

#### TIllukningen til en mengde FA-er: F⁺
- F er en mengde funksjonelle avhengigheter
- F⁺ = {X -> Y | X -> Y}
	- opplistning av alle FA-er som gjelder når vi har F.
	- Merk: F og F⁺ uttrykker akkurat samme restiksjon
- F⁺ inneholder mange trivielle funksjonelle avhengigheter
	- X -> Y, der Y del av X uttrykker ingen restriksjon
- Krevende å beregne F⁺, men sjeldent interessant
- Likevel nyttig å kunne snakke om alle FA-er som vil gjelde når FA-ene i F gjelder

#### Tillukningen til en mengde attributter: X⁺
- X⁺ = {Y e R | X -> Y e F⁺}
	- alle attributter som er funksjonelt avhengige av X

## Nøkkelbegrep
### Supernøkkel
- En supernøkkel for en tabell R er en mengde attributter S i R slik at
	- ingen forekomst av tabellen kan ha to rader med samme verdier for S
	- Supernøkkelen er en unik identifikator for tabellen
	- Det må være slik at: S -> alle de andre attributtene i tabellen
	- Siden S -> S vil det alltid være slik at S⁺=R
	- Alle tabeller vil ha minst en supernøkkel, ofte vil det være flere
		- siden vi ikke kan ha like rader i en tabell vil "alle attributtene" alltid være en supernøkkel
### Nøkkel
- En nøkkel K er en minimal supernøkkel
	-	Vi kan ikke fjerne noe attributt fra K og fortsatt ha en supernøkkel
	-	alle nøkler er supernøkler, noen supernøkler er nøkler
	-	K -> R og K⁺ = R
	-	Alle supernøkler inneholder minst en nøkkel
	-	En nøkkel er en "sterkere restriksjon" (enn en supernøkkel med flere attributter) fordi den inneholder så få attributter som mulig.

### Kandidat-, primær- og sekundærnøkler
- Alle tabeller vil ha minst en nøkkel
- En tabells mulige nøkler utgjør tabellens kandidatnøkler
- Primærnøkkelen velges blant kandidatnøklene
- Øvrige kandidatnøkler blir tabellens sekundærnøkler
	- her kan vi velge å tillate NULL-verdier

### Nøkkel- og ikke-nøkkel-attributter
- Nøkkelattributt
	- attributter som inngår i en eller flere kandidatnøkler
- Ikke-nøkkelattributt
	- attributter som ikke inngår i noen kandidatnøkkel
- Partisjonerer attributtene i en tabell i to deler

## Normalformer
- Regler som stiller stadig strengere krav til tabeller
	- sikrer at vi har tabeller som unngår uheldige egenskaper
- Første normalform (1NF)
	- attributtens domener inneholder atomiske verdier
	- verdien til et attributt er en enkelt verdi fra domenet
	- sikrer "flate, 2-dimensjonale tabeller"
	- unngår sammensatte attributter, flere verdier og nøstede tabeller
- Alle thøyere normalformer forutsetter 1NF
- Alle tabeller på 2NF er på 1NF, alle på 3NF er på 2NF osv.

### Full funksjonell avhengighet
- En funksjonell avhengighet X -> Y er en full funksjonell avhengighet hvis det er umulig å fjerne et attributt, A e X og ha (X - {A}) -> Y
	- inneholder ikke "overflødige" venstreside-attributter
	- kan tenke på den som en "sterkere" regel enn en delvis funksjonell avhengighet, der vi kan fjerne venstreside-attributt

### Andre normalform 2NF
- en tabell er på andre normalform hvis og bare hvis
	- det ikke finnes noen ikke-nøkkel-attributter som er delvis avhengig av en kandidatnøkkel
		-	i slike tilfeller kan man lage nye tabeller, som fjerner kilden til redundans og blir kvitt anomaliene

### Tredje Normalform 3NF
- En tabell er på tredje normalform hvis og bar ehvis det for alle funksjonelle avhengigheter på formen, X -> A, som gjelder for tabellen er slik at
	- X er en supernøkkel i tabellen, eller
	- A er et nøkkelattributt i tabellen
- X -> BCD utleder X -> B, X -> C, X -> D

Løsningen på slike problemer er igjen å splitte i to tabeller

### Boyce-Codd normalform
- 3NF kan ha redundansproblemer ved overlappende kandidatnøkler
- En tabell er på Boyce-Codd normalform (BCNF) hvis og bare hvis det for alle funksjonelle avhengigheter på formen, X -> Y, som gjelder for tabellen er slik at:
	- X er en supernøkkel i tabellen
- Alle venstresider i FA-er må altså være supernøkler
- Løser eventuelle redundanser og anomalier ved å splitte tabeller

## Kriterier
Starter med R og F. Finner en dekomponering i tabeller (projeksjoner) som kan lagre det samme og har bedre egenskaper
1. Normalform
	- ser på hver enkelt tabell for seg selv
2. Attributtbevaring
	- må kunne lagre det samme
3. Bevaring av funksjonelle avhengigheter
	- Beholde samme restriksjon
4. Tapsløs sammenstilling til utgangspunktet
	- Ikke skape "falsk" data

### Attributtbevaring
- Alle attributter i R må finnes i minst en av projeksjonene
- Betyr: Kan lagre de samme dataene

### Bevaring av funksjonelle avhengigheter
- F representerer en restiksjon på alle tabellforekomster av R
	- Må ivaretas i tabellene i dekomponeringen
- Mål
	- alle funksjonelle avhengigheter i F skal finnes i en eller flere R_i.er eller kunne utledes fra FA-ene som gjelder i R_i-ene
- Eller vil vi få inter-tabell-avhengigheter som vi må joine tabeller for å sjekke.

### Tapsløs sammenstilling til utgangspunktet
- En dekomponering av R, D = {R_1, R_2, ..., R_m}, har tapsløst-join-egenskapen hvis det for alle forekomster r(R) som oppfyller F, er slik at:
	- $*(\Pi_{R_1}(r), \Pi_{R_2}(r), ..., \Pi_{R_n}(r)) = r$

**Sjekke T-J-egenskapen**
- En dekomponering av R, D={R_1, R_2} har T-J-egenskap hvis
	- $R_1 \cup R_2 \rightarrow R_1 \lor R_1 \cap R_2 -> R_2$
- Komponent-tabellenes felles attributter er supernøkkel for en eller begge komponenttabellene
- Ved oppsplitting i flere trinn vil sluttresultatet ha T-J egenskapen hvis alle oppslittings-tinnene har det
- Kan bruke "felles-attributt-regelen" for å sjekke hver dekomponering

### BCNF og kriteriene
- Kan alltid oppnå alle 4 kritierer når vi nøyer oss med tabeller på 3NF
- Ønsker vi å BCNF kan vi måtte velge bort noe:
	- attributtbevaring - kan vi ikke klare oss uten
	- bevaring av funksjonelle avhengigheter - restriksjonene blir mer krevende å gjennomføre
	- tapsløst-join-egenskapen - kan ikke velge bort da får vi et design som generer søppel

## MVD-er, 4NF, avrunding
3NF/BCNF kan ha redundansutfordringer

### Flerverdi-avhengigheter
- Gitt X, Y, Z som er delmenger av R
- En multi-value dependency betyr at mengden Y-verdier som er assosiert med en X-verdi bestemmes av X og er uavhengig av andre attributter

#### Egenskaper
- X ->> Y er triviell hvis Y er en delmengde av X eller $X \cup Y = R$
	- gir ingen restriksjon
- Alle FA-er er MVD-er
- MVDer kommer ofte i par
	- hvis X ->> Y så vil X ->> Z der $Z = R - (X \cup Y)$

### 4NF
- En tabell er på 4NF hvis det for alle ikke-trivielle MVDer, X ->> Y er slik at X er en supernøkkel
- Fagins teorem
	- A, B delmengder av R, C = R-AB
	- projeksjonene AB og AC har tapsløs-join-egenskapen hvis A ->> B eller A ->> C 

