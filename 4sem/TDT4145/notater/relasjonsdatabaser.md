# Relasjonsdatabaser
## Struktur
- Fremmednøkkel referer til primærnøkkelen til entieten man har en relasjon til
- Skjema til en tabell (entitetsklasse) viser hvilke kolonner (attributter) entitetsklassen har og hvilke typerestriksjoner man har
- Forekomst av data er "rader" (records)
- NULL gir der man ikke har noen verdi eller er ukjent

## Typer
- Standard relasjonsdatabaser har
	- atomiske verdier i domenene (datatypene)
	- en verdi for hvert attributt (i en rad)
	- Dette gir oss "flate, 2-dimensjonale tabeller"
- Ikke-standard relasjonsdatabaser kan ha
	- sammensatte attributter (poster, tabeller), repeterende grupper
- Entitetsintegritet
	- En relasjon (tabell) er en mengde tuppler
		- Kan ikke ha to like tuppler (rader)
	- Alle tabeller må ha en primærnøkkel
		- For å sikre at primærnøkkelen har unike verdier tillates ikke NULL-verdier i primærnøkkelen
- Referanseintegritet
	- Fremmednøkler må referer til et tupper (rad) som finnes i den tabellen det referes til eller bare bestå av NULL-verdi(er)

## Mapping fra ER-diagram til relasjonsdatabaser
### Regulære entitetsklasser
En tabell for entitetsklassen med entitetsklassens nøkkelatrributt og en-verdi-attributter. For sammensatte attributter beholdes "løv-attributtene"
- Entitetsklassen får en egen tabell
	- enkle attributter som i ER-modellen
	- Sammensatte attributter splittes opp
	- primærnøkkel som i ER-modell
### Svake entitetsklasser
En tabell for entitetsklassen med entitetsklassens delvise nøkkelattributt og en-verdi-attributter. I tillegg må nøkkelattributten(e) til den identifiserende entitetsklassen tas med som nøkkelattributter. Dette vil være en fremmednøkkel mot den aktuelle tabellen som ikke kan ha NULL-verdier. 
- Vi legger til primærnøkkel fra identifiserende entitetsklasse som utgjør primærnøkkel for den svake klassen sammen med klassens delvise nøkkel
- Primærnøkkel fra identifiserende entitetklasser er fremmednøkkel mot tabellen for "egen klasse"

Legg merke til at det er som for en regulær entitetsklasse, unntatt håndteringen av primærnøkkel.

### Binære 1:1 relasjonsklasser
- Legger til en fremmednøkkel i en av tabellene eller lager en egen koblingstabell
- Velger ofte den løsningen som gir minst NULL-verdier

### Binære 1:n relasjonsklasser
Relativt likt for 1:1, men hvis man ikke lager egen koblingstabell kan man bare legge fremmednøkkel i "mange" klassen.

Kan legge til fremmednøkkel og evt. attributter som hører til relasjonsklassen, i tabellen som representerer mange-siden i relasjonsklassen. Alternativt kan det legges til en koblingstabell

### Binære N:M relasjonsklasser
Må lage koblingstabell

Må ha en egen koblingstabell, med fremmednøkler til begge entitetsklasse-tabellene. Fremmednøklene utgjør sammen primærnøkkelen. Evt. attributter i relasjonsklassen tas med i koblingstabellen.

### Fler-verdi attributter
Fler-verdiattributt må (vanligvis) håndteres i en egen tabell, med en fremmednøkkel til entitetstabellen.

Det kan være flere muligheter når det gjelder nøkkel i denne fler-verdi-attributt-tabellen.

### N-ære relasjonsklasser
1. Lager en tabell for relasjonsklassen med fremmednøkler til tabellene som representerer de deltakende entietsklassene
2. Attributter på relasjonsklassen blir attributter i "koblingstabellen".

### Spesialisering/Generalisering
- Ingen egentlig støtte for dette
- 4 opsjoner
	- både superklasse-tabell og subklasse-tabell
	- bare subklasse-tabeller (best med total og disjunkt spesialisering)
	- bare superklassetabell (med attributter til subklassene)

### Kategorier
- Lager egen tabell for kategori-entitetsklassen (vanlige regler, untatt primærnøkkel)
- Superklassene har som regel ulike nøkler, lager i disse tilfellene en spesielle primærnøkkel for kategori tabellen (surrogatnøkkel)
- legger surrogatnøkkelen inn som fremmednøkkel i alle superklasse-tabeller
- legger inn et type-attributt i kategori-tabellen som viser hvilken superklasse hver kategori-entitet er.

