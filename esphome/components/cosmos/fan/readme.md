- immer "forward"


bedeutet, set_hbridge_levels_(X, 1.0f)


- Stop offset als Configurationsvariable einfügen

50 - Stop_Offset = Break


Beispiel:
               | Von | Bis | Default  | Verschiebung
------------------------------------------------
Break Level    |  53 |  59 | 47 -  53 | 6
Rein / Forward |   0 |  52 |  0 -  46 | 6
Raus / Reverse |  60 | 100 | 54 - 100 | 6


Break: 50 + Verschiebung / 2
50% Forward Speed: (47 + Verschiebung) * 50% = 26,5
50% Reverse Speed: (53 + Verschiebung) * 50% * 100% = 88,5
0% Forward Speed: (47 + Verschiebung) * 0% = 0
0% Reverse Speed: (53 + Verschiebung) * 0% * 100% = 0



# todo
- [ ] Max Forward, Min Reverse, Brake konfigurierbar machen
- [x] Decay Mode ausbauen
- [ ] Default Wert auf break setzen ?