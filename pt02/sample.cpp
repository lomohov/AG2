#ifndef __PROGTEST__

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

using Place = std::string;
using Capacity = unsigned;
using Connection = std::tuple<Place, Place, Capacity>;
struct Map {
    std::vector<Place> places;
    std::vector<Connection> connections;
};

#endif

class CCapacitySolver {
public:
    explicit CCapacitySolver(const Map &map) :
            m_VertexCount(map.places.size()),
            m_Graph(m_VertexCount, std::vector(m_VertexCount, 0u)),
            m_Flows(m_VertexCount, std::vector<Capacity>(m_VertexCount, 0u)),
            m_Neighbours(m_VertexCount) {
        m_Mapper.reserve(m_VertexCount);
        for (size_t i = 0; i < m_VertexCount; ++i) {
            m_Mapper[map.places[i]] = i;
        }
        for (const auto &[from, to, capacity]: map.connections) {
            auto fromIndex = m_Mapper[from];
            auto toIndex = m_Mapper[to];
            if (fromIndex == toIndex) continue;
            if (!m_Graph[fromIndex][toIndex]) {
                m_Neighbours[fromIndex].emplace_back(toIndex), m_Neighbours[toIndex].emplace_back(fromIndex);
            }
            m_Graph[fromIndex][toIndex] += capacity, m_Graph[toIndex][fromIndex] += capacity;
        }
    }

    Capacity bfs(uint_fast32_t, uint_fast32_t, std::vector<uint_fast32_t> &) const;

    Capacity fordFulkerson(uint_fast32_t source_index, uint_fast32_t target_index);

    std::pair<Capacity, std::set<Place>> placeEcoTerrorists(const Map &map);

    std::set<Place> getPlaces(const Map &map) const;

private:
    uint_fast32_t m_VertexCount;
    std::unordered_map<Place, uint_fast32_t> m_Mapper;
    std::vector<std::vector<Capacity>> m_Graph;
    std::vector<std::vector<Capacity>> m_Flows;
    std::vector<std::vector<uint_fast32_t>> m_Neighbours;
};

Capacity
CCapacitySolver::bfs(uint_fast32_t sourceIndex, uint_fast32_t targetIndex, std::vector<uint_fast32_t> &parent) const {
    std::fill(parent.begin(), parent.end(), -2);
    std::queue<uint_fast32_t> q;
    q.emplace(sourceIndex);
    parent[sourceIndex] = (uint_fast32_t) -1;
    while (!q.empty()) {
        uint_fast32_t u = q.front();
        q.pop();
        for (const auto &v: m_Neighbours[u]) {
            if (!m_Graph[u][v] || parent[v] != (uint_fast32_t) -2) continue;
            auto reserve = m_Graph[u][v] - m_Flows[u][v] + m_Flows[v][u];
            if (reserve > 0) {
                parent[v] = u;
                if (v == targetIndex) {
                    auto path_flow = UINT32_MAX;
                    for (uint_fast32_t vertex_v = targetIndex, vertex_u = parent[vertex_v];
                         vertex_v != sourceIndex; vertex_v = vertex_u, vertex_u = parent[vertex_u]) {
                        auto vertex_reserve =
                                m_Graph[vertex_u][vertex_v] - m_Flows[vertex_u][vertex_v] + m_Flows[vertex_v][vertex_u];
                        path_flow = std::min(path_flow, vertex_reserve);
                    }
                    return path_flow;
                }
                q.push(v);
            }
        }
    }
    return 0;
}

Capacity CCapacitySolver::fordFulkerson(uint_fast32_t source_index, uint_fast32_t target_index) {
    Capacity flow = 0;
    std::vector<uint_fast32_t> parent(m_Graph.size(), -1);
    while (auto path_flow = bfs(source_index, target_index, parent)) {
        for (uint_fast32_t v = target_index; v != source_index; v = parent[v]) {
            uint_fast32_t u = parent[v];
            if (m_Flows[v][u] >= path_flow) {
                m_Flows[v][u] -= path_flow;
            } else {
                auto remainder = path_flow - m_Flows[v][u];
                m_Flows[v][u] = 0;
                m_Flows[u][v] += remainder;
            }
        }
        flow += path_flow;
    }
    return flow;
}

