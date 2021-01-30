#include <iostream>
#include <string>
#include <thread>
#include <random>
#include <cmath>
#include <vector>
#include <ctime>

#pragma warning(disable : 4996)


class FeedforwardANN {
public:
    FeedforwardANN(std::vector<int> layers_struct):delta(0,1), re(time(0)), sf(0.001) {
        layers.resize(layers_struct.size());
        layers[0].size = layers_struct[0];
        layers[0].nodes.resize(layers[0].size);
        for(int i = 1; i < layers_struct.size(); i++) {
            layers[i].size = layers_struct[i];
            layers[i].nodes.resize(layers[i].size);
            layers[i].biases.resize(layers[i].size, 0);
            layers[i].weights.resize(layers[i].size);
            for(int k = 0; k < layers_struct[i]; k++) {
                layers[i].weights[k].resize(layers[i - 1].size, 0);
            }
        }
    }
    FeedforwardANN(const FeedforwardANN& source): delta(0,1), re(time(0)), sf(0.001) {
        size = source.size;
        layers = source.layers;
    }
    void operator= (const FeedforwardANN& source) {
        size = source.size;
        layers = source.layers;
        threads.resize(source.threads.size());
    }

    void randomize() {
        double lower_bound = -0.6;
        double upper_bound = 0.6;
        std::uniform_real_distribution<double> delta(lower_bound,upper_bound);
        std::default_random_engine re(time(0));
        for(int i = 1; i < layers.size(); i++) {
            for(int k = 0; k < layers[i].size; k++) {
                for(int j = 0; j < layers[i].weights[k].size(); j++) {
                    layers[i].weights[k][j] = delta(re);
                }
            }
            for(int j = 0; j < layers[i].biases.size(); j++) {
                layers[i].biases[j] = delta(re);
            }
        }
    }

    void mutate(double cost) {
        srand(clock());
        double step_size = sf * fmax(cost, 1);
        std::cauchy_distribution<double> delta(0,1);
        std::default_random_engine re(time(0));
        for(int i = 1; i < layers.size(); i++) {
            for(int k = 0; k < layers[i].size; k++) {
                for(int j = 0; j < layers[i].weights[k].size(); j++) {
                    if (rand() % 20 == 0)
                        layers[i].weights[k][j] += step_size * delta(re);
                }
            }
            for(int j = 0; j < layers[i].biases.size(); j++) {
                if (rand() % 20 == 0)
                    layers[i].biases[j] += step_size * delta(re);
            }
        }
    }

    int output_size(){
        return layers[layers.size() - 1].size;
    }

    void print(){
        for(int i = 1; i < layers.size(); i++) {
            std::cout << i << '\n';
            for(int k = 0; k < layers[i].size; k++) {
                for(int j = 0; j < layers[i].weights[k].size(); j++) {
                    std::cout << layers[i].weights[k][j] << ' ';
                }
                std::cout << '\n';
            }
            for(int j = 0; j < layers[i].biases.size(); j++) {
                std::cout << layers[i].biases[j] << ' ';
            }
            std::cout << '\n';
        std::cout << '\n';
        }
    }
    
    std::vector<double> process(std::vector<double> input) {
        layers[0].nodes = input;
        auto process_one_node = [&](int layer_counter, int node_counter) {
            double ans = 0;
            for(int i = 0; i < layers[layer_counter - 1].size; i++) {
                ans += layers[layer_counter].weights[node_counter][i] * layers[layer_counter - 1].nodes[i];
            }
            //layers[layer_counter].nodes[node_counter] = 1 / (1 +  exp(-(ans + layers[layer_counter].biases[node_counter])));
            layers[layer_counter].nodes[node_counter] = fmax(0, ans + layers[layer_counter].biases[node_counter]);
        };
        // std::vector<std::thread> processes(80);
        // int thread_c = 0;
        // for(int i = 1; i < layers.size(); i++) {
        //     for(int k = 0; k < layers[i].size; k++) {
        //         //std::cout << thread_c <<  "ddd\n";
        //         if (processes[thread_c % 100].joinable()) {
        //             processes[thread_c % 100].join();
        //             processes[thread_c++ % 100]  = std::thread(process_one_node, i, k);
        //         } else {
        //             processes[thread_c++ % 100]  = std::thread(process_one_node, i, k);
        //         }
        //     }
        // }
        // for(int i = 0; i < processes.size(); i++) {
        //     if (processes[i].joinable())
        //         processes[i].join();
        // }

        for(int i = 1; i < layers.size(); i++) {
            for(int k = 0; k < layers[i].size; k++) {
                process_one_node(i, k);
            }
        }

        return layers[layers.size() - 1].nodes;
    }

