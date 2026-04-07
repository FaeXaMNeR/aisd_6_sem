#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <limits>
#include <map>

struct Edge;

struct Node {
    std::string id;
    std::vector<Edge *> incoming;
    std::vector<Edge *> outgoing;
};

struct Edge {
    Node *from;
    Node *to;
    int weight;
};

class Graph {
private:
    std::unordered_map<std::string, Node *> nodes;
    std::vector<Edge *> edges;

public:
    ~Graph() {
        for (auto &[id, node] : nodes) delete node;
        for (auto *edge : edges) delete edge;
    }

    void addNode(const std::string &id) {
        if (nodes.find(id) == nodes.end()) {
            Node *node = new Node();
            node->id = id;
            nodes[id] = node;
        }
    }

    void addEdge(const std::string &fromId, const std::string &toId, int weight) {
        bool hasFrom = nodes.count(fromId);
        bool hasTo   = nodes.count(toId);

        if (!hasFrom && !hasTo) {
            std::cout << "Unknown nodes " << fromId << " " << toId << "\n";
            return;
        }
        if (!hasFrom) {
            std::cout << "Unknown node " << fromId << "\n";
            return;
        }
        if (!hasTo) {
            std::cout << "Unknown node " << toId << "\n";
            return;
        }

        Node *from = nodes[fromId];
        Node *to   = nodes[toId];

        // Проверяем, нет ли уже такого ребра
        for (auto *e : from->outgoing) {
            if (e->to == to) return; // уже существует
        }

        Edge *edge = new Edge();
        edge->from = from;
        edge->to   = to;
        edge->weight = weight;

        from->outgoing.push_back(edge);
        to->incoming.push_back(edge);
        edges.push_back(edge);
    }

