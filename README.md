# Tiralabra-projekti: Dinic ja Link/Cut tree

Projektissa on toteutettu Dinic'n algoritmi maximum flow -ongelman ratkaisemiseen aikavaativuudella `O(VE log V)`. Tätä varten on toteutettu link/cut tree ja splay tree -tietorakenteet.

## dokumentaatio
[Määrittelydokumentti](https://github.com/anroysko/tiralabra/blob/master/doc/maarittely.md)

[Käyttöohje](https://github.com/anroysko/tiralabra/blob/master/doc/ohje.md)

[Testausdokumentti](https://github.com/anroysko/tiralabra/blob/master/doc/testaus.md)

[Toteutusdokumentti](https://github.com/anroysko/tiralabra/blob/master/doc/toteutus.md)
### viikkoreportit
* [Viikko 1](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week1.md)
* [Viikko 2](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week2.md)
* [Viikko 3](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week3.md)
* [Viikko 4](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week4.md)

## Komentorivitoiminnot
Testikattavuusreportin generointiin tarvitaan ohjelma `lcov`
### Testaus
Testit suoritetaan komennolla
```
make test
```
Performanssitestit suoritetaan komennolla
```
make performance
```
Testikattavuusraportti luodaan komennolla
```
make coverage
```
Kattavuusraporttia voi tarkastella avaamalla selaimella tiedosto `src/bin/cov/html/index.html`
### Ohjelman ajaminen
Ohjelman voi ajaa komennolla
```
make run
```
### Koodihakemiston puhdistaminen
src-folderin voi puhdistaa kompiloitaessa luotavista tiedostoista komennolla
```
make clean
```