    void save(std::string path) {
        FILE* stor = fopen(path.c_str(), "wb");
        fwrite(&size, sizeof(size_t), 1, stor);
        int size_of = threads.size();
        fwrite(&size_of, sizeof(int), 1, stor);
        size_of = layers.size();
        fwrite(&size_of, sizeof(int), 1, stor);
        for(int i = 0; i < layers.size(); i++ ) {
            size_of = layers[i].weights.size();
            fwrite(&size_of, sizeof(int), 1, stor);
            for (int k = 0; k < layers[i].weights.size(); k++) {
                int size_of_w = layers[i].weights[k].size();
                fwrite(&size_of_w, sizeof(int), 1, stor);
                for (int j = 0; j < size_of_w; j++) {
                    fwrite(&layers[i].weights[k][j], sizeof(double), 1, stor);
                }
            }
            size_of = layers[i].biases.size();
            fwrite(&size_of, sizeof(int), 1, stor);
            for (int k = 0; k < size_of; k++) {
                fwrite(&layers[i].biases[k], sizeof(double), 1, stor);
            }
            size_of = layers[i].nodes.size();
            fwrite(&size_of, sizeof(int), 1, stor);
            for (int k = 0; k < size_of; k++) {
                fwrite(&layers[i].nodes[k], sizeof(double), 1, stor);
            }
            fwrite(&layers[i].size, sizeof(size_t), 1, stor);
        }
        fclose(stor);
    }
    void load(std::string path) {
        FILE* stor = fopen(path.c_str(), "rb");
        fread(&size, sizeof(size_t), 1, stor);
        int size_of;
        fread(&size_of, sizeof(int), 1, stor);
        threads = std::vector<std::thread>(size_of);
        fread(&size_of, sizeof(int), 1, stor);
        layers = std::vector<Layer>(size_of);
        for(int i = 0; i < layers.size(); i++ ) {
            fread(&size_of, sizeof(int), 1, stor);
            layers[i].weights = std::vector<std::vector<double>>(size_of);
            for (int k = 0; k < layers[i].weights.size(); k++) {
                int size_of_w;
                fread(&size_of_w, sizeof(int), 1, stor);
                layers[i].weights[k] = std::vector<double>(size_of_w);
                for (int j = 0; j < size_of_w; j++) {
                    fread(&layers[i].weights[k][j], sizeof(double), 1, stor);
                }
            }
            fread(&size_of, sizeof(int), 1, stor);
            layers[i].biases = std::vector<double>(size_of);
            for (int k = 0; k < size_of; k++) {
                fread(&layers[i].biases[k], sizeof(double), 1, stor);
            }
            fread(&size_of, sizeof(int), 1, stor);
            layers[i].nodes = std::vector<double>(size_of);
            for (int k = 0; k < size_of; k++) {
                fread(&layers[i].nodes[k], sizeof(double), 1, stor);
            }
            fread(&layers[i].size, sizeof(size_t), 1, stor);

        }
        fclose(stor);
    }

private:
    class Layer {
        public:
            std::vector<std::vector<double>> weights;
            std::vector<double> biases;
            std::vector<double> nodes;
            size_t size;
    };
    std::cauchy_distribution<double> delta;
    std::default_random_engine re;
    double sf;

    size_t size;
    std::vector<Layer> layers;
    std::vector<std::thread> threads;
};


template<class I, class O>
void RandomHillClimbing(FeedforwardANN& source,I input, O output, double expected_acc, int size_for_fitness = 100, int itres_per_one_set = -1, bool is_rand = true) {
    FeedforwardANN current(source);
    if (is_rand)
        current.randomize();
    FeedforwardANN step(current);

    std::vector<double> res;
    std::vector<double> data;

    double cost = 0;
    double step_cost = 0;

    for(int k = 0; k < size_for_fitness; k++) {
        res = current.process(input());
        data = output();
        for(int j = 0; j < res.size(); j++) {
            cost += pow(res[j] - data[j], 2);
        }
    }

    int count;
    int pass;

    int set_counter = 1;

    while(true) {
        step = current;
        step.mutate(cost / size_for_fitness);
        count = 0;
        pass = 0;

        step_cost = 0;
        for(int k = 0; k < size_for_fitness; k++) {
            res = step.process(input());
            data = output();
            for(int j = 0; j < res.size(); j++) {
                step_cost += pow(res[j] - data[j], 2);
            }
            // test acc
            // std::pair<int, double> max_d(0, data[0]);
            // for(int j = 0; j < data.size(); j++) {
            //     if (max_d.second < data[j]) {
            //         max_d.second = data[j];
            //         max_d.first = j;
            //     }
            // }
            // std::pair<int, double> max_r(0, res[0]);
            // for(int j = 0; j < res.size(); j++) {
            //     if (max_r.second < res[j]) {
            //         max_r.second = res[j];
            //         max_r.first = j;
            //     }
            // }
            // count++;
            // if (max_r.first == max_d.first) {
            //     pass++;
            // }
        }
        if (step_cost < cost) {
            current = step;
            cost = step_cost;
        }

        std::cout << '\n' << set_counter << "'s itter";
        std::cout << "Step Cost:" << step_cost << '\n';
        std::cout << "Curr Accuracy: " << 100 - (cost / (size_for_fitness * source.output_size())) * 100 << '\n';
        std::cout << "Step Accuracy: " << 100 - (step_cost / (size_for_fitness * source.output_size())) * 100 << '\n';
        //std::cout << "Norm Accuracy: " << (double)pass / count * 100 << '\n';
        

        if (expected_acc < (100 - (cost/ (size_for_fitness * source.output_size()) * 100))) {
            source = current;
            break;
        }
        if (itres_per_one_set > 0) {
            current.save("Autosave.ann");
            set_counter++;
            if (itres_per_one_set == set_counter) {
                cost = 0;
                for(int k = 0; k < size_for_fitness; k++) {
                    res = current.process(input());
                    data = output();
                    for(int j = 0; j < res.size(); j++) {
                        cost += pow(res[j] - data[j], 2);
                    }
                    
                }
                std::cout << "\n\nNew Set\n\n"; 
                set_counter = 1;
            } 
        }
    }
}