std::set<Place> CCapacitySolver::getPlaces(const Map &map) const {
    std::set<Place> places;
    std::queue<uint_fast32_t> q;
    std::vector<bool> visited(m_VertexCount, false);
    q.emplace(0);
    places.emplace(map.places[0]);
    while (!q.empty()) {
        auto u = q.front();
        q.pop();
        for (const auto &v: m_Neighbours[u]) {
            if (!m_Graph[u][v] || visited[v]) continue;
            auto reserve = m_Graph[u][v] + m_Flows[v][u] - m_Flows[u][v];
            if (reserve > 0) {
                q.push(v);
                visited[v] = true;
                places.emplace(map.places[v]);
            }
        }
    }
    return places;
}

std::pair<Capacity, std::set<Place>> CCapacitySolver::placeEcoTerrorists(const Map &map) {
    auto max_flow = (Capacity) -1;
    uint_fast32_t current_max_target = 0;
    for (uint_fast32_t target_index = 1; target_index < m_VertexCount; ++target_index) {
        auto flow = fordFulkerson(0, target_index);
        std::fill(m_Flows.begin(), m_Flows.end(), std::vector<Capacity>(m_Flows.size(), 0u));
        if (max_flow > flow) {
            current_max_target = target_index;
            max_flow = flow;
            if (!flow) break;
        }
    }

    fordFulkerson(0, current_max_target);
    std::set<Place> places = getPlaces(map);
    return {max_flow, places};
}

std::pair<Capacity, std::set<Place>> critical_streets(const Map &map) {
    CCapacitySolver solver(map);
    return solver.placeEcoTerrorists(map); 
}

#ifndef __PROGTEST__

using Test = std::pair<unsigned, Map>;

