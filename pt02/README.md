Konšelé díky Vaší pomoci posílili síť PID tak, že se již nyní dá pohodlně přepravit mezi libovolnými dvěma místy ve městě. Nicméně jako vedlejší efekt této situace jezdí po městě neúnosné množství povozů i drožek, navíc často nebezpečnou rychlostí kvůli neúprosně optimalizovaným jízdním řádům (kdoví jestli to také není vaše práce).To pochopitelně dráždí velkou část měšťanů, kterým vadí neustalý hluk koňského ržání, práskaní bičů a mají strach o svoji bezpečnost, kdykoliv jen vyrazí ven na ulici.

Zato konšelé jsou se současnou situací nad míru spokojení, jelikož optimalizovaný provoz PID jim přináší zlaťáky do městské pokladny i vlastní kapsy, a veškeré stížnosti se jim doposud dařilo zametat pod stůl. Jenže nyní se k nim doneslo, že skupina neodbytných měšťanů chystá radikálnější akci. Plánují protestní pochod městem s požadavkem celoplošného omezení rychlosti na rychlost pomalého koňského klusu. Toho se konšelé zalekli, jelikož hrozí kolaps PID a celé dopravy ve městě.

Najali si tedy opět vás, abyste vypracovali analýzu dopadu takového protestu v závislosti na jeho velikosti. Pro každou ulici je jednoduché zjistit, kolik měšťanů je potřeba na zablokování průjezdu. Konšely ale zajímá kolik celkem měšťanů by se muselo alespoň sejít tak, aby způsobili úplný dopravní kolaps. Dopravním kolapsem se myslí situace, kdy ve městě vzniknou dvě místa, mezi nimiž se nepůjde dopravit žádným způsobem. Navíc by chtěli vědět, kde se nachází kritické ulice, které takový dopravní kolaps způsobí, tak aby byla městská stráž připravená podobný protest rozehnat.

Vy tedy obdržíte mapu města, kde pro každou ulici máte napsáno kolik měšťanů způsobí její zablokování, a musíte spočítat kolik nejméně protestujících měšťanů je potřeba na způsobení kolapsu dopravní sítě a zároveň, kde se nachází kritické ulice, jejichž zablokování tento kolaps způsobí. Koncept jednosměrné ulice je pochopitelně výmysl moderní doby, a tedy můžete považovat každou ulici ve městě (jakkoli úzkou) za průjezdnou oběma směry.
Rozhraní programu

Vaším úkolem je implementovat funkci std::pair<unsigned, std::set<Place>> critical_streets(const Map& map), která vrátí počet měšťanů nutných k zablokování dopravy a seznam míst, která skončí na jedné straně blokády.

Objekt třídy Map obsahuje atribut places se seznamem existujících míst a atribut connections se seznamem trojic místo, místo, kapacita, který popisuje ulice a počet měšťanů nutných k jejich zablokování. Ulice jsou obousměrné, mezi dvěma místy jich může být více a občas může být cílové a počáteční místo totožné. Každé místo je popsána datovým typem Place. V testovacích datech je Place implementováno pomocí std::string, ale v ostrých testech je užita efektivnější implementace.

Ve všech testech krom bonusového jsou kapcity ulic velmi malé.
Bodové podmínky

-    Pro zisk 1 bodu stačí, když funkce critical_streets, vrátí správný počet měšťanů, seznam míst není kontrolován. (Časový limit 17 sekund.)

-    Pro zisk 5 bodů musí funkce critical_streets navíc správně zjistit blokovaná místa. (Časový limit 17 sekund.)

-    Pro zisk 10 bodů musí navíc být funkce critical_streets rozumně asymptoticky efektivní. (Časový limit 5 sekund.)

-    Pro zisk 15 bodů musí navíc být funkce critical_streets efektivní. (Časový limit 10 sekund.)

-    Bonus: Pro zisk až 17.4 bodu musí vaše implementace být rychlá i pro mapy s mnoha ulicemi o velkých kapacitách. (Časový limit 10 sekund.)

Na všechny testy dohromady kromě prvního je limit 30 sekund. Selhání v libovolném testu znamená automaticky selhání ve všech následujících.
