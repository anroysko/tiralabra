# Tiralabra-projekti: Dinic ja Link/Cut tree
## dokumentaatio
[Määrittelydokumentti](https://github.com/anroysko/tiralabra/blob/master/doc/maarittely.md)
### viikkoreportit
* [Viikko 1](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week1.md)
* [Viikko 2](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week2.md)
* [Viikko 3](https://github.com/anroysko/tiralabra/blob/master/doc/weekly_reports/week3.md)

## Komentorivitoiminnot
Testikattavuusreportin generointiin tarvitaan ohjelma `lcov`
### Testaus
Testit suoritetaan komennolla
```
make test
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
