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
#include <getopt.h>
#include "P2a.h"
#include "P2random.h"
using namespace std;
void Galaxy::getmode(int argc, char * argv[]) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int cho;
    int opt_index = 0;
    option long_options[] = {
        {"verbose", no_argument, nullptr, 'v'},
        {"median",no_argument, nullptr, 'm'},
        {"general-eval",no_argument, nullptr, 'g'},
        {"watcher", no_argument,       nullptr, 'w'},
        { nullptr, 0,                 nullptr, '\0' }
    };
    while ((cho = getopt_long(argc, argv, "vmgw", long_options, &opt_index)) != -1) {
        switch (cho) {
        case 'v':
            out_v = true;
            break;

        case 'm':
            out_m = true;
            break;

        case 'g':
            out_g = true;
            break;

        case 'w':
            out_w = true;
            break;
        default:
            cerr << "Unknown command line option" << endl;
            exit(1);
        } // switch
    } // while
}
void Galaxy::battle(uint32_t &num_war, uint32_t planetid, vector<planet> &planet_sort, vector<general> &generator) {
    while(!planet_sort[planetid].deploy_jedi.empty() && !planet_sort[planetid].deploy_sith.empty() &&
    planet_sort[planetid].deploy_jedi.top().force <= planet_sort[planetid].deploy_sith.top().force) {
                //deployment& d_sith = planet_sort[planetid].deploy_jedi.top();
                //deployment* d_jedi;
            //if(changet == false) 
                const deployment &j = planet_sort[planetid].deploy_jedi.top();
                const deployment &s = planet_sort[planetid].deploy_sith.top();
                gernid_s = s.gernID;
                gernid_j = j.gernID;
                //store the gernid of the war first for s and second for j

                if(j.troopsize <= s.troopsize) {
                    total_loss = 2 * j.troopsize;
                    s.troopsize -= j.troopsize;
                    generator[planet_sort[planetid].deploy_jedi.top().gernID].all_loss += j.troopsize;
                    generator[planet_sort[planetid].deploy_sith.top().gernID].all_loss += j.troopsize;
                    if(out_m) {
                        planet_loss = 2 * j.troopsize;
                        if(planet_sort[planetid].median_smalls.empty()) {
                            planet_sort[planetid].median_smalls.push(planet_loss);
                        } else {
                            if(planet_loss >= planet_sort[planetid].median_smalls.top()) {
                              planet_sort[planetid].median_bigs.push(planet_loss);
                            } else if(planet_loss < planet_sort[planetid].median_smalls.top()) {
                             planet_sort[planetid].median_smalls.push(planet_loss);
                            } 
                        }
                        if(planet_sort[planetid].median_smalls.size() - planet_sort[planetid].median_bigs.size() == 2) {
                            planet_sort[planetid].median_bigs.push(planet_sort[planetid].median_smalls.top());
                            planet_sort[planetid].median_smalls.pop();
                        } else if(planet_sort[planetid].median_bigs.size() - planet_sort[planetid].median_smalls.size() == 2) {
                            planet_sort[planetid].median_smalls.push(planet_sort[planetid].median_bigs.top());
                            planet_sort[planetid].median_bigs.pop();
                        }
                    }
                    planet_sort[planetid].deploy_jedi.pop();
                    //planet_sort[planetid].deploy_sith.top().troopsize = s.troopsize;
                    if(s.troopsize == 0) {
                        planet_sort[planetid].deploy_sith.pop();
                    }
                } else {
                    total_loss = 2 * s.troopsize;
                    j.troopsize -= s.troopsize;
                    generator[planet_sort[planetid].deploy_jedi.top().gernID].all_loss += s.troopsize;
                    generator[planet_sort[planetid].deploy_sith.top().gernID].all_loss += s.troopsize;
                    if(out_m) {
                        planet_loss = 2 * s.troopsize;
                        if(planet_sort[planetid].median_smalls.empty()) {
                            planet_sort[planetid].median_smalls.push(planet_loss);
                        } else {
                            if(planet_loss >= planet_sort[planetid].median_smalls.top()) {
                              planet_sort[planetid].median_bigs.push(planet_loss);
                            } else if(planet_loss < planet_sort[planetid].median_smalls.top()) {
                             planet_sort[planetid].median_smalls.push(planet_loss);
                            } 
                        }
                        if(planet_sort[planetid].median_smalls.size() - planet_sort[planetid].median_bigs.size() == 2) {
                            planet_sort[planetid].median_bigs.push(planet_sort[planetid].median_smalls.top());
                            planet_sort[planetid].median_smalls.pop();
                        } else if(planet_sort[planetid].median_bigs.size() - planet_sort[planetid].median_smalls.size() == 2) {
                            planet_sort[planetid].median_smalls.push(planet_sort[planetid].median_bigs.top());
                            planet_sort[planetid].median_bigs.pop();
                        }
                    }
                    //planet_sort[planetid].median_deploy.push(planet_loss);
                    planet_sort[planetid].deploy_sith.pop();
                }
                num_war++;
                //num_temp_war++;
                if(out_v) {
                    cout<< "General " << gernid_s <<"'s battalion attacked General " <<
                    gernid_j << "'s battalion on planet " << planetid << ". " << total_loss <<
                    " troops were lost.\n"; 
                }
            //}
    }

}
//get median and output when time change.
void Galaxy::getmedian(vector<planet>const &planet_sort, uint32_t num_planets, uint32_t pretime) {
    for(size_t j = 0; j < num_planets; j++) {
        if(!planet_sort[j].median_smalls.empty()) {
            cout << "Median troops lost on planet " << j << " at time " << pretime << " is ";
            if((planet_sort[j].median_bigs.size() + planet_sort[j].median_smalls.size()) % 2 == 0) {
                medianout = (planet_sort[j].median_bigs.top() + planet_sort[j].median_smalls.top()) / 2;
            } else if (planet_sort[j].median_bigs.size() + planet_sort[j].median_smalls.size() == 1) {
                medianout = planet_sort[j].median_smalls.top();
            } else {
                if(planet_sort[j].median_bigs.size() > planet_sort[j].median_smalls.size()) {
                    medianout = planet_sort[j].median_bigs.top();
                } else {
                    medianout = planet_sort[j].median_smalls.top();
                }
            }              
            cout << medianout << ".\n";
        }
    }

}
//output_g
void Galaxy::output_g(vector<general>const &generator, uint32_t num_general) {
    cout << "---General Evaluation---\n";
    for(size_t i = 0; i < num_general; i++) {
        cout << "General " << i << " deployed " << generator[i].all_jedi << " Jedi troops and " << generator[i].all_sith <<
        " Sith troops, and " << (generator[i].all_jedi + generator[i].all_sith - generator[i].all_loss) << "/" << 
        generator[i].all_jedi + generator[i].all_sith  << " troops survived.\n";
    }
}

