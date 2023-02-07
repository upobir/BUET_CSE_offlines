# pragma once

class ProgressBar {
public:
    ProgressBar(int totalBars);
    void update(double progress);
private:
    int totalBars;
    int currentBars;
};


ProgressBar::ProgressBar(int totalBars) : totalBars(totalBars), currentBars(0){
}

void ProgressBar::update(double progress){
    int newBars = (int) round(progress * totalBars);
    newBars = std::min(newBars, totalBars);
    while(newBars > currentBars){
        std::cout << "|" << std::flush;
        currentBars++;
        if(currentBars == totalBars){
            std::cout << std::endl;
        }
    }
}

