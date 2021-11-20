package server.stock;

import java.util.HashSet;

import server.*;

public class Stock implements SubjectInterface {
    String name;
    int count;
    double price;
    String lastAction;

    HashSet<ObserverInterface> subscribers;

    public Stock(String name, int count, double price){
        this.name = name;
        this.count = count;
        this.price = price;
        this.lastAction = "";
        this.subscribers = new HashSet<>();
    }

    public String getName() {
        return name;
    }

    public int getCount() {
        return count;
    }

    public double getPrice() {
        return price;
    }

    public String getLastAction() {
        return lastAction;
    }

    @Override
    public void attach(ObserverInterface o) {
        subscribers.add(o);
        
    }

    @Override
    public void detach(ObserverInterface o) {
        subscribers.remove(o);
    }

    @Override
    public void notifyObservers() {
        
        for(ObserverInterface o : subscribers){
            o.update(this);
        }
        
    }

    public void increasePrice(double priceChange) {
        price += priceChange;
        lastAction = "Price increased by " + priceChange;
        notifyObservers();
    }

    public void decreasePrice(double priceChange) {
        price -= priceChange;
        lastAction = "Price decreased by " + priceChange;
        notifyObservers();
    }

    public void changeCount(int stockCount) {
        count = stockCount;
        lastAction = "Stock count changed to " + stockCount;
        notifyObservers();
    }
}