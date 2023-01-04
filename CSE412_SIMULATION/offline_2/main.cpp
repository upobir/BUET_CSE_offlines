#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <iostream>

const int NUM_EVENTS = 4;
const float INF = 1.0e+30;

class InventorySimulator {
public:
    InventorySimulator(int initial_inv_level, std::vector<float> prob_distrib_demand, float mean_interdemand, float setup_cost, float incremental_cost, float holding_cost, float shortage_cost, float minlag, float maxlag) 
        : initial_inv_level(initial_inv_level), 
        prob_distrib_demand(prob_distrib_demand),
        mean_interdemand(mean_interdemand), 
        setup_cost(setup_cost), 
        incremental_cost(incremental_cost), 
        holding_cost(holding_cost), 
        shortage_cost(shortage_cost), 
        minlag(minlag), maxlag(maxlag) {
    }

    void simulate(int num_months, int smalls, int bigs) {
        initialize(num_months);

        Event next_event_type;
        do {
            auto timing_result = timing();
            sim_time = timing_result.first;
            next_event_type = timing_result.second;
            
            update_time_avg_stats();
            switch (next_event_type) {
            case Event::ORDER_ARRIVAL:
                order_arrival();
                break;
            case Event::DEMAND:
                demand();
                break;
            case Event::EVALUATE:
                evaluate(smalls, bigs);
                break;
            case Event::SIM_END:
                compute_avg(num_months);
                break;
            }
        } while (next_event_type != Event::SIM_END);
    }

    float get_avg_holding_cost() const { 
        return avg_holding_cost; 
    }
    float get_avg_ordering_cost() const { 
        return avg_ordering_cost; 
    }
    float get_avg_shortage_cost() const { 
        return avg_shortage_cost; 
    }
    float get_avg_total_cost() const { 
        return avg_holding_cost + avg_ordering_cost + avg_shortage_cost; 
    }


private:
    int initial_inv_level;

    std::vector<float> prob_distrib_demand;

    float mean_interdemand;
    float setup_cost;
    float incremental_cost;
    float holding_cost;
    float shortage_cost;
    float minlag;
    float maxlag;

    std::mt19937 rng;

    enum class Event { NONE = 0, ORDER_ARRIVAL = 1, DEMAND = 2, SIM_END = 3, EVALUATE = 4};

    float sim_time;
    int inv_level;
    float time_last_event;
    float total_ordering_cost;
    float area_holding;
    float area_shortage;
    std::array<float, NUM_EVENTS+1> time_next_event;
    int amount;

    float avg_holding_cost, avg_ordering_cost, avg_shortage_cost;

    void initialize(int num_months){
        sim_time = 0.0;
        inv_level = initial_inv_level;
        time_last_event = 0.0;
        total_ordering_cost = 0.0;
        area_holding = 0.0;
        area_shortage = 0.0;

        time_next_event[int(Event::ORDER_ARRIVAL)] = INF;
        time_next_event[int(Event::DEMAND)] = sim_time + std::exponential_distribution<float>(1.0/mean_interdemand)(rng);
        time_next_event[int(Event::SIM_END)] = num_months;
        time_next_event[int(Event::EVALUATE)] = 0.0;

        return;
    }

    std::pair<float, Event> timing() {
        float min_time_next_event = INF/2;
        Event next_event_type = Event::NONE;
        for (int i = 1; i <= NUM_EVENTS; ++i)
            if (time_next_event[i] < min_time_next_event) {
                min_time_next_event = time_next_event[i];
                next_event_type = Event(i);
            }
        if (next_event_type == Event::NONE) {
            std::cerr << "Event list empty at time " << sim_time << std::endl;
            exit(1);
        }
        return std::make_pair(min_time_next_event, next_event_type);
    }

    void update_time_avg_stats(void) {
        float time_since_last_event = sim_time - time_last_event;
        time_last_event = sim_time;
        if (inv_level < 0)
            area_shortage -= inv_level * time_since_last_event;
        else if (inv_level > 0)
            area_holding += inv_level * time_since_last_event;
        return;
    }

    void order_arrival(void) {
        inv_level += amount;
        time_next_event[int(Event::ORDER_ARRIVAL)] = INF;
    }

    void demand(void) {
        inv_level -= random_integer();
        time_next_event[int(Event::DEMAND)] = sim_time + std::exponential_distribution<float>(1.0/mean_interdemand)(rng);
    }

