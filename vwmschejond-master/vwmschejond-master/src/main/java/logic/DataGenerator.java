package logic;

import org.springframework.beans.factory.annotation.Autowired;
import vwmschejond.Car;
import vwmschejond.CarService;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public class DataGenerator {
    @Autowired
    private static CarService carService;

    public static List<Car> generateCars(final long numberOfRecordsToGenerate) {
        long id = 1;
        List<Car> cars = new ArrayList<>();
        for (long i = 0; i < numberOfRecordsToGenerate; i++) {
            Car car = new Car();
            car.setId(id);
            car.setMaxSpeed(ThreadLocalRandom.current().nextInt(80, 320));
            if (car.getMaxSpeed() > 170) {
                car.setPrice(ThreadLocalRandom.current().nextInt(700000, 2800000));
                car.setHorsePower(ThreadLocalRandom.current().nextInt(200, 600));
            } else {
                car.setPrice(ThreadLocalRandom.current().nextInt(20000, 1000000));
                car.setHorsePower(ThreadLocalRandom.current().nextInt(70, 210));
            }
            car.setAge(ThreadLocalRandom.current().nextInt(1, 20));
            car.setDoorCount(ThreadLocalRandom.current().nextInt(2, 7));
            car.setTrunkVolume(ThreadLocalRandom.current().nextInt(250, 400));
            car.setSpeedometer(ThreadLocalRandom.current().nextInt(0, 300000));
            car.setTankVolume(ThreadLocalRandom.current().nextInt(30, 90));
            car.setOilVolume(ThreadLocalRandom.current().nextInt(5, 10));
            car.setNumberOfSeats(ThreadLocalRandom.current().nextInt(2, 10));
            cars.add(car);
            id++;
        }
        return cars;
    }
}
