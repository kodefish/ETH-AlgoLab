#include <iostream>
#include <string>

// Returns final timestamp
int dfs_rec(bool *adj, int *timestamps, int starting_vertex, int current_timestamp, int num_vertices) {
    // label starting_vertex as discovered
    timestamps[2*starting_vertex] = current_timestamp++; 

    // visit neighbors: adj[starting_vertex*num_vertex + i] is true if there is an edge between starting_vertex and i
    for(int i = 0; i < num_vertices; i++) {
        // if there is an edge and if not discovered
        if (adj[starting_vertex*num_vertices + i] && timestamps[2*i] == -1) {
            // then recursively call dfs_rec on the neighbor
            current_timestamp = dfs_rec(adj, timestamps, i, current_timestamp, num_vertices);
        }
    }
    // Mark discovery as finished
    timestamps[2*starting_vertex + 1] = current_timestamp++;
    return current_timestamp;
}

void testcase() {
    int num_vertices; std::cin >> num_vertices;
    int num_edges; std::cin >> num_edges;
    int starting_vertex; std::cin >> starting_vertex;

    // Adjecency matrix
    bool adj[num_vertices*num_vertices];
    for(int i = 0; i < num_vertices*num_vertices; i++) {
        adj[i] = false;
    }

    // Record all the edge info - build the adjecency matrix
    for(int i = 0; i < num_edges; i++) {
        int v1; std::cin >> v1; 
        int v2; std::cin >> v2; 

        adj[v1*num_vertices + v2] = true;
        adj[v2*num_vertices + v1] = true;
    }

    // Records timestamps for nodes
    // i:   timestamp of discovery
    // i+1: timestamp of finish
    int timestamps[2*num_vertices];
    for(int i = 0; i < 2*num_vertices; i++) {
        timestamps[i] = -1;
    }

    int res = dfs_rec(adj, timestamps, starting_vertex, 0, num_vertices);

    for(int i = 0; i < num_vertices; i++) {
            std::cout << timestamps[2*i] << " ";
    }
    std::cout << std::endl;

    for(int i = 0; i < num_vertices; i++) {
            std::cout << timestamps[2*i + 1] << " ";
    }
    std::cout << std::endl;
    
}

int main() {
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}