void Galaxy::read_file() {
    stringstream ss;
    string comment;
    getline(cin, comment);
    string junck;
    cin >> junck >> inputMode;
    cin >> junck >> num_general;
    //cin >> gern >> num_general;
    cin >> junck >> num_planets;
    // inputMode is the "PR" or "DL" from line 2
    if (inputMode == "PR") {  // inputMode is read from line 2 of the header
        uint32_t seed;
        cin >> junck >> seed;
        uint32_t num_deploys;
        cin >> junck >> num_deploys;
        uint32_t rate;
        cin >> junck >> rate;
        P2random::PR_init(ss, seed, num_general, num_planets, num_deploys, rate);

    }  // if ..inputMode

    istream &inputStream = inputMode == "PR" ? ss : cin;

    // Make sure to read an entire deployment in the while statement
    //resize the vector of generate.
    generator.resize(num_general);
    planet_sort.resize(num_planets);
    //
    cout << "Deploying troops...\n";
    while (inputStream >> timestamp >> name >> none >> gernal_inp >> none >> planetid >>
           none >> force_inp >> none >> size_inp) {
        deployment group;
        //planet curplanet;
        // Process this deployment, use PQs, initiate battles (if possible), etc.
        //change the current time to check the time consegutive
        if(timestamp < precur) {
            cerr << "Invalid decreasing timestamp" << endl;
            exit(1);
        } else {
            precur = timestamp;
        }
        //check general id
        if(gernal_inp >= num_general) {
            cerr << "Invalid general ID" << endl;
            exit(1);
        }
        if(force_inp <= 0) {
            cerr << "Invalid force sensntivity level" << endl;
            exit(1);
        }
        if(size_inp <= 0) {
            cerr << "Invalid number of troops" << endl;
            exit(1);
        }
        //check the size of planet and add to the vector
        if(planetid >= num_planets) {
            cerr << "Invalid planet ID" << endl;
            exit(1);
        }
        //watcher
        //check name to store in dif pq
        group.id = numdeploy;
        group.gernID = gernal_inp;
        group.force = force_inp;
        group.troopsize = size_inp;
        numdeploy++;
        if(name == "JEDI") {
            planet_sort[planetid].deploy_jedi.push(group);
            generator[gernal_inp].all_jedi += group.troopsize;
            //curplanet.norm_jedi.push(group);
        } else {
            planet_sort[planetid].deploy_sith.push(group);
            generator[gernal_inp].all_sith += group.troopsize;
            //curplanet.norm_sith.push(group);
        }
        //battle(num_war, planetid, temp, planet_sort, generator);
        //
        //curplanet.all.push_back(group);
        //planet_sort[planetid] = curplanet;
        if(out_w) {
            watcher(planet_sort, planetid, force_inp, name, timestamp);
        }
        if(pretime != timestamp) {
            if(out_m) {
                getmedian(planet_sort, num_planets, pretime);
            }
        }
        pretime = timestamp;
        battle(num_war, planetid, planet_sort, generator);
    }
    if(out_m) {
        getmedian(planet_sort,num_planets,timestamp);
    }
    cout << "---End of Day---\n";
    cout << "Battles: " << num_war <<"\n";
    if(out_g) {
        output_g(generator, num_general);
    }
    if(out_w) {
        cout <<"---Movie Watcher---\n";
        output_w(planet_sort);

    }

}

