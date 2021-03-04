#include "graphics.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <cstdlib>

using namespace std;
using namespace genv;

typedef vector<vector<int> > map_to_read;

int XX;
int YY;

void read(map_to_read &m, string file_name="map.txt")
{
    ifstream file_in(file_name);
    if(!file_in.good()){
      cerr << file_name << "No such file\n";
      exit(1);
    }
    file_in >> XX >> YY;
    m.resize(XX);
    for(int i = 0; i < XX; ++i)
    {
        m[i].resize(YY);
        for(int j = 0; j < YY; ++j)
        {
            file_in >> m[i][j];
        }
    }
}

struct vertex{
    int x, y, x_from, y_from;
    double g_value, h_value, f_value;
};

    bool operator < (const vertex &vertex_first,const vertex &vertex_second){
    return vertex_first.f_value < vertex_second.f_value;
    }

    bool valid(int x, int y){
        if (x < 0 || y < 0 || x >= XX || y >= YY){
            return false;
        }
        else return true;
    }

    bool dest(int x, int y, vertex vertex_dest){
        if (x == vertex_dest.x && y == vertex_dest.y){
            return true;
        }
        return false;
    }

    double calc_h(int x, int y, vertex vertex_dest){
        double dx = abs(x - vertex_dest.x);
        double dy = abs(y - vertex_dest.y);
        double h = sqrt(dx*dx+dy*dy);
        return h;
    }


    static vector<vertex> shortest_path(vector<vector<vertex>> calculated_dist, vertex vertex_dest){

        int x = vertex_dest.x;
        int y = vertex_dest.y;
        vector<vertex> path;
        vector<vertex> best_path;

        while (!(calculated_dist[x][y].x_from == x && calculated_dist[x][y].y_from == y) && calculated_dist[x][y].x != -1 && calculated_dist[x][y].y != -1){
            path.push_back(calculated_dist[x][y]);
            int temp_x = calculated_dist[x][y].x_from;
            int temp_y = calculated_dist[x][y].y_from;
            x = temp_x;
            y = temp_y;
        }
        path.push_back(calculated_dist[x][y]);

        while (!path.empty()) {
                vertex top = path[path.size()];
                path.pop_back();
                best_path.emplace_back(top);
            }
            for(auto i : best_path){
                gout << move_to(i.x,i.y) << color(255,0,0) << dot;
            }
            cout << "Shortest path:" << best_path.size() << endl;
            return best_path;
        }

    vector<vertex> acsillag(vertex vertex_current, vertex vertex_dest, map_to_read &m){

    vector<vertex> empty;

    if ((valid(vertex_dest.x, vertex_dest.y) == false) || m[vertex_dest.x][vertex_dest.y] < 0){
        cout << "Bad destination given" << endl;
        return empty;
    }

    if(dest(vertex_current.x, vertex_current.y, vertex_dest)){
        cout << "You are at the destination" << endl;
        return empty;
    }

    if((valid(vertex_current.x, vertex_current.y) == false) || m[vertex_current.x][vertex_current.y] < 0){
        cout << "You are starting from the sea" << endl;
        return empty;
    }

    bool closed[XX][YY];

    vector<vector<vertex>> starting_pos;

    starting_pos.resize(XX);
    for (int x = 0; x < XX; ++x) {
        starting_pos[x].resize(YY);
        for (int y = 0; y < YY; ++y) {
            starting_pos[x][y].f_value = FLT_MAX;
            starting_pos[x][y].g_value = FLT_MAX;
            starting_pos[x][y].h_value = FLT_MAX;
            starting_pos[x][y].x_from = -1;
            starting_pos[x][y].y_from = -1;
            starting_pos[x][y].x = x;
            starting_pos[x][y].y = y;
            closed[x][y] = false;
            }
    }

    int x = vertex_current.x;
    int y = vertex_current.y;
    starting_pos[x][y].f_value = 0;
    starting_pos[x][y].g_value = 0;
    starting_pos[x][y].h_value = 0;
    starting_pos[x][y].x_from = x;
    starting_pos[x][y].y_from = y;

    vector<vertex> open;
    open.emplace_back(starting_pos[x][y]);
 //   bool celnal = false;
    unsigned int max_size = XX*YY;

    while(!open.empty() && open.size()< max_size){
        vertex current_vertex;
        do {
            float temp = FLT_MAX;
            vector<vertex>::iterator vertex_iterator;
            for (vector<vertex>::iterator it = open.begin();
                    it != open.end(); ++it) {
                    vertex n = *it;
                    if (n.f_value < temp) {
                        temp = n.f_value;
                        vertex_iterator = it;
                    }
                }
                current_vertex = *vertex_iterator;
                open.erase(vertex_iterator);
        }
        while (valid(current_vertex.x, current_vertex.y) == false);


        x = current_vertex.x;
        y = current_vertex.y;

        closed[x][y] = true;

        for (int x_coord = -1; x_coord <= 1; x_coord++) {
                for (int y_coord = -1; y_coord <= 1; y_coord++) {
                    double g_value_new, h_value_new, f_value_new;
                    if ((valid(x + x_coord, y + y_coord)) && m[x + x_coord][y + y_coord] > 0){
                        if (dest(x + x_coord, y + y_coord, vertex_dest)){
                            starting_pos[x + x_coord][y + y_coord].x_from = x;
                            starting_pos[x + x_coord][y + y_coord].y_from = y;
                          //  celnal = true;
                            return shortest_path(starting_pos, vertex_dest);
                        }
                        else if (closed[x + x_coord][y + y_coord] == false)
                        {
                            g_value_new = current_vertex.g_value +1.0;
                            h_value_new = calc_h(x + x_coord, y + y_coord, vertex_dest);
                            f_value_new = g_value_new + h_value_new;
                            if (starting_pos[x + x_coord][y + y_coord].f_value == FLT_MAX || starting_pos[x + x_coord][y + y_coord].f_value > f_value_new)
                            {
                                starting_pos[x + x_coord][y + y_coord].f_value = f_value_new;
                                starting_pos[x + x_coord][y + y_coord].g_value = g_value_new;
                                starting_pos[x + x_coord][y + y_coord].h_value = h_value_new;
                                starting_pos[x + x_coord][y + y_coord].x_from = x;
                                starting_pos[x + x_coord][y + y_coord].y_from = y;
                                open.emplace_back(starting_pos[x + x_coord][y + y_coord]);
                            }
                        }
                    }
                }
            }
        }
        cout << "Destination can not be reached on land" << endl;
            return empty;
    }


void draw_map(map_to_read &t) {

    for(size_t i = 0; i < t.size(); ++i)
    {
        for(size_t j = 0; j < t[i].size(); ++j)
        {
            int r,g,b;
            int c = 127+t[i][j];
            g = c;
            if(t[i][j] <= 0) {
                b = 255+t[i][j];
                g = c;
                r = 0;
            }
            else b = c/2;
            gout << move_to(i,j) << color(r,g,b) << dot;
        }
    }
    gout << refresh;
}

int main() {

  map_to_read m;
  read(m);
  vertex vertex_from,vertex_to;

  gout.open(XX, YY);
  draw_map(m);
  gout << refresh;

  event ev;

  while (gin >> ev && ev.keycode != key_escape) {
        gout << refresh;

    if(ev.type == ev_mouse && ev.button == btn_left){
        vertex_from.x = ev.pos_x, vertex_from.y =ev.pos_y;
    }
    if(ev.type == ev_mouse && ev.button == btn_right){
        vertex_to.x = ev.pos_x, vertex_to.y =ev.pos_y;
    }
    if(ev.type == ev_key && ev.keycode == key_enter){
        //kirajzol(t);
        acsillag(vertex_from, vertex_to, m);
    }

  }

  return 0;
}
