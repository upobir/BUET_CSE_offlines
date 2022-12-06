#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <queue>

const int Q_LIMIT = 100;
const int NUM_EVENTS = 2;
const float INF = 1.0e+30;

class MM1Simulator {
public:
    MM1Simulator(float _mean_interarrival, float _mean_service) 
        : mean_interarrival(_mean_interarrival), mean_service(_mean_service), rng(std::random_device()()){
    }

    void simulate(int num_delays_required){
        initialize();

        while(num_custs_delayed < num_delays_required){
            auto timing_result = timing();
            sim_time = timing_result.first;
            Event next_event_type = timing_result.second;

            update_time_avg_stats();

            switch(next_event_type){
                case Event::ARRIVAL:
                    arrive();
                    break;
                case Event::DEPARTURE:
                    depart();
                    break;
            }
        }
    }

    float getAvgDelay(){
        return total_of_delays / num_custs_delayed;
    }

    float getExpectedAvgDelay(){
        return mean_service * mean_service / (mean_interarrival - mean_service);
    }

    float getAvgNumInQ(){
        return area_num_in_q / sim_time;
    }

    float getExpectedAvgNumInQ(){
        return mean_service * mean_service / mean_interarrival / (mean_interarrival - mean_service);
    }

    float getServerUtil(){
        return area_server_status / sim_time;
    }

    float getExpectedServerUtil(){
        return mean_service / mean_interarrival;
    }

    float getSimTime(){
        return sim_time;
    }

private:
    enum class State{IDLE = 0, BUSY = 1};
    enum class Event{NONE = 0, ARRIVAL = 1, DEPARTURE = 2};

    float mean_interarrival, mean_service;
    
    float sim_time;
    State server_status;
    float time_last_event;
    int num_custs_delayed;
    float total_of_delays;
    float area_num_in_q;
    float area_server_status;
    float time_next_event[NUM_EVENTS+1];
    std::queue<float> time_arrivals;

    std::mt19937 rng;

    void initialize(){
        sim_time = 0.0;
        server_status = State::IDLE;
        time_last_event = 0.0;
        num_custs_delayed = 0;
        total_of_delays = 0.0;
        area_num_in_q = 0.0;
        area_server_status = 0.0;
        time_arrivals = std::queue<float>();

        time_next_event[int(Event::ARRIVAL)] = sim_time + std::exponential_distribution<float>(1.0/mean_interarrival)(rng);
        time_next_event[int(Event::DEPARTURE)] = INF;
    }

    std::pair<float, Event> timing(){
        float min_time_next_event = INF/2;
        Event next_event_type = Event::NONE;

        for(int i = 1; i <= NUM_EVENTS; i++){
            if(time_next_event[i] < min_time_next_event){
                min_time_next_event = time_next_event[i];
                next_event_type = Event(i);
            }
        }

        if(next_event_type == Event::NONE){
            std::cerr<< "Event List empty at time "<<sim_time<<std::endl;
            exit(1);
        }
        return {min_time_next_event, next_event_type};
    }

    void update_time_avg_stats(){
        float time_since_last_event;
        time_since_last_event = sim_time - time_last_event;
        time_last_event = sim_time;

        area_num_in_q += int(time_arrivals.size()) * time_since_last_event;
        area_server_status += int(server_status) * time_since_last_event;
    }

    void arrive(){
        float delay;
        time_next_event[int(Event::ARRIVAL)] = sim_time + std::exponential_distribution<float>(1.0/mean_interarrival)(rng);

        if(server_status == State::BUSY){
            if(time_arrivals.size()+1 > Q_LIMIT){
                std::cerr<<"Overflow of array time_arrival at time "<<sim_time<<std::endl;
                exit(2);
            }
            time_arrivals.push(sim_time);
        }
        else{
            delay = 0.0;
            total_of_delays += delay;
            ++num_custs_delayed;
            server_status = State::BUSY;
            time_next_event[int(Event::DEPARTURE)] = sim_time + std::exponential_distribution<float>(1.0/mean_service)(rng);
        }
    }

    void depart(){
        float delay;
        if(time_arrivals.empty()){
            server_status = State::IDLE;
            time_next_event[int(Event::DEPARTURE)] = INF;
        }
        else{
            delay = sim_time - time_arrivals.front();
            total_of_delays += delay;
            ++num_custs_delayed;
            time_next_event[int(Event::DEPARTURE)] = sim_time + std::exponential_distribution<float>(1.0/mean_service)(rng);
            time_arrivals.pop();
        }
    }
};

int main(int argc, char** argv){
    std::ifstream infile("mm1.in");
    std::ofstream outfile("mm1.out");

    float mean_interarrival, mean_service;
    int num_delays_required;

    infile >> mean_interarrival >> mean_service >> num_delays_required;

    MM1Simulator simulator(mean_interarrival, mean_service);

    outfile << "Single-server queueing system" << std::endl << std::endl;
    outfile << "Mean interarrival time"<<std::fixed << std::setw(11) << std::setprecision(3) << mean_interarrival << " minutes"<< std::endl << std::endl;
    outfile << "Mean service time"<<std::fixed << std::setw(16) << std::setprecision(3) << mean_service << " minutes"<< std::endl << std::endl;
    outfile << "Number of customers"<<std::fixed << std::setw(14) << num_delays_required << std::endl << std::endl;

    simulator.simulate(num_delays_required);

    outfile << std::endl << std::endl;
    outfile << "Average delay in queue" << std::fixed << std::setw(11) << std::setprecision(3) << simulator.getAvgDelay() << " minutes" << std::endl << std::endl;
    outfile << "Average number in queue" << std::fixed << std::setw(10) << std::setprecision(3) << simulator.getAvgNumInQ()  << std::endl << std::endl;
    outfile << "Server utilization" << std::fixed << std::setw(15) << std::setprecision(3) << simulator.getServerUtil() << std::endl << std::endl;
    outfile << "Time simulation ended" << std::fixed << std::setw(12) << std::setprecision(3) << simulator.getSimTime() << " minutes" << std::endl << std::endl;

    outfile << std::endl << std::endl;
    outfile << "Expected Average delay in queue" << std::fixed << std::setw(11) << std::setprecision(3) << simulator.getExpectedAvgDelay() << " minutes" << std::endl << std::endl;
    outfile << "Expected Average number in queue" << std::fixed << std::setw(10) << std::setprecision(3) << simulator.getExpectedAvgNumInQ()  << std::endl << std::endl;
    outfile << "Expected Server utilization" << std::fixed << std::setw(15) << std::setprecision(3) << simulator.getExpectedServerUtil() << std::endl << std::endl;

    infile.close();
    outfile.close();

    return 0;
}