void Galaxy::watcher(vector<planet> &planet_sort, uint32_t planetid, uint32_t force_inp, string name, uint32_t timestamp) {
    //attack
    if(name == "SITH") {
        //attack sith last
        if(planet_sort[planetid].attack == seenone) {
            if(force_inp >= planet_sort[planetid].watch_attack.j_force) {
                planet_sort[planetid].watch_attack.s_force = force_inp;
                planet_sort[planetid].watch_attack.s_time = timestamp;
                planet_sort[planetid].attack = seenboth;
            }
        } else if(planet_sort[planetid].attack == seenboth) {
            if(force_inp > planet_sort[planetid].watch_attack.s_force) {
                planet_sort[planetid].watch_attack.s_force = force_inp;
                planet_sort[planetid].watch_attack.s_time = timestamp;
            }
        } else if(planet_sort[planetid].attack == maybe) {
            if(force_inp > planet_sort[planetid].watch_attack.maybe_force) {
                if((force_inp - planet_sort[planetid].watch_attack.maybe_force) > (planet_sort[planetid].watch_attack.s_force - 
                planet_sort[planetid].watch_attack.j_force)) {
                    planet_sort[planetid].watch_attack.j_force = planet_sort[planetid].watch_attack.maybe_force;
                    planet_sort[planetid].watch_attack.j_time = planet_sort[planetid].watch_attack.maybe_time;
                    planet_sort[planetid].watch_attack.s_force = force_inp;
                    planet_sort[planetid].watch_attack.s_time = timestamp;
                    //planet_sort[planetid].watch_ambush.maybe_force = 0;
                    //planet_sort[planetid].watch_ambush.maybe_time = 0;
                    planet_sort[planetid].attack = seenboth;
                }
            }  
        }
        //ambush sith first
        if(planet_sort[planetid].ambush == initial) {
            planet_sort[planetid].watch_ambush.s_force = force_inp;
            planet_sort[planetid].watch_ambush.s_time = timestamp;
            planet_sort[planetid].ambush = seenone;
        } else if(planet_sort[planetid].ambush == seenone) {
            //better sith
            if(force_inp > planet_sort[planetid].watch_ambush.s_force) {
                planet_sort[planetid].watch_ambush.s_force = force_inp;
                planet_sort[planetid].watch_ambush.s_time = timestamp;
            }
        } else if(planet_sort[planetid].ambush == seenboth) {
            if(force_inp > planet_sort[planetid].watch_ambush.s_force) {
                planet_sort[planetid].watch_ambush.maybe_force = force_inp;
                planet_sort[planetid].watch_ambush.maybe_time = timestamp;
                planet_sort[planetid].ambush = maybe;
            }
        //maybe states
        } else {
            if(force_inp > planet_sort[planetid].watch_ambush.s_force) {
                if(force_inp > planet_sort[planetid].watch_ambush.maybe_force) {
                    planet_sort[planetid].watch_ambush.maybe_force = force_inp;
                    planet_sort[planetid].watch_ambush.maybe_time = timestamp;
                }
            }
        }
    } else {//jedi
        if(planet_sort[planetid].attack == initial) {
            planet_sort[planetid].watch_attack.j_force = force_inp;
            planet_sort[planetid].watch_attack.j_time = timestamp;
            planet_sort[planetid].attack = seenone;
        } else if(planet_sort[planetid].attack == seenone) {
            //better jedi
            if(force_inp < planet_sort[planetid].watch_attack.j_force) {
                planet_sort[planetid].watch_attack.j_force = force_inp;
                planet_sort[planetid].watch_attack.j_time = timestamp;
            }
        } else if(planet_sort[planetid].attack == seenboth) {
            if(force_inp < planet_sort[planetid].watch_attack.j_force) {
                planet_sort[planetid].watch_attack.maybe_force = force_inp;
                planet_sort[planetid].watch_attack.maybe_time = timestamp;
                planet_sort[planetid].attack = maybe;
            }
        } else { //maybe
            if(force_inp < planet_sort[planetid].watch_attack.j_force) {
                if(planet_sort[planetid].watch_attack.maybe_force > force_inp) {
                    planet_sort[planetid].watch_attack.maybe_force = force_inp;
                    planet_sort[planetid].watch_attack.maybe_time = timestamp;
                }
            }
        }
        //ambush
        if(planet_sort[planetid].ambush == seenone) {
            if(force_inp <= planet_sort[planetid].watch_ambush.s_force) {
                planet_sort[planetid].watch_ambush.j_force = force_inp;
                planet_sort[planetid].watch_ambush.j_time = timestamp;
                planet_sort[planetid].ambush = seenboth;
            }
        } else if(planet_sort[planetid].ambush == seenboth) {
            if(force_inp < planet_sort[planetid].watch_ambush.j_force) {
                planet_sort[planetid].watch_ambush.j_force = force_inp;
                planet_sort[planetid].watch_ambush.j_time = timestamp;
            }
        } else if(planet_sort[planetid].ambush == maybe) {
            if((planet_sort[planetid].watch_ambush.maybe_force - force_inp) > (planet_sort[planetid].watch_ambush.s_force - 
                    planet_sort[planetid].watch_ambush.j_force)) {
                planet_sort[planetid].watch_ambush.s_force = planet_sort[planetid].watch_ambush.maybe_force;
                planet_sort[planetid].watch_ambush.s_time = planet_sort[planetid].watch_ambush.maybe_time;
                planet_sort[planetid].watch_ambush.j_force = force_inp;
                planet_sort[planetid].watch_ambush.j_time = timestamp;
                //planet_sort[planetid].watch_ambush.maybe_force = 0;
                //planet_sort[planetid].watch_ambush.maybe_time = 0;
                planet_sort[planetid].ambush = seenboth;
            }
        }

    }
    
}
void Galaxy::output_w(vector<planet> const &planet_sort) {
    for(size_t i = 0; i < planet_sort.size(); i++) {
        if(planet_sort[i].ambush == seenboth || planet_sort[i].ambush == maybe) {
            cout << "A movie watcher would enjoy an ambush on planet " << i <<
            " with Sith at time " << planet_sort[i].watch_ambush.s_time << 
            " and Jedi at time " << planet_sort[i].watch_ambush.j_time << 
            " with a force difference of " << 
            (planet_sort[i].watch_ambush.s_force - planet_sort[i].watch_ambush.j_force) << ".\n";

        } else {
            cout << "A movie watcher would not see an interesting ambush on planet " << i <<".\n";
        }
        if(planet_sort[i].attack == seenboth || planet_sort[i].attack == maybe) {
            cout << "A movie watcher would enjoy an attack on planet " << i <<
            " with Jedi at time " << planet_sort[i].watch_attack.j_time << 
            " and Sith at time " << planet_sort[i].watch_attack.s_time <<
            " with a force difference of " << 
            (planet_sort[i].watch_attack.s_force - planet_sort[i].watch_attack.j_force) << ".\n";
        } else {
            cout << "A movie watcher would not see an interesting attack on planet " << i <<".\n";
        }
    }
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    Galaxy sophy;
    sophy.getmode(argc, argv);
    sophy.read_file();
    return 0;
}