Ve městě velikém, jehož sláva hvězd se dotýká, zvolil lid nové konšele. Po předlouhém hašteření a dohadování se o tom, kdo bude držet městskou pokladnu a kdo zas řídit obranu města, se konšelům podařilo zvolit ze svého středu nového purkmistra a nová městská rada se ujala vlády nad městem. Jako první svou pozornost upřela na jeden z nejpalčivějších problému města, dopravu.

Předchozí městské rady zavedly pokrokovou celoměstskou síť veřejných dopravních prostředků, od všemožných povozů a drožek až po lodičky a vory, nazvanou zjednodušeně Povozy I Drožky (zkráceně PID). Součástí tohoto systému mohli obyvatelé napríklad využít i síť sdílených kárek (takzvaná Rekárka). Jenže přes veškeré snahy se zatím nepovedlo zajistit dostatečné množství spojů tak, aby se dalo přepravit mezi libovolnými dvěma místy ve městě. Nová městská rada by chtěla tento nedostatek co nejdříve napravit a svolala na to tedy zvláštní schůzi.

Protože pochopitelně musíme brát ohledy na městskou pokladnu (a tedy i na kapsy konšelů), je potřeba posílit síť PID co nejefektivněji. Každý konšel má právo na sezení vystoupit s návrhem na posílení dopravní sítě pomocí přidání konkrétních tras povozů či jiných dopravních spojení a městská rada následně rozhodne, který návrh bude realizován. Pro to je potřeba umět rozhodnout jak přesně ovlivní přidání daných spojů současnou dopravní síť. Navíc bychom to chtěli zjistit rychle, jelikož konšelé mají taky jiné starosti než jen sedět celý den na schůzi rady.

Tedy předem dostanete mapu současného stavu sítě PID a následně pro každý přeložený návrh musíte rychle spočítat jak přidání daných spojů ovlivní propojenost dopravní sítě. Pro zjednodušení práce (konšelů, ne vaší) je propojenost zcela popsána počtem oblastí. Dvě zastávky jsou v téže oblasti, právě když se mezi nimi lze oběma směry přepravit v rámci sítě PID.
Rozhraní programu

Vaším úkolem je implementovat třídu TrafficNetworkTester. Třída musí mít konstruktor s argumentem typu Map pospisujícím současnou situaci a metodu count_areas(new_connections), která spočte kolik oblastí v dopravní síti bude existovat po přidání spojů popsaných new_connections (formát stejný jako Map::connections, vizte níže, ale navíc může zavádět nové zastávky). Oblastí se myslí maximální množina zastávek taková, že se z každé z nich lze dostat do všech ostatních a zpět.

Objekt třídy Map obsahuje atribut places se seznamem existujících zastávek a atribut connections se seznamem dvojic zastávek, mezi kterými existuje přímé spojení. Spojení jsou jednosměrná, mezi dvěma zastávkami jich může být více a občas může být cílová a počáteční zastávka totožná. Každá zastávka je popsána datovým typem Place. V testovacích datech je Place implementováno pomocí std::string, ale v ostrých testech je užita efektivnější implementace.

V testech je počet volání metody count_areas výrazně vyšší než počet vytvářených instancí. Kopírující konstruktor a operátor přiřazení nejsou třeba.
Bodové podmínky

-    Pro zisk 1 bodů stačí, když metoda count_areas rozhoduje, zda má výsledná dopravní síť právě jednu (pak vrátí 1) nebo více oblastí (pak vrátí libovolné číslo větší než 1). (Časový limit 17 sekund.)

-    Pro zisk 5 bodů musí metoda count_areas správně spočítat počet oblastí. (Časový limit 17 sekund.)

-    Pro zisk 9 bodů musí navíc být metoda count_areas efektivní vzhledem k velikosti výsledné sítě. (Časový limit 17 sekund.)

-    Pro zisk 14 bodů musíte využít toho, že současná síť má (oproti své velikosti) málo oblastí. (Časový limit 5 sekund.)

-    Pro zisk 15 bodů vaše implementace nesmí mít přílišnou hluboku. (Časový limit 0.5 sekundy.)

-    Bonus: Pro zisk až 17.4 bodu musí vaše implementace být rychlejší než nastavený limit. Body jsou škálovány dle dosaženého času. Pro maximum musí výpočet trvat nejvýše polovinu limitu. (Časový limit 2.8 sekundy.)

Na všechny testy dohromady kromě prvního je limit 30 sekund. Selhání v libovolném testu znamená automaticky selhání ve všech následujících.
