struct TrafficNetworkTester
{
    const Map& map;
    std::unordered_map<Place, unsigned> condensedPlaces;
    std::vector<std::unordered_set<unsigned>> condForwardConns;
    std::vector<std::unordered_set<unsigned>> condBackwardConns;


    explicit TrafficNetworkTester(const Map & map) : map(map)
    {
        std::unordered_map<Place, unsigned> placeToNum;
        std::vector<std::unordered_set<unsigned>> forwardConns;
        std::vector<std::unordered_set<unsigned>> backwardConns;
        std::vector<unsigned> components;

        placesVecToMap(map.places, placeToNum);
        extendConnVectors(map.connections, forwardConns, backwardConns, placeToNum);
        unsigned componentCnt = findComponents(forwardConns, backwardConns, components);
        createCondensedMap(components);
        createCondesedConnections(componentCnt);
    }

    void createCondensedMap(const std::vector<unsigned>& components){
        for(unsigned i = 0; i < map.places.size(); i++)
            condensedPlaces.insert({map.places[i], components[i]});
    }

    void createCondesedConnections(const unsigned componentCnt){
        condForwardConns.resize(componentCnt);
        condBackwardConns.resize(componentCnt);

        for(unsigned i = 0; i < map.connections.size(); i++){
            Connection conn = map.connections[i];
            unsigned fromNum = condensedPlaces.find(conn.first)->second;
            unsigned toNum = condensedPlaces.find(conn.second)->second;
            condForwardConns[fromNum].insert(toNum);
            condBackwardConns[toNum].insert(fromNum);
        }
    }

    void placesVecToMap(const std::vector<Place> & vec, std::unordered_map<Place, unsigned>& compMap)const{
        for(unsigned i = 0; i < vec.size(); i++)
            compMap.insert({vec[i], i});
    }

    void extendConnVectors(const std::vector<Connection>& connsToAdd, std::vector<std::unordered_set<unsigned>>& forwardConns, std::vector<std::unordered_set<unsigned>>& backwardConns, const std::unordered_map<Place, unsigned>& placeToNum)const{
        unsigned placesNum = placeToNum.size();
        forwardConns.resize(forwardConns.size() + placesNum);
        backwardConns.resize(backwardConns.size() + placesNum);

        for(auto & toAdd : connsToAdd){
            if(toAdd.first != toAdd.second){
                forwardConns[placeToNum.find(toAdd.first)->second].insert(placeToNum.find(toAdd.second)->second);
                backwardConns[placeToNum.find(toAdd.second)->second].insert(placeToNum.find(toAdd.first)->second);
            }
        }
    }

    // void dfs(std::stack<unsigned>& stack, const std::vector<std::set<unsigned>>& connections, const unsigned node, std::vector<bool>& state)const{
    //     state[node] = true;
    //     for(unsigned neighbour : connections[node]){
    //         if(state[neighbour] == false){
    //             dfs(stack, connections, neighbour, state);
    //             stack.push(neighbour);
    //         }
    //     }
    // }

    // void specialDfs(const std::vector<std::set<unsigned>>& connections, const unsigned node, std::vector<bool>& state, std::vector<unsigned>& components, const unsigned component)const{
    //     state[node] = true;
    //     unsigned minusOne = -1;
    //     for(unsigned neighbour : connections[node]){
    //         if(state[neighbour] == false && components[neighbour] == minusOne){
    //             components[neighbour] = component;
    //             specialDfs(connections, neighbour, state, components, component);
    //         }
    //     }
    // }
    void specialIterativeDfs(const std::vector<std::unordered_set<unsigned>>& connections, const unsigned node, std::vector<bool>& state, std::vector<unsigned>& components, const unsigned component)const{
        for(auto it = connections[node].begin(); it != connections[node].end(); ++it);
        state[node] = true;
        std::stack<std::pair<unsigned, std::unordered_set<unsigned int>::const_iterator>> dfsStack;
        dfsStack.emplace(node, connections[node].begin());
        unsigned minusOne = -1;

        while(!dfsStack.empty()){
            std::pair<unsigned, std::unordered_set<unsigned int>::const_iterator>& top = dfsStack.top();
            if(top.second == connections[top.first].end()){
                dfsStack.pop();
            }else{
                unsigned neighbour = *(top.second);
                if(!state[neighbour] && components[neighbour] == minusOne){
                    components[neighbour] = component;
                    state[neighbour] = true;
                    dfsStack.emplace(neighbour, connections[neighbour].begin());
                }
                top.second++;
            }
        }
    }

