# Käyttöohje

Ohjelma ottaa sisään flow-ongelman, ja ratkaisee sen, tulostaen ensin kuinka paljon flowia siitä pystyy työntämään läpi, ja sen jälkeen jokaiselle edgelle, kuinka paljon sen läpi kulkee ratkaisussa flowia.

Ohjelmalle annetaan syöte seuraavassa muodossa:
```
n m source sink
edge_source edge_target edge_capacity
...
edge_source edge_target edge_capacity
```
Esimerkiksi syöte
```
3 2 0 2
0 1 10
1 2 20
```
Vastaa kolmen noden putkea, missä ensimmäisen edgen kapasiteetti on 10, ja toisen 20. verkon source on ensimmäinen node, ja sink viimeinen.

Kun koodin ajaa tällä syötteellä, saa tuloksen:
```
10
10 10
```
Eli flowin kokonaismäärä on 10, ja sitä kulkee molempien edgejen läpi tämä määrä.

Ohjelman binääri ei ole mukana valmiina. Se kannattaa ajaa ajamalla komento `make run` src-kansiossa. Jos sen haluaa ajaa itse, se pitää ensin kompiloida, jonka voi tehdä komennolla `make`. Tämän jälkeen kompiloitu ohjelma löytyy pathilla `bin/main`. Sen voi ajaa komennolla `./bin/main`.

