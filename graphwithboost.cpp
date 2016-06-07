//=======================================================================
// Autor: Tal Zeev T.
//
// Boost Software License, Version 1.0.
//=======================================================================

#include <boost/config.hpp>
#include <iostream> //std:cout
#include <utility> //std:pair
#include <algorithm> //std:for_each

//boost
#include <boost/utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

template <class Graph> struct exercise_vertex{
	exercise_vertex(Graph& g_) : g(g_){}
	Graph& g;

	//Descriptor de vertices
	typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
	void operator()(const Vertex& v) const{
		using namespace boost;
		typedef graph_traits<Graph> GraphTraits;
		typename property_map<Graph, vertex_index_t>::type
		index = get(vertex_index, g);
		//typename property_map<Graph, vertex_index_t>::type
		//index = get(vertex_index, g);
		//std::cout << "vertex: " << name[get(index, v)] << std::endl;

		//Las aristas que salen de cada vértice
		std::cout << "out-edges: ";
		typename GraphTraits::out_edge_iterator out_i, out_end;
		typename GraphTraits::edge_descriptor e;
		for (tie(out_i, out_end) = out_edges(v, g);
				out_i != out_end; ++out_i){
			e = *out_i;
		Vertex src = source(e, g), targ = target(e, g);
		std::cout << "(" << index[src] << "," << index[targ] << ") ";
		}
		std::cout << std::endl;

		//Las aristas que entran a cada vértice
		std::cout << "in-edges: ";
		typename graph_traits<Graph>::in_edge_iterator in_i, in_end;
		for(tie(in_i, in_end) = in_edges(v, g);
				in_i != in_end; ++in_i){
			e = *in_i;
		Vertex src = source(e, g), targ = target(e, g);
		std::cout << "(" << index[src] << "," << index[targ] << ") ";
		}
		std::cout << std::endl;

		//Vertices adyacentes
		std::cout << "adjacent vertices: ";
		typename graph_traits<Graph>::adjacency_iterator ai, ai_end;
		for(tie(ai, ai_end) = adjacent_vertices(v, g);
				ai != ai_end; ++ai){
			std::cout << index[*ai] << " ";
		}
		std::cout << std::endl;
	}
	//...
};

int main(int, char*[])
{

	//crear el typedef para el grafo
	typedef adjacency_list < vecS, vecS, bidirectionalS, no_property, property < edge_weight_t, int > > Graph; //bidireccional para que sea dirigido y acceso a ambos fuera y en los bordes //directedS, undirectedS


	//Etiquetas para vértices
	enum {A, B, C, D, E, N};
	const int num_vertices = N; //cantidad de vértices
	const char name[] = "ABCDE";

	//Escribir aristas en el grafo
	typedef std::pair<int, int> Edge;
	Edge edge_array[] = { Edge(A,B), Edge(A,C), Edge(A,D), Edge(A,E), Edge(C,E), Edge(C,A), Edge(D,B), Edge(D,E), Edge(D,A), Edge(E,A), Edge(E,B) };
	const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]); //cantidad de aristas

	//Asignar pesos
	int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1};

	//declarar el grafo
	Graph g(num_vertices);


	// agregar las aristas al grafo
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
	for(int i=0; i < num_edges; ++i){
		graph_traits<Graph>::edge_descriptor e; bool inserted;
		tie(e, inserted) = add_edge(edge_array[i].first, edge_array[i].second, g);
		weightmap[e] = weights[i];
	}

	//mapa de indices de vertices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);

	typedef property_map<Graph, edge_weight_t>::type TransWeight;
	TransWeight trans = get(edge_weight, g);

	//contando vertices
	std::cout << "vertices(g) = ";
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	std::pair<vertex_iter, vertex_iter> vp;
	for(vp = vertices(g); vp.first != vp.second; ++vp.first){
		std::cout << index[*vp.first] << " ";
	}
	std::cout << std::endl;

	//contando aristas
	std::cout << "edges(g) = ";
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
		std::cout << "(" << index[source(*ei, g)] << ","
			<< index[target(*ei, g)] << ") ";
	}
	std::cout << std::endl;

	//Estructura de la lista de adyacencia
	std::for_each(vertices(g).first, vertices(g).second, exercise_vertex<Graph>(g));
	return 0;

}