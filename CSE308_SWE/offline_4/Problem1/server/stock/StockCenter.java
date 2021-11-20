package server.stock;

import java.io.*;
import java.util.*;

public class StockCenter {
    HashMap<String, Stock> stocks;

    private StockCenter() {
        this.stocks = new HashMap<>();
    }

    private static StockCenter stockCenter;

    public static StockCenter getStockCenter() {
        if(stockCenter == null){
            stockCenter = new StockCenter();
        }
        return stockCenter;
    }

    public void readStockData(String inputFileName){
        try{
            File file = new File(inputFileName);

            Scanner scanner = new Scanner(file);

            while(scanner.hasNext()){
                String stockName = scanner.next();
                int stockCount = scanner.nextInt();
                double stockPrice = scanner.nextDouble();
                
                Stock stock = new Stock(stockName, stockCount, stockPrice);

                stocks.put(stock.name, stock);
            }

            scanner.close();

        } catch(FileNotFoundException e) {
            System.err.println("File not found");
            e.printStackTrace();
        }
    }

    public boolean subscribe(User user, String stockName) {
        Stock stock = stocks.get(stockName);
        if(stock == null)
            return false;
        stock.attach(user);
        return true;
    }

    public boolean unsubscribe(User user, String stockName) {
        Stock stock = stocks.get(stockName);
        if(stock == null)
            return false;
        stock.detach(user);
        return true;
    }

    public boolean increaseStockPrice(String stockName, double priceChange){
        Stock stock = stocks.get(stockName);
        if(stock == null)
            return false;
        stock.increasePrice(priceChange);
        return true;
    }

    public boolean decreaseStockPrice(String stockName, double priceChange){
        Stock stock = stocks.get(stockName);
        if(stock == null)
            return false;
        stock.decreasePrice(priceChange);
        return true;
    }

    public boolean changeStockCount(String stockName, int stockCount){
        Stock stock = stocks.get(stockName);
        if(stock == null)
            return false;
        stock.changeCount(stockCount);
        return true;
    }

    public Collection<Stock> getStocks() {
        return stocks.values();
    }
}