    void removeNode(const std::string &id) {
        if (!nodes.count(id)) {
            std::cout << "Unknown node " << id << "\n";
            return;
        }

        Node *node = nodes[id];

        // Удаляем все рёбра, связанные с этим узлом
        std::unordered_set<Edge *> toRemove;
        for (auto *e : node->incoming) toRemove.insert(e);
        for (auto *e : node->outgoing) toRemove.insert(e);

        // Убираем из смежных узлов ссылки на эти рёбра
        for (auto *e : toRemove) {
            auto &outVec = e->from->outgoing;
            outVec.erase(std::remove(outVec.begin(), outVec.end(), e), outVec.end());
            auto &inVec = e->to->incoming;
            inVec.erase(std::remove(inVec.begin(), inVec.end(), e), inVec.end());
        }

        // Удаляем из глобального списка рёбер
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [&](Edge *e){ return toRemove.count(e); }), edges.end());

        for (auto *e : toRemove) delete e;

        delete node;
        nodes.erase(id);
    }

    void removeEdge(const std::string &fromId, const std::string &toId) {
        bool hasFrom = nodes.count(fromId);
        bool hasTo   = nodes.count(toId);

        if (!hasFrom && !hasTo) {
            std::cout << "Unknown nodes " << fromId << " " << toId << "\n";
            return;
        }
        if (!hasFrom) {
            std::cout << "Unknown node " << fromId << "\n";
            return;
        }
        if (!hasTo) {
            std::cout << "Unknown node " << toId << "\n";
            return;
        }

        Node *from = nodes[fromId];
        Node *to   = nodes[toId];

        Edge *found = nullptr;
        for (auto *e : from->outgoing) {
            if (e->to == to) { found = e; break; }
        }
        if (!found) return;

        auto &outVec = from->outgoing;
        outVec.erase(std::remove(outVec.begin(), outVec.end(), found), outVec.end());
        auto &inVec = to->incoming;
        inVec.erase(std::remove(inVec.begin(), inVec.end(), found), inVec.end());
        edges.erase(std::remove(edges.begin(), edges.end(), found), edges.end());

        delete found;
    }

    // DFS для RPO: возвращает false если обнаружен цикл (back edge)
    bool dfsRPO(Node *node,
                std::unordered_set<Node *> &visited,
                std::unordered_set<Node *> &inStack,
                std::vector<Node *> &postOrder,
                std::string &loopFrom,
                std::string &loopTo)
    {
        visited.insert(node);
        inStack.insert(node);

        for (auto *e : node->outgoing) {
            Node *next = e->to;
            if (inStack.count(next)) {
                // Цикл обнаружен
                loopFrom = node->id;
                loopTo   = next->id;
                return false;
            }
            if (!visited.count(next)) {
                if (!dfsRPO(next, visited, inStack, postOrder, loopFrom, loopTo))
                    return false;
            }
        }

        inStack.erase(node);
        postOrder.push_back(node);
        return true;
    }

    void rpoNumbering(const std::string &startId) {
        if (!nodes.count(startId)) {
            std::cout << "Unknown node " << startId << "\n";
            return;
        }

        std::unordered_set<Node *> visited;
        std::unordered_set<Node *> inStack;
        std::vector<Node *> postOrder;
        std::string loopFrom, loopTo;

        bool ok = dfsRPO(nodes[startId], visited, inStack, postOrder, loopFrom, loopTo);

        if (!ok) {
            std::cout << "Found loop " << loopFrom << "->" << loopTo << "\n";
        }

        std::reverse(postOrder.begin(), postOrder.end());

        for (size_t i = 0; i < postOrder.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << postOrder[i]->id;
        }
        std::cout << "\n";
    }

    void dijkstra(const std::string &startId) {
        if (!nodes.count(startId)) {
            std::cout << "Unknown node " << startId << "\n";
            return;
        }

        const int INF = std::numeric_limits<int>::max();
        std::unordered_map<Node *, int> dist;
        for (auto &[id, node] : nodes) dist[node] = INF;

        Node *src = nodes[startId];
        dist[src] = 0;

        using PII = std::pair<int, Node *>;
        std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto *e : u->outgoing) {
                Node *v = e->to;
                int nd = dist[u] + e->weight;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.push({nd, v});
                }
            }
        }

        std::map<std::string, int> result;
        for (auto &[node, d] : dist) {
            if (node != src) result[node->id] = d;
        }
        for (auto &[id, d] : result) {
            std::cout << id << " " << d << "\n";
        }
    }

    // алгоритм Эдмондса-Карпа: BFS по остаточной сети
    void maxFlow(const std::string &srcId, const std::string &snkId) {
        if (!nodes.count(srcId)) {
            std::cout << "Unknown node " << srcId << "\n";
            return;
        }
        if (!nodes.count(snkId)) {
            std::cout << "Unknown node " << snkId << "\n";
            return;
        }

        Node *source = nodes[srcId];
        Node *sink   = nodes[snkId];

        // Остаточная ёмкость: capacity[u][v]
        std::unordered_map<Node *, std::unordered_map<Node *, int>> cap;

        for (auto *e : edges) {
            cap[e->from][e->to] += e->weight;
            if (!cap[e->to].count(e->from))
                cap[e->to][e->from] = 0;
        }

        // Список всех соседей
        std::unordered_map<Node *, std::vector<Node *>> adj;
        for (auto *e : edges) {
            adj[e->from].push_back(e->to);
            adj[e->to].push_back(e->from);
        }

        int totalFlow = 0;

        while (true) {
            // BFS
            std::unordered_map<Node *, Node *> parent;
            parent[source] = nullptr;
            std::queue<Node *> q;
            q.push(source);

            while (!q.empty() && !parent.count(sink)) {
                Node *u = q.front(); q.pop();
                for (Node *v : adj[u]) {
                    if (!parent.count(v) && cap[u][v] > 0) {
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }

            if (!parent.count(sink)) break;

            int pathFlow = std::numeric_limits<int>::max();
            for (Node *v = sink; v != source; v = parent[v]) {
                Node *u = parent[v];
                pathFlow = std::min(pathFlow, cap[u][v]);
            }

            for (Node *v = sink; v != source; v = parent[v]) {
                Node *u = parent[v];
                cap[u][v] -= pathFlow;
                cap[v][u] += pathFlow;
            }

            totalFlow += pathFlow;
        }

        std::cout << totalFlow << "\n";
    }

    struct TarjanState {
        std::unordered_map<Node *, int>  index;
        std::unordered_map<Node *, int>  lowlink;
        std::unordered_map<Node *, bool> onStack;
        std::vector<Node *>              stack;
        std::vector<std::vector<Node *>> sccs;
        int counter = 0;
    };

    void tarjanDFS(Node *u, TarjanState &s) {
        s.index[u]   = s.counter;
        s.lowlink[u] = s.counter;
        ++s.counter;
        s.stack.push_back(u);
        s.onStack[u] = true;

        for (auto *e : u->outgoing) {
            Node *v = e->to;
            if (!s.index.count(v)) {
                // v ещё не посещён
                tarjanDFS(v, s);
                s.lowlink[u] = std::min(s.lowlink[u], s.lowlink[v]);
            } else if (s.onStack[v]) {
                // v на стеке
                s.lowlink[u] = std::min(s.lowlink[u], s.index[v]);
            }
        }

        // u — корень SCC
        if (s.lowlink[u] == s.index[u]) {
            std::vector<Node *> scc;
            while (true) {
                Node *w = s.stack.back();
                s.stack.pop_back();
                s.onStack[w] = false;
                scc.push_back(w);
                if (w == u) break;
            }
            s.sccs.push_back(std::move(scc));
        }
    }

    void tarjan(const std::string &startId) {
        if (!nodes.count(startId)) {
            std::cout << "Unknown node " << startId << "\n";
            return;
        }

        TarjanState s;

        tarjanDFS(nodes[startId], s);
        for (auto &[id, node] : nodes) {
            if (!s.index.count(node)) {
                tarjanDFS(node, s);
            }
        }

        for (auto &scc : s.sccs) {
            if (scc.size() <= 1) continue;

            std::vector<std::string> ids;
            for (auto *n : scc) ids.push_back(n->id);
            std::sort(ids.begin(), ids.end());

            for (size_t i = 0; i < ids.size(); ++i) {
                if (i > 0) std::cout << " ";
                std::cout << ids[i];
            }
            
            std::cout << "\n";
        }
    }
};

int main() {
    Graph g;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "NODE") {
            std::string id;
            iss >> id;
            g.addNode(id);

        } else if (cmd == "EDGE") {
            std::string a, b;
            int w;
            iss >> a >> b >> w;
            g.addEdge(a, b, w);

        } else if (cmd == "REMOVE") {
            std::string sub;
            iss >> sub;
            if (sub == "NODE") {
                std::string id;
                iss >> id;
                g.removeNode(id);
            } else if (sub == "EDGE") {
                std::string a, b;
                iss >> a >> b;
                g.removeEdge(a, b);
            }

        } else if (cmd == "RPO_NUMBERING") {
            std::string id;
            iss >> id;
            g.rpoNumbering(id);

        } else if (cmd == "DIJKSTRA") {
            std::string id;
            iss >> id;
            g.dijkstra(id);

        } else if (cmd == "TARJAN") {
            std::string id;
            iss >> id;
            g.tarjan(id);

        } else if (cmd == "MAX") {
            std::string sub, a, b;
            iss >> sub >> a >> b;
            if (sub == "FLOW") {
                g.maxFlow(a, b);
            }
        }
    }

    return 0;
}