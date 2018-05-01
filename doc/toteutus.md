#Toteutusdokumentti

Ohjelma koostuu kolmesta osasta:

- Splay Tree, joka on tasapainotettu puurakenne
- Link/Cut Tree, muuttuvaa metsää esittävä ja siitä queryitä tukeva rakenne.
- Dinic, maximum flow ongelman ratkaiseva algoritmi.

Link/Cut Tree käyttää sisäisesti Splay-puita, koska ne amortisoituvat samalla lailla.
Dinic:ssä käytetään Link/Cut treetä, jotta pitkiä polkuja ei tarvitsisi kulkea kokonaan läpi aina kun jokin edge reitillä saturoituu.

Splay-puu tukee yleisiä puurakenneoperaatioita amortisoidussa `O(log n)` ajassa, ja käyttää `O(n)` muistia.
Link/Cut treen operaatiot toimivat amortisoidussa `O(log n)` ajassa käyttäen `O(n)` muistia
Dinic:n aikavaativus link/cut treetä apuna käyttäessä on `O(VE log V)`, ja muuten `O(V^2 E)`, ja muistinkäyttö lineaarinen `O(V + E)`

Käytännössä Dinic toimii lähes lineaarisessa ajassa. Link/Cut treetä on hidasta käyttää sen isojen vakiokertoimien takia etsiessä lyhyitä polkuja, joten käyttämällä Link/Cut treetä pitkiä polkuja etsiessä saadaan käytännössä ja asymptioottisesti nopea koodi.

Työhön ei nykyisellään ole oikein mitään lisättävää. Sen vakiokertoimia pystyisi kyllä varmasti vielä viilaamaan.

Lähteet:
[Dinic](https://en.wikipedia.org/wiki/Maximum_flow_problem#Solutions)
[Link/Cut tree](https://en.wikipedia.org/wiki/Link/cut_tree)
[Splay tree](https://en.wikipedia.org/wiki/Splay_tree)