std::array TESTS = {
        Test{3, {{
                         "Dejvicka", "Hradcanska"
                 }, {
                         {"Dejvicka", "Dejvicka", 3}, {"Hradcanska", "Dejvicka", 3}
                 }}},
        Test{2, {{
                         "Dejvicka", "Hradcanska", "Malostranska", "Staromestska", "Mustek", "Muzeum"
                 }, {
                         {"Dejvicka", "Hradcanska", 3}, {"Hradcanska", "Malostranska", 2},
                         {"Malostranska", "Staromestska", 4}, {"Staromestska", "Mustek", 5},
                         {"Mustek", "Muzeum", 3}
                 }}},
        Test{13, {{
                          "Na Sklonku", "Poliklinika Čumpelíkova", "Šumavská", "Nové Podolí",
                          "Vozovna Střešovice (Muzeum MHD)",
                          "Florenc", "Franty Kocourka", "Cukrovar Čakovice", "Praha-Dejvice", "Pod Říhákem",
                          "Sukovská", "Novoborská", "U Průhonu", "Nádraží Modřany",
                  }, {
                          {"Pod Říhákem", "Novoborská", 3}, {"Nové Podolí", "Franty Kocourka", 4},
                          {"Cukrovar Čakovice", "Florenc", 10},
                          {"Vozovna Střešovice (Muzeum MHD)", "Cukrovar Čakovice", 5},
                          {"U Průhonu", "Praha-Dejvice", 8}, {"Sukovská", "Nové Podolí", 10},
                          {"Poliklinika Čumpelíkova", "Nové Podolí", 5}, {"Florenc", "Na Sklonku", 5},
                          {"Praha-Dejvice", "Cukrovar Čakovice", 6}, {"Franty Kocourka", "Šumavská", 10},
                          {"Florenc", "Vozovna Střešovice (Muzeum MHD)", 6}, {"Pod Říhákem", "Florenc", 3},
                          {"Novoborská", "Šumavská", 8}, {"Sukovská", "Šumavská", 8},
                          {"Nové Podolí", "Pod Říhákem", 10}, {"Vozovna Střešovice (Muzeum MHD)", "Na Sklonku", 4},
                          {"Cukrovar Čakovice", "Nádraží Modřany", 9},
                          {"Vozovna Střešovice (Muzeum MHD)", "Šumavská", 5},
                          {"Novoborská", "Cukrovar Čakovice", 3}, {"Šumavská", "Na Sklonku", 5},
                          {"Poliklinika Čumpelíkova", "Vozovna Střešovice (Muzeum MHD)", 5},
                          {"Nové Podolí", "Na Sklonku", 7},
                          {"Pod Říhákem", "U Průhonu", 5}, {"Šumavská", "Sukovská", 7},
                          {"Šumavská", "Nádraží Modřany", 4}, {"Nové Podolí", "Šumavská", 10},
                          {"Poliklinika Čumpelíkova", "Šumavská", 9}, {"Šumavská", "Florenc", 3},
                          {"Franty Kocourka", "Nové Podolí", 3}, {"U Průhonu", "Vozovna Střešovice (Muzeum MHD)", 5},
                          {"Praha-Dejvice", "Florenc", 3}, {"Nové Podolí", "Praha-Dejvice", 1},
                          {"Pod Říhákem", "Florenc", 4},
                  }}},
        Test{5, {{
                         "U Vojenské nemocnice", "Kuchyňka", "V Korytech", "Kelerka", "Vozovna Strašnice",
                         "Geologická", "U Studánky", "U Jahodnice", "Hadovka", "Barrandovská",
                         "K Netlukám", "Obchodní centrum Sárská", "Praha-Smíchov", "Sušická", "Moráň",
                         "Praha-Bubny", "Rajská zahrada", "Strossmayerovo náměstí", "Průmstav",
                 }, {
                         {"U Vojenské nemocnice", "Vozovna Strašnice", 10},
                         {"K Netlukám", "Obchodní centrum Sárská", 6},
                         {"Praha-Smíchov", "U Jahodnice", 8}, {"Praha-Smíchov", "K Netlukám", 7},
                         {"Vozovna Strašnice", "Kelerka", 5}, {"Obchodní centrum Sárská", "Geologická", 1},
                         {"K Netlukám", "Praha-Smíchov", 3}, {"V Korytech", "Geologická", 9},
                         {"V Korytech", "Vozovna Strašnice", 1}, {"Vozovna Strašnice", "V Korytech", 8},
                         {"U Vojenské nemocnice", "Kuchyňka", 6}, {"Kelerka", "Geologická", 2},
                         {"Praha-Bubny", "Strossmayerovo náměstí", 4}, {"Kuchyňka", "V Korytech", 3},
                         {"Praha-Smíchov", "Praha-Bubny", 9}, {"Obchodní centrum Sárská", "Moráň", 1},
                         {"Kelerka", "V Korytech", 10}, {"Kelerka", "V Korytech", 5},
                         {"Hadovka", "Rajská zahrada", 6}, {"V Korytech", "Geologická", 7},
                         {"Sušická", "Praha-Smíchov", 2}, {"Barrandovská", "K Netlukám", 6},
                         {"V Korytech", "Kelerka", 8}, {"K Netlukám", "V Korytech", 6},
                         {"U Studánky", "Kuchyňka", 5}, {"Hadovka", "Barrandovská", 6},
                         {"Praha-Bubny", "U Studánky", 4}, {"Moráň", "K Netlukám", 4},
                         {"Strossmayerovo náměstí", "Kelerka", 7}, {"Barrandovská", "U Jahodnice", 4},
                         {"V Korytech", "Kuchyňka", 5}, {"Průmstav", "Praha-Smíchov", 5},
                         {"Geologická", "V Korytech", 7}, {"Rajská zahrada", "Kuchyňka", 7},
                         {"U Jahodnice", "Kuchyňka", 1}, {"Praha-Smíchov", "Sušická", 8},
                         {"K Netlukám", "Obchodní centrum Sárská", 3}, {"Geologická", "Kelerka", 4},
                         {"Obchodní centrum Sárská", "K Netlukám", 2}, {"Obchodní centrum Sárská", "K Netlukám", 6},
                         {"Hadovka", "U Studánky", 10}, {"K Netlukám", "Sušická", 4},
                         {"Moráň", "U Vojenské nemocnice", 2}, {"Obchodní centrum Sárská", "Praha-Smíchov", 3},
                         {"V Korytech", "U Studánky", 10}, {"Kuchyňka", "Geologická", 6},
                         {"K Netlukám", "Moráň", 4}, {"Sušická", "U Vojenské nemocnice", 5},
                         {"Kuchyňka", "U Vojenské nemocnice", 8},
                 }}},
        Test{0, {{
                         "Na Lukách", "Plánická", "U Mezníku", "Bílá Hora", "Psohlavců",
                         "Koupaliště Čakovice", "Volha", "Dolnopočernický hřbitov", "Studentský dům", "U Rozcestí",
                         "Koleje Jižní Město", "Kusá", "Vozovna Pankrác", "Ke Koulce", "Opatov",
                         "Nádraží Žvahov - V Násypu", "Na Blanici", "Nádraží Libeň", "Ořechovka", "Ke Kateřinkám",
                         "Divadlo pod Palmovkou", "Přístav Radotín", "Žákovská", "Pankrác",
                 }, {
                         {"Koleje Jižní Město", "U Rozcestí", 7}, {"Na Lukách", "Psohlavců", 7},
                         {"U Rozcestí", "U Mezníku", 5}, {"Ke Koulce", "Kusá", 1},
                         {"Nádraží Žvahov - V Násypu", "Nádraží Libeň", 1}, {"Koleje Jižní Město", "U Rozcestí", 1},
                         {"Koupaliště Čakovice", "Psohlavců", 1}, {"Žákovská", "Pankrác", 7},
                         {"Kusá", "U Rozcestí", 10}, {"Psohlavců", "Bílá Hora", 8},
                         {"Volha", "Na Lukách", 4}, {"Na Lukách", "Plánická", 1},
                         {"Vozovna Pankrác", "Opatov", 7}, {"Koupaliště Čakovice", "Bílá Hora", 1},
                         {"Ořechovka", "Na Blanici", 10}, {"Psohlavců", "U Mezníku", 1},
                         {"U Mezníku", "Bílá Hora", 8}, {"Divadlo pod Palmovkou", "U Rozcestí", 10},
                         {"Vozovna Pankrác", "U Mezníku", 7}, {"Psohlavců", "Koupaliště Čakovice", 2},
                         {"Na Blanici", "Nádraží Žvahov - V Násypu", 2}, {"Na Lukách", "Bílá Hora", 10},
                         {"Kusá", "Koleje Jižní Město", 10}, {"Nádraží Libeň", "Ke Koulce", 2},
                         {"Opatov", "Ke Koulce", 10}, {"Přístav Radotín", "Koupaliště Čakovice", 8},
                         {"Kusá", "Koleje Jižní Město", 5}, {"Pankrác", "Žákovská", 2},
                         {"Vozovna Pankrác", "Koleje Jižní Město", 6}, {"Plánická", "Na Lukách", 10},
                         {"Bílá Hora", "Koupaliště Čakovice", 10}, {"Kusá", "Vozovna Pankrác", 7},
                         {"Koupaliště Čakovice", "Bílá Hora", 8}, {"Psohlavců", "Koleje Jižní Město", 5},
                         {"Bílá Hora", "U Mezníku", 6}, {"Psohlavců", "Bílá Hora", 4},
                         {"Koleje Jižní Město", "Kusá", 1}, {"Přístav Radotín", "Opatov", 5},
                         {"Plánická", "U Mezníku", 3}, {"Vozovna Pankrác", "Nádraží Žvahov - V Násypu", 1},
                         {"Koupaliště Čakovice", "Bílá Hora", 9}, {"Nádraží Žvahov - V Násypu", "Opatov", 10},
                         {"Vozovna Pankrác", "Opatov", 9}, {"Studentský dům", "Volha", 1},
                         {"Žákovská", "Pankrác", 1}, {"U Mezníku", "Na Lukách", 9},
                         {"Ke Kateřinkám", "Koleje Jižní Město", 6}, {"U Mezníku", "Plánická", 9},
                         {"Opatov", "Na Blanici", 9}, {"U Mezníku", "Volha", 8},
                         {"Kusá", "Psohlavců", 2}, {"Kusá", "Ke Koulce", 2},
                         {"Dolnopočernický hřbitov", "Bílá Hora", 4},
                 }}},
};

template<typename C>
void test(C &&tests) {
    int fail = 0, ok = 0, test_number = 0;

    for (auto &&[ref_s, map]: tests) {
        auto [s, _] = critical_streets(map);
        if (s == ref_s) ok++;
        else {
            fail++;
            std::cout << test_number << ". Got " << s << " but expected " << ref_s << std::endl;
        }
        test_number++;
    }

    if (fail)
        std::cout << fail << " of " << fail + ok << " tests failed!" << std::endl;
    else
        std::cout << "All " << ok << " tests succeded!" << std::endl;
}

int main() {
    test(TESTS);
}

#endif