    void evaluate(int smalls, int bigs) {
        if (inv_level < smalls) {
            amount = bigs - inv_level;
            total_ordering_cost += setup_cost + incremental_cost * amount;
            time_next_event[int(Event::ORDER_ARRIVAL)] = sim_time + std::uniform_real_distribution<float>(minlag, maxlag)(rng);
        }
        time_next_event[int(Event::EVALUATE)] = sim_time + 1.0;
    }

    void compute_avg(int num_months){
        avg_holding_cost = area_holding * holding_cost / num_months;
        avg_ordering_cost = total_ordering_cost / num_months;
        avg_shortage_cost = area_shortage * shortage_cost / num_months;
    }

    int random_integer(){
        int i;
        float u = std::uniform_real_distribution<float>(0.0, 1.0)(rng);
        for (i = 1; u >= prob_distrib_demand[i]; ++i)
            ;
        return i;
    }
};

int main() {
    std::ifstream infile("inv.in");
    std::ofstream outfile("inv.out");

    int initial_inv_level, num_months, num_policies, num_values_demand;
    float mean_interdemand, setup_cost, incremental_cost, holding_cost,
        shortage_cost, minlag, maxlag;

    infile >> initial_inv_level >> num_months >> num_policies >>
        num_values_demand >> mean_interdemand >> setup_cost >>
        incremental_cost >> holding_cost >> shortage_cost >> minlag >>
        maxlag;

    std::vector<float> prob_distrib_demand(num_values_demand + 1);

    for (int i = 1; i <= num_values_demand; ++i)
        infile >> prob_distrib_demand[i];

    outfile << "Single-product inventory system" << std::endl << std::endl;
    outfile << "Initial inventory level" << std::setw(24) << initial_inv_level << " items" << std::endl << std::endl;
    outfile << "Number of demand sizes" << std::setw(25) << num_values_demand << std::endl << std::endl;
    outfile << "Distribution function of demand sizes ";
    for (int i = 1; i <= num_values_demand; ++i)
        outfile << std::setw(8) << std::fixed << std::setprecision(3) << prob_distrib_demand[i];

    outfile << std::endl << std::endl;

    outfile << "Mean interdemand time" << std::setw(26) << std::fixed << std::setprecision(2) << mean_interdemand << std::endl << std::endl;
    outfile << "Delivery lag range" << std::setw(29) << std::fixed << std::setprecision(2) << minlag << " to" << std::setw(10) << std::fixed << std::setprecision(2) << maxlag << " months" << std::endl << std::endl;
    outfile << "Length of the simulation" << std::setw(23) << num_months << " months" << std::endl << std::endl;

    outfile << "K ="<< std::setw(6) << std::fixed << std::setprecision(1) << setup_cost 
        << " i =" << std::setw(6) << std::fixed << std::setprecision(1) 
        << incremental_cost << " h =" << std::setw(6) << std::fixed << std::setprecision(1) 
        << holding_cost << " pi =" << std::setw(6) << std::fixed << std::setprecision(1) << shortage_cost << std::endl << std::endl;

    outfile << "Number of policies" << std::setw(29) << num_policies << std::endl << std::endl;
    outfile << "                   Average      Average         Average       Average" << std::endl;
    outfile << " Policy          total cost  ordering cost   holding cost  shortage cost" << std::endl;

    for (int i = 1; i <= num_policies; ++i)
    {
        int smalls, bigs;

        infile >> smalls >> bigs;

        InventorySimulator simulator(initial_inv_level, prob_distrib_demand, mean_interdemand, 
            setup_cost, incremental_cost, holding_cost, shortage_cost, minlag, maxlag);

        simulator.simulate(num_months, smalls, bigs);

        outfile << std::endl << std::endl;

        outfile <<"(" << std::setw(3) << smalls << "," << std::setw(3) << bigs << ")" 
            << std::setw(15) << std::fixed << std::setprecision(2) << simulator.get_avg_total_cost() 
            << std::setw(15) << std::fixed << std::setprecision(2) << simulator.get_avg_ordering_cost() 
            << std::setw(15) << std::fixed << std::setprecision(2) << simulator.get_avg_holding_cost() 
            << std::setw(15) << std::fixed << std::setprecision(2) << simulator.get_avg_shortage_cost();
    }
    infile.close();
    outfile.close();
    return 0;
}

