#include <iostream>
#include "ds.hpp"
#include "verifier.hpp"
#include <deque>
bool verifier::checkSegmentOverlapping(Layout *layout){
    bool pass = true;
    std::vector<std::vector<std::vector<Grid>>> grid;
    grid.resize(layout->width+1);
    for (int i = 0; i <= layout->width; i++) {
        grid[i].resize(layout->height+1);
        for (int j = 0; j <= layout->height; j++) {
            grid[i][j].resize(2);
            for (int k = 0; k < 2; k++) {
                grid[i][j][k].net_id = -1;
            }
        }
    }

    for(auto n : layout->netlist){
        for(auto hs : n.horizontal_segments){
            for(int i = std::min(hs.start_point.x, hs.end_point.x); i <= std::max(hs.start_point.x, hs.end_point.x); i++){
                if(grid.at(i).at(hs.start_point.y).at(hs.start_point.z).net_id != -1 && grid.at(i).at(hs.start_point.y).at(hs.start_point.z).net_id != n.id){
                    std::cout << "Error: Horizontal segment Net#" << n.id << " " << hs.start_point.toString() << "-" << hs.end_point.toString();
                    std::cout << " overlap with Net#" << grid.at(i).at(hs.start_point.y).at(hs.start_point.z).net_id << " ";
                    for(auto s : grid.at(i).at(hs.start_point.y).at(hs.start_point.z).segments){
                        std::cout << s->start_point.toString() << "-" << s->end_point.toString() << " ";
                    }
                    std::cout << ".\n";
                    pass = false;
                }
                grid.at(i).at(hs.start_point.y).at(hs.start_point.z).net_id = n.id;
                grid.at(i).at(hs.start_point.y).at(hs.start_point.z).segments.push_back(&hs);
            }
        }
        for(auto vs : n.vertical_segments){
            for(int i = std::min(vs.start_point.y, vs.end_point.y); i <= std::max(vs.start_point.y, vs.end_point.y); i++){
                if(grid.at(vs.start_point.x).at(i).at(vs.start_point.z).net_id != -1 && grid.at(vs.start_point.x).at(i).at(vs.start_point.z).net_id != n.id){
                    std::cout << "Error: Vertical segment Net#" << n.id << " " << vs.start_point.toString() << "-" << vs.end_point.toString();
                    std::cout << " overlap with Net#" << grid.at(vs.start_point.x).at(i).at(vs.start_point.z).net_id << " ";
                    for(auto s : grid.at(vs.start_point.x).at(i).at(vs.start_point.z).segments){
                        std::cout << s->start_point.toString() << "-" << s->end_point.toString() << " ";
                    }
                    std::cout << ".\n";
                    pass = false;
                }
                grid.at(vs.start_point.x).at(i).at(vs.start_point.z).net_id = n.id;
                grid.at(vs.start_point.x).at(i).at(vs.start_point.z).segments.push_back(&vs);
            }
        }
    }
    return pass;
}
std::pair<bool, bool> verifier::checkNetOpenAndConnectivity(Layout *layout){
    bool no_pass = true, c_pass = true;
    for(auto &n : layout->netlist){
        int node_cnt = 0;
        std::map<Coordinate2D, int> new_id;
        std::vector<Coordinate2D> trace_back;
        // Counting node number first
        for(auto hs : n.horizontal_segments){
            for(int i = std::min(hs.start_point.x, hs.end_point.x); i <= std::max(hs.start_point.x, hs.end_point.x); i++){
                if(!new_id.count(Coordinate2D(i, hs.start_point.y))){
                    new_id[Coordinate2D{i, hs.start_point.y}] = node_cnt++;
                    trace_back.emplace_back(i, hs.start_point.y);
                }
            }
        }
        for(auto vs : n.vertical_segments){
            for(int i = std::min(vs.start_point.y, vs.end_point.y); i <= std::max(vs.start_point.y, vs.end_point.y); i++){
                if(!new_id.count(Coordinate2D(vs.start_point.x, i))){
                    new_id[Coordinate2D{vs.start_point.x, i}] = node_cnt++;
                    trace_back.emplace_back(vs.start_point.x, i);
                }
            }
        }
        // Build graph
        std::vector<std::vector<int>> adj(node_cnt);
        std::vector<bool> vis(node_cnt, false);
        for(auto hs : n.horizontal_segments){
            for(int i = std::min(hs.start_point.x, hs.end_point.x); i <= std::max(hs.start_point.x, hs.end_point.x); i++){
                if(i == std::min(hs.start_point.x, hs.end_point.x)){
                    int cur = new_id[Coordinate2D{i, hs.start_point.y}], to = new_id[Coordinate2D{i + 1, hs.start_point.y}];
                    adj.at(cur).push_back(to);
                }
                else if(i == std::max(hs.start_point.x, hs.end_point.x)){
                    int cur = new_id[Coordinate2D{i, hs.start_point.y}], pre = new_id[Coordinate2D{i - 1, hs.start_point.y}];
                    adj.at(cur).push_back(pre);
                }
                else{
                    int cur = new_id[Coordinate2D{i, hs.start_point.y}], to = new_id[Coordinate2D{i + 1, hs.start_point.y}], pre = new_id[Coordinate2D{i - 1, hs.start_point.y}];
                    adj.at(cur).push_back(to);
                    adj.at(cur).push_back(pre);
                }
            }
        }
        for(auto vs : n.vertical_segments){
            for(int i = std::min(vs.start_point.y, vs.end_point.y); i <= std::max(vs.start_point.y, vs.end_point.y); i++){
                if(i == std::min(vs.start_point.y, vs.end_point.y)){
                    int cur = new_id[Coordinate2D{vs.start_point.x, i}], to = new_id[Coordinate2D{vs.start_point.x, i + 1}];
                    adj.at(cur).push_back(to);
                }
                else if(i == std::max(vs.start_point.y, vs.end_point.y)){
                    int cur = new_id[Coordinate2D{vs.start_point.x, i}], pre = new_id[Coordinate2D{vs.start_point.x, i - 1}];
                    adj.at(cur).push_back(pre);
                }
                else{
                    int cur = new_id[Coordinate2D{vs.start_point.x, i}], to = new_id[Coordinate2D{vs.start_point.x, i + 1}], pre = new_id[Coordinate2D{vs.start_point.x, i - 1}];
                    adj.at(cur).push_back(to);
                    adj.at(cur).push_back(pre);
                }
            }
        }
        // Check degree 1 whether a pin
        for(unsigned i = 0; i < adj.size(); i++){
            if(adj.at(i).size() == 1){
                bool ok = false;
                for(auto &p : n.pins){
                    if(p.x == trace_back.at(i).x && p.y == trace_back.at(i).y) ok = true;
                }
                if(!ok){
                    std::cout << "Error: Net#" << n.id << " " << trace_back.at(i).toString() << " is open.\n";
                    no_pass = false;
                }
            }
        }
        // DFS
        for(auto &p : n.pins){
            if(!new_id.count(Coordinate2D{p.x, p.y})){
                std::cout << "Error: Net#" << n.id << " is not connected.\n";
                c_pass = false;
            }
        }
        if(c_pass){
            std::deque<int> dq;
            dq.push_back(0);
            while(!dq.empty())
            {
                int now = dq.back(); dq.pop_back();
                vis[now] = true;
                for(auto i : adj[now])
                    if(!vis[i]) dq.push_back(i);	
            }
            for(auto i : vis) if(!i) {
                std::cout << "Error: Net#" << n.id << " is not connected.\n";
                c_pass = false;
            }
        }
    }
    return std::make_pair(no_pass, c_pass);
}