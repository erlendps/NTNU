"NP er klassen av ja-nei-problemer der ethvert ja-svar har et bevis som kan
sjekkes i polynomisk tid"

"Alt i NP kan reduseres i polynomisk tid til et komplett problem.
NP kan ikke løses i ploynomisk tid med mindre alle i NP kan det"


### Reduksjon

* Ønsker å beskrive problemets vanskelighetsgrad
* Absolutt vanskegrad: Mulig, men mest teoretisk
* Sammenligner derfor problemer i stedet

Å redusere betyr at man kan løse A ved hjelp av B. Da reduserer vi A til B.
Samtidig er: $A \leq B$
* Spør om det er "minst like vanskelig".
* I vårt tilfelle bruker vi polynomiske redusjoner
* Altså handler det om probleme kna løses i polynomisk tid
    * Om B kan løses i polynomisk tid, kan også A det.

```
Has-Short-Path(G, u, v, k):
    BFS(G, u)
    return v.d <= k
```

* Om A kan reduserers til et løsbart problem, må også A være løsbart.
* Hvis løsningen B løses i polynomisk tid, kan også A det.

#### Hamilton sykel

* Vi kan reduer HAM-SYKEL til HAS-LONG-PATH
* Om vi finner en løsning på HAS-LONG-PATH har vi også funnet en løsning
på hamilton problemet

### Verifikasjon

* Ønsker et "univers"
* Velger problemer der en løsning kan verifiseres i polynomisk tid
* Klassen kalles NP for "nondeterministically polynomial"

Det er like lett å verifisere at det fines en kort vei som en lang vei.
* De fleste problemer av pratktisk betyding er av denne typen
* Co-NP: det samme, men man ønsker et sertifikat for nei-svar

### Kompletthet

* Vi har et univers av problemer og vi kan sammenligne dem
* Dette gir ipphav til et sett maksimalt vanskelige problemer
* Disse kalles komplette for klasssen NP under polynomiske reduksjoner.

Om man klarer å løse et komplett NP-problem klarer man også å løse alle andre problemer
i NP.

### Refleksjoner

* Reduser alltid fra et komplett NP-problem.
