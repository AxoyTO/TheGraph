#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

std::string get_date_and_time(){
    std::time_t now =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%d/%m/%Y %T");
    std::string time = ss.str();
    return time;
}

// можно input получить тут, чтобы легче было в main и сделать эту функцию
// generic(template) для будущего, или же просто int, поскольку сейчас вводы
// будут только max_depth, new_v_num и graphs_count
/*
 * template <typename T>
 * T get_input(T input){};
 */

// Не уверен, правильно ли создать такой объёмный неймспейс в main,
// можно в logger.cpp это перенести вместе с get_date_and_time
namespace logger {
    std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> set_colors(
            const Graph &graph){
        std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> colors = {
                {uni_cpp_practice::Edge::Color::Gray,
                        graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Gray)},
                {uni_cpp_practice::Edge::Color::Green,
                        graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Green)},
                {uni_cpp_practice::Edge::Color::Blue,
                        graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Blue)},
                {uni_cpp_practice::Edge::Color::Yellow,
                        graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Yellow)},
                {uni_cpp_practice::Edge::Color::Red,
                        graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Red)}};

        return colors;
    }

    void log_start(Logger &logger, const int graph_number){
        logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
                   ", Generation Started\n");
    }

    void log_depth(Logger &logger, const Graph &graph){
        for(int j = 0; j <= graph.depth(); j++){
            logger.log(std::to_string(graph.get_vertices_in_depth(j).size()));
            if(j != graph.depth())
                logger.log(", ");
        }
    }

    void log_colors(
            Logger &logger,
            const std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> &colors){
        for(int j = 0; j < colors.size(); j++){
            logger.log(color_to_string(colors[j].first) + ": " +
                       std::to_string(colors[j].second));
            if(j + 1 != colors.size())
                logger.log(", ");
        }
    }

    void log_end(
            Logger &logger,
            const Graph &graph,
            const std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> &colors,
            int &graph_number){
        logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number++) +
                   ", Generation Finished {  \n");
        logger.log("  depth: " + std::to_string(graph.depth()) + ",\n");
        logger.log("  vertices: " + std::to_string(graph.get_vertices().size()) +
                   ", [");
        log_depth(logger, graph);
        logger.log("],\n  edges: " + std::to_string(graph.get_edges().size()) +
                   ", {");
        log_colors(logger, colors);
        logger.log("}\n}\n");
    }
}  // namespace logger

int main(){
    int max_depth = 0;
    int new_vertices_num = 0;
    int graphs_count = 0;

    std::cout << "Enter graphs_count: ";
    do{
        std::cin >> graphs_count;
        if(graphs_count < 0)
            std::cerr << "Count of graphs to be created can not be negative!\n"
                         "Enter a non-negative graphs_count: ";
    } while(graphs_count < 0);
    std::cout << "Enter max_depth: ";
    do{
        std::cin >> max_depth;
        if(max_depth < 0)
            std::cerr << "Depth can not be negative!\n"
                         "Enter a non-negative max_depth: ";
    } while(max_depth < 0);
    std::cout << "Enter new_vertices_num: ";
    do{
        std::cin >> new_vertices_num;
        if(new_vertices_num < 0)
            std::cerr << "Number of new vertices created by each vertex can not be "
                         "negative!\nEnter a non-negative new_vertices_num: ";
    } while(new_vertices_num < 0);
    std::cout << "\n";
    try{
        if(!std::filesystem::create_directory("./temp")){
        }
    } catch(const std::exception &ex){
        std::cerr << ex.what() << "\n";
    }

    Logger &logger = Logger::get_instance();
    logger.set_file("./temp/log.txt");

    int graph_number = 0;
    for(int i = 0; i < graphs_count; i++){
        std::string filename = "./temp/graph";
        filename = filename + std::to_string(graph_number) + ".json";
        std::ofstream file(filename, std::ios::out);
        if(!file.is_open())
            std::cerr << "Error opening the file graph " << graph_number << ".json\n";
        else{
            logger::log_start(logger, graph_number);
            const auto graph = GraphGenerator(max_depth, new_vertices_num).generate();
            auto colors = logger::set_colors(graph);
            logger::log_end(logger, graph, colors, graph_number);
            GraphPrinter graph_printer(graph);
            file << graph_printer.print();
            file.close();
        }
    }
    return 0;
}