template<class F>
void FitnessRandomHillClimbing(FeedforwardANN& source,F fit_func, double expected_fitness, int size_for_fitness = 100, bool is_rand = false) {
    FeedforwardANN current(source);
    if (is_rand)
        current.randomize();
    FeedforwardANN step(current);

    std::vector<double> res;

    double fitness = 0;
    double step_fitness = 0;

    for(int k = 0; k < size_for_fitness; k++) {
        std::cout << "summ pre" << fitness << std::endl;
        double aaa = fit_func(current);
        std::cout << "what" << aaa << std::endl;
        fitness += aaa;
        std::cout << "summ" <<  fitness << std::endl;
    }

    int count;
    int pass;

    int set_counter = 1;

    while(true) {
        step = current;
        step.mutate(5 / (fitness) + 0.5);
        count = 0;
        pass = 0;

        step_fitness = 0;
        for(int k = 0; k < size_for_fitness; k++) {
            step_fitness += fit_func(step);
        }
        if (step_fitness > fitness) {
            current = step;
            fitness = step_fitness;
        }

        std::cout << '\n' << set_counter << "'s itter\n";
        std::cout << "Curr Fitness: " << fitness << '\n';
        std::cout << "Step Fitness:" << step_fitness << '\n';
        

        if (expected_fitness < fitness) {
            source = current;
            break;
        }
        current.save("Autosave.ann");
        set_counter++;
    }
}

template<class I, class O>
void MultyRandomHillClimbing(FeedforwardANN& source,I input, O output, double expected_acc, int start_number, int size_for_fitness = 100) {
    std::vector<FeedforwardANN> current(start_number, source);
    std::vector<FeedforwardANN> step(start_number, source);
    //FeedforwardANN step(source);

    std::vector<double> res;
    std::vector<double> data;

    std::vector<double> cost(start_number, 0);
    std::vector<double> step_cost(start_number, 0);

    for(int i = 1; i < current.size(); i++) {
        current[i].randomize();
    }

    for(int i = 0; i < current.size(); i++) {
        for(int k = 0; k < size_for_fitness; k++) {
            res = current[i].process(input());
            data = output();
            for(int j = 0; j < res.size(); j++) {
                cost[i] += pow(res[j] - data[j], 2);
                //std::cout << pow(res[j] - data[j], 2) << '\n';
            }
        }
    }


    while(true) {
        std::string out_str;
        for(int i = 1; i < current.size(); i++) {
            step[i] = current[i];
            step[i].mutate(cost[i] / size_for_fitness);
            step_cost[i] = 0;
            for(int k = 0; k < size_for_fitness; k++) {
                res = step[i].process(input());
                data = output();
                for(int j = 0; j < res.size(); j++) {
                    step_cost[i] += pow(res[j] - data[j], 2);
                }
            }
            if (step_cost[i] < cost[i]) {
                current[i] = step[i];
                cost[i] = step_cost[i];
            }
            out_str += std::to_string(cost[i]) + '\n';
            
        }

        double min_cost = cost[0];
        for(int k = 0; k < cost.size(); k++) {
            if (cost[k] < min_cost) {
                min_cost = cost[k];
            }
        }
        std::cout << out_str << '\n';
        std::cout << min_cost << '\n';
        std::cout << "Best Accuracy: " << 100 - (min_cost / (size_for_fitness * source.output_size())) * 100 << '\n' << '\n';

        current[0].save("Autosave.ann");

        if (expected_acc < (100 - (min_cost/ (size_for_fitness * source.output_size()) * 100))) {
            std::pair<int, double> min_cost(0, cost[0]);
            for(int i = 1; i < current.size(); i++) {
                if(cost[i] <= min_cost.second) {
                    min_cost.first = i;
                    min_cost.second = cost[i];
                }
            }
            source = current[min_cost.first];
            break;
        }
    }
}