### oppg1
DigitalKamera(<ins>Modell-ID</ins>, Navn, Type, LansertÅr, oppløsning_bredde, oppløsning_høyde, sensor_høyde, sensor_bredde, <ins>Produsent.navn</ins>)

FilmKamera(<ins>Modell-ID</ins>, Navn, Type, LansertÅr, <ins>Filmformat.navn</ins>, <ins>Produsent.navn</ins>)

Filmformat(<ins>Navn</ins>, høyde, bredde)

Produsent(<ins>Navn</ins>)

Filformat(<ins>ID</ins>, navn, beskrivelse)

LagrerBilderI(<ins>DigitalKamera.Modell-ID</ins>, <ins>Filformat.ID</ins>)

## Relasjonsalgebra
- Operator for å manipulere tabeller
- operatorene er lukket over tabeller
	- operanden(e) er tabell(er), resultatet er en ny tabell
- mengdeoperatorer
	- union, snitt mengdedifferanse, kartesisk produkt
- spesielle operatorerer
	- seleksjon, projeksjon, join-operator

### Projeksjon
- Resultat-tabellen får nytt skjema
	- består av kolonnene i attributtlisten
- fjerner duplikater i resultat-tabellen

### Seleksjon
- Velger ut rader som oppfyller logisk betingelse
	- sammenligningsoperator + logiske konnektiver + parenteser
- Resultattabellen har samme skjema som operand-tabellen

### Kombinasjon av operatorer
- må passe på rekkefølgen av operatorene
- pass på ikke å fjerne data vi trenger senere

### mengdeoperatorene
- arbeider på data fra to operand-tabeller
- kun mellom union-kompatible tabeller:
	- samme grad - det vil si like mange kolonner
	- korresponderende kolonner er definert over samme datatype

### Kartesisk produkt
- kombinerer alle rader i den ene tabellen med alle rader i den andre tabellen
- resultattabellen får alle kolonner fra den første tabellen og alle kolonner fra den andre
	- ved behov kan vi prefiske med tabellnavn: Hund.navn
- To operand-tabeller med heholdsvis r og s rader, vil gi resultattabell med r*s rader
	- gir fort store resultat-tabeller

### JOIN - kritisk sammenstilling
- kartesisk produkt er ukritisk sammenstilling
	- alle reader kombineres med alle rader
- join-operatorer kombinerer relaterte rader
	- radene i resultat-tabellen oppfyller en join-betingeøse
- join-operatoren kan implementeres mye billigere enn kartesisk produkt
- EQUIJOIN - basert på likhet, vi kan også ha generaliserte join-betingelser

### Equijoin
- kombinerer rader basert på likhet
	- ett eller flere par av kolonner (fra hver sin operandtabell)
- Filtrerer vekk rader som ikke matcher
- Alltid ett eller flere par kolonner med like verdier

### Naturlig join
- Som EQUIJOIN, men
	- \* som operatorsymbol
	- implisitt join-getingelse
		- likhet i alle par av kolonner med like navn
	- fjerner alle duplikatkolonner
- Tilsvarer
	- kartesisk produkt + seleksjon + projekson
- Risiko
	- kan bli en "større" join-betingelse enn man ønsker
	- ved endringer i operand-tabellene kan join-betingelsen endre seg

### Omdøping og sortering
- Omdøpingoperator
	- p (rho)
	- nye navn på tabell og kolonner
- sorteringsoperator
	- stigende ASC
	- synkende DESC

### OUTER JOIN
- inner join filtrerer vekk rader. vanlig equijoin
- outer-join får med alle rader i en eller begge operand-tabellene
	- left outer join: alle rader fra venstre operanden
	- right outer join: alle rader fra høyre operanden
	- full outer join: alle rader fra begge operandene
- Filtrerer ikke bort rader slik som inner-join-operatorer
- rader som amgnler "matchende" rad i den andre tabellen
	- kombineres med en rad der alle attributter er satt til null

### Aggregering / gruppering
- Aggregeringsfunksjoner
	- SUM(attributt), AVG(), MIN(), MAX(),...
	- COUNT()
		- count(attributt) - antall verdier, unntatt NULL
		- count(distinct attributt) - antall unike verdier, untatt NULL
		- count(*) - antall rader
- Grupperinger
	- grupperingsattributtene definerer en partisjonering av radene i tabellforekomsten
	- aggregeringen gjøres over radene i hver partisjon som hver bidrar til en rad i resultattabellen
	- uten grupperingsattributter aggregeres det over alle radene i tabellen

### Relasjonsalgebra kan uttryke restriksjoner
- Entitetsintegritet
- Referanseintegritet
- Maks 10 offer for en hund f.eks