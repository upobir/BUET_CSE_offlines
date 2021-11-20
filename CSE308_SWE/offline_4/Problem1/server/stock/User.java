package server.stock;

import java.net.Socket;
import java.util.HashSet;

import server.*;
import server.networking.UserConnection;

public class User implements ObserverInterface {
    private UserConnection connection;
    private HashSet<String> stocks;
    private String name;

    public User(Socket socket) {
        this.stocks = new HashSet<>();
        this.connection = new UserConnection(this, socket);
        this.connection.start();

        System.out.println("client "+ this.name + " connected");

        connection.println("Stocks Available:");
        for(Stock stock : StockCenter.getStockCenter().getStocks()){
            connection.println(stock.getName() + " " + stock.getCount() + " " + stock.getPrice());
        }
    }

    public UserConnection getConnection() {
        return connection;
    }

    @Override
    public void update(SubjectInterface s) {
        Stock stock = (Stock) s;
        connection.println(stock.getName() + " : " + stock.getLastAction());
        connection.println(stock.getName() + " info : count = " + stock.getCount() + ", price = " + stock.getPrice());
    }

    public void update(String command, String stockName) {
        switch(command){
            case "S":{
                if(StockCenter.getStockCenter().subscribe(this, stockName)){
                    stocks.add(stockName);
                }
            } break;

            case "U":{
                if(StockCenter.getStockCenter().unsubscribe(this, stockName)){
                    stocks.remove(stockName);
                }
            } break;

            default:
        }
    }

    public void stop() {
        System.out.println("Client " + name + " disconnected");
        for (String name : stocks){
            StockCenter.getStockCenter().unsubscribe(this, name);
        }
    }

    public void setName(String name) {
        this.name = name;
    }
    
}
