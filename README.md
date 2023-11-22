# Navodial
Naloga zahteva izdelavo pretvornika v poljubnem programskem jeziku, kot so C#, .NET, Vue.JS, ki bo prebral JSON datoteko in jo pretvoril v veljavno HTML datoteko.

Pomembno je, da je program napisan dovolj splošno, da deluje za različne JSON datoteke, brez zapečatene kode. Poudarek je na arhitekturi, eleganci kode in skladnosti s principi objektno orientiranega programiranja (OOP). 

Implementacija naj predpostavlja, da so vhodne in izhodne datoteke v isti mapi kot program, in ni potrebno implementirati izbire datotek ali uporabniškega vmesnika. Indentacije so zaželjene, vendar neobvezne. 

Za interpretacijo JSON datoteke se lahko uporabi poljubna knjižnica, medtem ko je pretvorba v HTML zaželjeno, da se implementira samostojno. Pripete so tudi primerne JSON in HTML datoteke za preverjanje pravilnosti rešitve.

## Build instructions
`mkdir build`
`cd build`
`cmake ..`
`make`
Run with:
`./run`
