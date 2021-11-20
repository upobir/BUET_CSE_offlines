
import java.io.*;
import java.util.*;

import FoodOrder.*;

public class Main{
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        FoodOrderBuilder foodOrderBuilder = new FoodOrderBuilder();
        
        mainloop: while(true)
        {
            showMenu(System.out);
            int order = scanner.nextInt();

            switch(order){
                case 1:
                    foodOrderBuilder.addPizza("beef");
                    foodOrderBuilder.addAppetizer("french fries");
                    break;
                case 2:
                    foodOrderBuilder.addPizza("veggi");
                    foodOrderBuilder.addAppetizer("onion rings");
                    break;
                case 3:
                    foodOrderBuilder.addPizza("veggi");
                    foodOrderBuilder.addAppetizer("french fries");
                    foodOrderBuilder.addDrink("coke");
                    break;
                case 4:
                    foodOrderBuilder.addPizza("veggi");
                    foodOrderBuilder.addAppetizer("onion rings");
                    foodOrderBuilder.addDrink("coffee");
                    break;
                case 5:
                    foodOrderBuilder.addPizza("beef");
                    break;
                case 6:
                    buildCustomOrder(foodOrderBuilder, System.out, scanner);
                    break;
                default:
                    break mainloop;
            }

            FoodOrder foodOrder = foodOrderBuilder.getFoodOrder();
            if(foodOrder == null) 
                break mainloop;

            System.out.println(">>>> Price: " + foodOrder.getPrice());
            System.out.print(">>>> ");
            foodOrder.serve(System.out);
            
            foodOrderBuilder.clearOrder();
            System.out.println();
        }

        scanner.close();
    }

    private static void buildCustomOrder(FoodOrderBuilder foodOrderBuilder, PrintStream out, Scanner scanner) {
        out.println("Enter Pizza type (Beef/Veggi): ");
        foodOrderBuilder.addPizza(scanner.next());

        out.println("Enter Appetizer (Onion Rings/French Fries/None): ");
        foodOrderBuilder.addAppetizer(scanner.next());

        out.println("Enter Drinks (Coffee/Coke/None): ");
        foodOrderBuilder.addDrink(scanner.next());

        return;
    }

    private static void showMenu(PrintStream out) {
        out.println("Menu:");
        out.println("1. Beef Pizza with French fries");
        out.println("2. Veggi Pizza with onion rings");
        out.println("3. A combo meal with Veggi Pizza, French Fries and Coke");
        out.println("4. A combo meal with Veggi Pizza, Onion Rings and Coffee");
        out.println("5. A Beef Pizza only");
        out.println("6. Custom Order");
        out.print("Enter Order number: ");
        return;
    }
}