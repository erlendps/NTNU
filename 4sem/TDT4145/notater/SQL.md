# Structured Query Language
```SQL
CREATE TABLE /* opprette tabell */
ALTER TABLE /* Endre tabell*/
DROP TABLE /* slette tabell*/
```

## Hundedatabase: Opprette tabell (MySQL)
```SQL
CREATE TABLE Person (
	Pnr INTEGER NOT NULL,
	Navn VARCHAR(30),
	CONSTRAINT Person_PK PRIMARY KEY (Pnr));

CREATE TABLE Hund (
	Regnr INTEGER NOT NULL,
	Navn VARCHAR(30),
	Rase VARCHAR(30),
	Faar INTEGER,
	EierPnr INTEGER NOT NULL,
	CONSTRAINT Hund_PK PRIMARY KEY (Regnr),
	CONSTRAINT Hund_FK FOREIGN KEY (EierPnr) REFERENCES Person(Pnr)
		ON UPDATE CASCADE
		ON DELETE NO ACTION);
		
CREATE TABLE BittAv (
	Pnr INTEGER NOT NULL,
	RegNr INTEGER NOT NULL,
	Antall INTEGER DEFAULT 0,
	CONSTRAINT BittAv_PK PRIMARY KEY (Pnr, Regnr),
	CONSTRAINT BittAv_FK1 FOREIGN KEY (Pnr) REFERENCES Person(Pnr)
		ON UPDATE CASCADE
		ON DELETE CASCADE,
	CONSTRAINT BittAv_FK2 FOREIGN KEY (Regnr) REFERENCES Hund(Regnr)
		ON UPDATE CASCADE
		ON DELETE CASCADE);
```
### Foregin Key restriksjoner
- NO ACTION (default) / RESTRICT : stopper handlingen
- SET NULL : setter NULL-verdi
- CASCADE : oppdaterer / sletter tilsvarende

## Data fra flere tabeller
- Spesifiserer sammenstillingen i FROM-delen:
	- JOIN-typer:
		- JOIN / INNER JOIN
		- NATURAL JOIN
		- LEFT OUTER JOIN, RIGHT OUTER JOIN, JOIN, FULL OUTER JOIN
		- CROSS JOIN
	- JOIN-betingelse
		- NATURAL
		- ON < logisk betingelse >
		- USING  (< attributtliste >)

