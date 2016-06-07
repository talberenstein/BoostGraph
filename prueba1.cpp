//=======================================================================
// Autor: Tal Zeev T.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

int main(int, char*[]){

	//Lista de adayacencia con propiedades
	typedef adjacency_list < listS, vecS, bidirectionalS,
    	no_property, property < edge_weight_t, int > > Graph;
    //Descriptor de vértices
  	typedef graph_traits < Graph >::vertex_descriptor vertex_descriptor;
  	//Descriptor de aristas
  	typedef graph_traits < Graph >::edge_descriptor edge_descriptor;
  	//Forma par de arista (x,y)
  	typedef std::pair<int, int> Edge;

	//Etiquetas para vértices
	enum {A, B, C, D, E, N};
	const int num_v = N; //cantidad de vértices
	const char name[] = "ABCDE";

	//Aristas del grafo
	Edge edge_array[] = { Edge(A,B), Edge(A,C), Edge(A,D), Edge(A,E), 
		Edge(C,E), Edge(C,A), Edge(D,B), Edge(D,E), Edge(D,A), Edge(E,A), Edge(E,B) };
	//Pesos para cada arista respectiva
	int weights[] = { 4, 2, 1, 2, 7, 3, 1, 1, 5, 5, 3};
	//numero de aristas
	int num_arcs = sizeof(edge_array) / sizeof(Edge);

	//CONSTRUCCIÓN DEL GRAFO
	//caso 1
	#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  	Graph g(num_nodes);
  	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  	for (std::size_t j = 0; j < num_arcs; ++j) {
    edge_descriptor e; bool inserted;
    boost::tie(e, inserted) = add_edge(edge_array[j].first, edge_array[j].second, g);
    weightmap[e] = weights[j];
  	}
  	//caso 2
  	#else
  	Graph g(edge_array, edge_array + num_arcs, weights, num_v);
  	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g); //mapa de pesos en las aristas
  	//Ambos casos
  	#endif
  	std::vector<vertex_descriptor> p(num_vertices(g));
  	std::vector<int> d(num_vertices(g));
  	vertex_descriptor s = vertex(A, g);

  	//APLICACIÓN DE DIJKSTRA BOOST
  	//caso 1
  	#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  	property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g); //mapa de indice de vértices
  	dijkstra_shortest_paths(g, s, &p[0], &d[0], weightmap, indexmap, 
                          std::less<int>(), closed_plus<int>(), 
                          (std::numeric_limits<int>::max)(), 0,
                          default_dijkstra_visitor());
  	//caso 2
  	#else
  	dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));

  	//Impresión por pantalla
  	#endif

  	std::cout << "Distancias y nodo padre" << std::endl;
  	graph_traits <Graph>::vertex_iterator vi, vend;
  	for(boost::tie(vi,vend) = vertices(g); vi != vend; ++vi){
  		std::cout << "Distancia más corta para llegar a nodo " << name[*vi] << ": " << d[*vi] << std::endl;
  		std::cout << "Camino en Dijkstra (nodo anterior) para llegar a nodo " << name[*vi] << ": " << name[p[*vi]] << std::endl;
  	}
  	std::cout << std::endl;

std::ofstream dot_file("figs/graph.dot");

  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  graph_traits < Graph >::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    graph_traits < Graph >::edge_descriptor e = *ei;
    graph_traits < Graph >::vertex_descriptor
      u = source(e, g), v = target(e, g);
    dot_file << name[u] << " -> " << name[v]
      << "[label=\"" << get(weightmap, e) << "\"";
    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"grey\"";
    dot_file << "]";
  }
  dot_file << "}";
  return EXIT_SUCCESS;
}