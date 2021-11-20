package FoodOrder.PizzaItem;

public class BeefPizza extends Pizza{

    @Override
    public double getPrice() {
        return 300.00;
    }

    @Override
    public String getName() {
        return "Beef Pizza";
    }
    
}
