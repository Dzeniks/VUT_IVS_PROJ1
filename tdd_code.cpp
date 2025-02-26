//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"

Graph::Graph() {}

Graph::~Graph() {
    clear();
}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;
    nodes.reserve(m_nodes.size());

    for (const auto& pair : m_nodes) {
        nodes.push_back(pair.second);
    }

    return nodes;
}

std::vector<Edge> Graph::edges() const {
    return m_edges;
}

Node* Graph::addNode(size_t nodeId) {
    // Kontrola, zda uzel již existuje
    if (m_nodes.find(nodeId) != m_nodes.end()) {
        return nullptr;
    }

    // Vytvoření nového uzlu
    Node* newNode = new Node(nodeId);
    m_nodes[nodeId] = newNode;

    // Inicializace prázdného seznamu sousedů pro nový uzel
    m_adjacency[nodeId] = std::vector<size_t>();

    return newNode;
}

bool Graph::addEdge(const Edge& edge) {
    // Ignorování smyček
    if (edge.a == edge.b) {
        return false;
    }

    // Kontrola, zda hrana již existuje
    if (containsEdge(edge)) {
        return false;
    }

    // Přidání uzlů, pokud neexistují
    if (m_nodes.find(edge.a) == m_nodes.end()) {
        addNode(edge.a);
    }

    if (m_nodes.find(edge.b) == m_nodes.end()) {
        addNode(edge.b);
    }

    // Přidání hrany
    m_edges.push_back(edge);

    // Aktualizace seznamů sousedů
    m_adjacency[edge.a].push_back(edge.b);
    m_adjacency[edge.b].push_back(edge.a);

    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    for (const Edge& edge : edges) {
        addEdge(edge);
    }
}

Node* Graph::getNode(size_t nodeId) {
    auto it = m_nodes.find(nodeId);
    if (it != m_nodes.end()) {
        return it->second;
    }

    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const {
    // Kontrola existence uzlů
    auto itA = m_nodes.find(edge.a);
    auto itB = m_nodes.find(edge.b);

    if (itA == m_nodes.end() || itB == m_nodes.end()) {
        return false;
    }

    // Hledání hrany v seznamu hran
    for (const Edge& e : m_edges) {
        if (e == edge) {
            return true;
        }
    }

    return false;
}

void Graph::removeNode(size_t nodeId) {
    auto nodeIt = m_nodes.find(nodeId);
    if (nodeIt == m_nodes.end()) {
        throw std::out_of_range("Node does not exist");
    }

    // Odstranění všech hran spojených s tímto uzlem
    auto it = m_edges.begin();
    while (it != m_edges.end()) {
        if (it->a == nodeId || it->b == nodeId) {
            // Odstranění ze seznamu sousedů
            if (it->a == nodeId) {
                auto& neighbors = m_adjacency[it->b];
                neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), nodeId), neighbors.end());
            } else {
                auto& neighbors = m_adjacency[it->a];
                neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), nodeId), neighbors.end());
            }

            it = m_edges.erase(it);
        } else {
            ++it;
        }
    }

    // Odstranění seznamu sousedů
    m_adjacency.erase(nodeId);

    // Uvolnění paměti uzlu a odstranění z mapy uzlů
    delete nodeIt->second;
    m_nodes.erase(nodeIt);
}

void Graph::removeEdge(const Edge& edge) {
    if (!containsEdge(edge)) {
        throw std::out_of_range("Edge does not exist");
    }

    // Odstranění hrany z vektoru hran
    auto it = m_edges.begin();
    while (it != m_edges.end()) {
        if (*it == edge) {
            // Odstranění ze seznamu sousedů
            auto& neighborsA = m_adjacency[it->a];
            neighborsA.erase(std::remove(neighborsA.begin(), neighborsA.end(), it->b), neighborsA.end());

            auto& neighborsB = m_adjacency[it->b];
            neighborsB.erase(std::remove(neighborsB.begin(), neighborsB.end(), it->a), neighborsB.end());

            it = m_edges.erase(it);
            return;
        } else {
            ++it;
        }
    }
}

size_t Graph::nodeCount() const {
    return m_nodes.size();
}

size_t Graph::edgeCount() const {
    return m_edges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const {
    auto it = m_adjacency.find(nodeId);
    if (it == m_adjacency.end()) {
        throw std::out_of_range("Node does not exist");
    }

    return it->second.size();
}

size_t Graph::graphDegree() const {
    if (m_nodes.empty()) {
        return 0;
    }

    size_t maxDegree = 0;
    for (const auto& pair : m_adjacency) {
        maxDegree = std::max(maxDegree, pair.second.size());
    }

    return maxDegree;
}

void Graph::coloring() {
    if (m_nodes.empty()) {
        return;
    }

    // Implementace greedy barvení grafu
    // Nejprve seřadíme uzly podle stupně (sestupně)
    std::vector<size_t> nodeIds;
    nodeIds.reserve(m_nodes.size());

    for (const auto& pair : m_nodes) {
        nodeIds.push_back(pair.first);
    }

    std::sort(nodeIds.begin(), nodeIds.end(), [this](size_t a, size_t b) {
        return this->nodeDegree(a) > this->nodeDegree(b);
    });

    // Pro každý uzel najdeme první dostupnou barvu
    // Barvy číslujeme od 1, 0 znamená neobarveno
    for (size_t nodeId : nodeIds) {
        std::set<size_t> neighborColors;

        // Sbíráme barvy sousedů
        for (size_t neighborId : m_adjacency[nodeId]) {
            size_t neighborColor = m_nodes[neighborId]->color;
            if (neighborColor != 0) {
                neighborColors.insert(neighborColor);
            }
        }

        // Najdeme první dostupnou barvu
        size_t color = 1;
        while (neighborColors.find(color) != neighborColors.end()) {
            color++;
        }

        m_nodes[nodeId]->color = color;
    }
}

void Graph::clear() {
    // Uvolnění paměti všech uzlů
    for (auto& pair : m_nodes) {
        delete pair.second;
    }

    // Vyčištění datových struktur
    m_nodes.clear();
    m_edges.clear();
    m_adjacency.clear();
}

/*** Konec souboru tdd_code.cpp ***/