    void iterativeDfs(std::stack<unsigned>& stack, const std::vector<std::unordered_set<unsigned>>& connections, const unsigned node, std::vector<bool>& state)const{

        state[node] = true;
        std::stack<std::pair<unsigned, std::unordered_set<unsigned int>::const_iterator>> dfsStack;
        dfsStack.emplace(node, connections[node].begin());

        while(!dfsStack.empty()){
            auto & top = dfsStack.top();
            if(top.second == connections[top.first].end()){
                dfsStack.pop();
                stack.push(top.first);
            }else{
                unsigned neighbour = *(top.second);
                if(!state[neighbour]){
                    state[neighbour] = true;
                    dfsStack.emplace(neighbour, connections[neighbour].begin());
                }
                top.second++;
            }
        }
    }


    unsigned findComponents(const std::vector<std::unordered_set<unsigned>>& forwardConns, const std::vector<std::unordered_set<unsigned>>& backwardConns, std::vector<unsigned>& components) const{
        std::stack<unsigned> stack;
        unsigned componentCnt = 0;
        unsigned placeNum = forwardConns.size();

        components.resize(placeNum);
        std::fill(components.begin(), components.end(), -1);
        std::vector<bool> state;
        state.resize(placeNum);
        std::fill(state.begin(), state.end(), 0);

        for(unsigned i = 0; i < placeNum; i++){
            if(!state[i]){
                // dfs(stack, backwardConns, i, state);
                iterativeDfs(stack, backwardConns, i, state);
                // stack.push(i);
            }
        }

        std::fill(state.begin(), state.end(), false);

        unsigned minusOne = -1;
        while(!stack.empty()){
            unsigned node = stack.top();
            stack.pop();
            if(components[node] == minusOne){
                components[node] = componentCnt;
                specialIterativeDfs(forwardConns, node, state, components, componentCnt);
                componentCnt++;
            }
        }

        return componentCnt;
    }

    void addNewPlaces(const std::vector<Connection> &conns, std::unordered_map<Place, unsigned>& placesMap, std::vector<std::unordered_set<unsigned>>& forwardConns, std::vector<std::unordered_set<unsigned>>& backwardConns)const{
        unsigned placeAmount = forwardConns.size();
        forwardConns.resize(placeAmount);
        backwardConns.resize(placeAmount);
        for(auto &conn : conns){
            auto fromIt = placesMap.find(conn.first);
            auto toIt = placesMap.find(conn.second);
            if(fromIt == placesMap.end() && toIt == placesMap.end()){ //both new
                if(conn.first == conn.second){ //to and from are the same, so no new connection
                    placesMap.insert({conn.first, placeAmount});
                    forwardConns.emplace_back();
                    backwardConns.emplace_back();
                    placeAmount++;
                }else{ //insert both and new connections for both
                    placesMap.insert({conn.first, placeAmount});
                    placesMap.insert({conn.second, placeAmount + 1});
                    forwardConns.push_back({placeAmount + 1});
                    forwardConns.emplace_back();
                    backwardConns.emplace_back();
                    backwardConns.push_back({placeAmount});
                    placeAmount = placeAmount + 2;
                }
            }else if(fromIt == placesMap.end()){ //from is new
                placesMap.insert({conn.first, placeAmount});
                forwardConns.push_back({toIt->second});
                backwardConns.emplace_back();
                backwardConns[toIt->second].insert(placeAmount);
                placeAmount++;
            }else if(toIt == placesMap.end()){ //to is new
                placesMap.insert({conn.second, placeAmount});
                forwardConns[fromIt->second].insert(placeAmount);
                forwardConns.emplace_back();
                backwardConns.push_back({fromIt->second});
                placeAmount++;
            }else{ //both old
                forwardConns[fromIt->second].insert(toIt->second);
                backwardConns[toIt->second].insert(fromIt->second);
            }
        }
    }
    // Count how many areas exist in the network
    // after adding conns.
    // Note that conns may introduce new places.

    unsigned count_areas(const std::vector<Connection> &conns) const{
        std::unordered_map<Place, unsigned> placesMap = condensedPlaces;
        std::vector<std::unordered_set<unsigned>> forwardConns = condForwardConns;
        std::vector<std::unordered_set<unsigned>> backwardConns = condBackwardConns;

        addNewPlaces(conns, placesMap, forwardConns, backwardConns);

        std::vector<unsigned> components;
        return findComponents(forwardConns, backwardConns, components);
    }
};
