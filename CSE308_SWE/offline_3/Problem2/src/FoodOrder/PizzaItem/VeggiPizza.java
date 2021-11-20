package FoodOrder.PizzaItem;

public class VeggiPizza extends Pizza {

    @Override
    public double getPrice() {
        return 250.00;
    }

    @Override
    public String getName() {
        return "Veggi Pizza";
    }

}