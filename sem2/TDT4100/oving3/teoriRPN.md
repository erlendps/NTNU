# Oppg 1
Jeg ser det naturlig å bruke et unntak av type <code>IllegalArgumentException</code> fordi vi
bruker et argument som ikke er lovlig gitt tilstanden til objektet. 
Fordelen med å bruke en exception er at brukeren kan få vite hva som gikk galt, og hva som evt
må rettes opp i. Ulempen med dette er at det kan stanse programmet. Man kan evt da sette opp 
en try/catch blokk.

# Oppg 2
Jeg synes det er naturlig at for multiplikasjon så er 1 en naturlig verdi. For divisjon
mener jeg at nevneren skal være 1, slik at man sitter igjen med samme stack.

Jeg tror jeg ville ha laget en hovedklasse som bestemmer på hvilken operasjon og hvile 
operander man skal utføre operasjonen på, og å ha enten flere hjelpemetoder innad i klassen
eller egne klasser som er designet kun for en operasjon. F.eks kunne man hatt en separat
metode for '+'-operasjonen osv. 

Det kan være lurt å løse ut et unntak, da brukeren kan få mer informasjon om hva som gikk galt.