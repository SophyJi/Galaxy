// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;
class Galaxy {
    public:

    // choices
    bool out_v = false;
    bool out_m = false;
    bool out_g = false;
    bool out_w = false;
    
    //read
    //char mode[6];
    string inputMode;
    uint32_t num_general;
    uint32_t num_planets;
    uint32_t timestamp;
    uint32_t pretime = 0;
    uint32_t precur = 0;
    char none;
    string name;
    uint32_t planetid;
    uint32_t numdeploy = 0;
    //bool changet = false; // time change
    
    uint32_t gernal_inp;
    uint32_t force_inp;
    uint32_t size_inp;
    //char start;
    uint32_t num_war = 0;
    uint32_t num_time = 0;
    uint32_t medianout = 0;
    uint32_t gernid_s;
    uint32_t gernid_j;
    uint32_t total_loss;

    struct deployment { //the 
        uint32_t id;
        uint32_t gernID;
        uint32_t force;
        mutable uint32_t troopsize;
        
    };

    struct sith_compare {
        bool operator()(deployment const& p1, deployment const& p2){
            // return "true" if "p1" is ordered
            //same then compare the id(time)
            if (p1.force == p2.force) {
                return p1.id > p2.id;
            } else {
                return p1.force > p2.force;
            }
        }
    };

    struct jedi_compare {
        bool operator()(deployment const& p1, deployment const& p2){
            // return "true" if "p1" is ordered
            //same then compare the id(time)
            if (p1.force == p2.force) {
                return p1.id > p2.id;
            } else {
                return p1.force < p2.force;
            }
        }
    };
    //struct med {
    uint32_t planet_loss;
    
    struct watch{
        uint32_t s_time;
        uint32_t j_time;
        uint32_t s_force;
        uint32_t j_force;
        uint32_t maybe_force;
        uint32_t maybe_time;

    };
    enum states {initial, seenone, seenboth, maybe};
    struct planet {
        priority_queue<deployment, vector<deployment>, jedi_compare> deploy_sith;
        priority_queue<deployment, vector<deployment>, sith_compare> deploy_jedi;
        
        priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> median_bigs;
        priority_queue<uint32_t, vector<uint32_t>> median_smalls;
        //vector<uint32_t> outmedian;
        watch watch_attack;
        watch watch_ambush;
        states attack = initial;
        states ambush = initial;
    };
    //for v out put
    
    struct general{
        uint32_t all_sith = 0;
        uint32_t all_jedi = 0;
        uint32_t all_loss = 0;
    };
    //need a vector to store the sith and jedi troops.
    vector<general> generator;
    vector<planet> planet_sort;
    // a vector of pair store the loss.
    //functions
    void getmode(int argc, char * argv[]);
    void read_file();
    void battle(uint32_t &num_war, uint32_t planetid, vector<planet> &planet_sort,
                vector<general> &generator);
    void output_g(vector<general> const &generator, uint32_t num_general);
    void getmedian(vector<planet> const &planet_sort, uint32_t num_planets, uint32_t pretime);
    //void output_m(uint32_t num_war,uint32_t num_time, uint32_t num_planets, vector<uint32_t> timechange, vector<planet> planet_sort);
    //bool is_sith = true;
    void watcher(vector<planet> &planet_sort, uint32_t planetid, uint32_t force_inp, string name, uint32_t timestamp);
    void output_w(vector<planet> const &planet_sort);
    
};