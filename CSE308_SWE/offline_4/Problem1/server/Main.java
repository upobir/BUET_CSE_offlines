package server;

import java.util.Scanner;

import server.networking.StockServer;
import server.stock.*;

public class Main{
    public static void main(String[] args) {
        StockCenter stockCenter = StockCenter.getStockCenter();
        stockCenter.readStockData("stockdata.txt");

        System.out.println("Stocks:");
        for(Stock stock : stockCenter.getStocks()){
            System.out.println(stock.getName() + " " + stock.getCount() + " " + stock.getPrice());

        }
        StockServer server = new StockServer();
        server.start();

        Scanner scanner = new Scanner(System.in);

        mainloop: while (true) {
            String command = scanner.next();
            

            switch(command){
                case "I":{
                    String stockName = scanner.next();
                    double priceChange = scanner.nextDouble();
                    if(stockCenter.increaseStockPrice(stockName, priceChange))
                        System.out.println("Stock updated");
                    else
                        System.err.println("Invalid stock name");
                }
                break;

                case "D":{
                    String stockName = scanner.next();
                    double priceChange = scanner.nextDouble();
                    if(stockCenter.decreaseStockPrice(stockName, priceChange))
                        System.out.println("Stock updated");
                    else
                        System.err.println("Invalid stock name");
                }
                break;

                case "C":{
                    String stockName = scanner.next();
                    int stockCount = scanner.nextInt();
                    if(stockCenter.changeStockCount(stockName, stockCount))
                        System.out.println("Stock updated");
                    else
                        System.err.println("Invalid stock name");
                }
                break;

                case "E":{
                    break mainloop;
                }

                default:{
                    System.out.println("Invalid command");
                }
                break;
            }
        }

        scanner.close();

        server.stopServer();
    }
}