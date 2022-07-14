package logic;

import vwmschejond.Car;

import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import java.util.*;

public class OrderedColumns {
    private List<CarWithNormalizedValue> maxSpeedColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> priceColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> horsePowerColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> ageColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> doorCountColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> trunkVolumeColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> speedometerColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> tankVolumeColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> oilVolumeColumn = new ArrayList<>();
    private List<CarWithNormalizedValue> numberOfSeatsColumn = new ArrayList<>();

    private int maxSpeedMin = Integer.MAX_VALUE, maxSpeedMax = Integer.MIN_VALUE, priceMin = Integer.MAX_VALUE, priceMax = Integer.MIN_VALUE, horsePowerMin = Integer.MAX_VALUE, horsePowerMax = Integer.MIN_VALUE;
    private int ageMin = Integer.MAX_VALUE, ageMax = Integer.MIN_VALUE, doorCountMin = Integer.MAX_VALUE, doorCountMax = Integer.MIN_VALUE, trunkVolumeMin = Integer.MAX_VALUE, trunkVolumeMax = Integer.MIN_VALUE;
    private int speedometerMin = Integer.MAX_VALUE, speedometerMax = Integer.MIN_VALUE, tankVolumeMin = Integer.MAX_VALUE, tankVolumeMax = Integer.MIN_VALUE;
    private int oilVolumeMin = Integer.MAX_VALUE, oilVolumeMax = Integer.MIN_VALUE, numberOfSeatsMin = Integer.MAX_VALUE, numberOfSeatsMax = Integer.MIN_VALUE;


    public void precompute(List<Car> cars) {
        //finds min and max value for each attribute
        for (Car car : cars) {
            //maxSpeed
            if (car.getMaxSpeed() < maxSpeedMin) {
                maxSpeedMin = car.getMaxSpeed();
            }
            if (car.getMaxSpeed() > maxSpeedMax) {
                maxSpeedMax = car.getMaxSpeed();
            }
            //price
            if (car.getPrice() < priceMin) {
                priceMin = car.getPrice();
            }
            if (car.getPrice() > priceMax) {
                priceMax = car.getPrice();
            }
            //horsePower
            if (car.getHorsePower() < horsePowerMin) {
                horsePowerMin = car.getHorsePower();
            }
            if (car.getHorsePower() > horsePowerMax) {
                horsePowerMax = car.getHorsePower();
            }
            //age
            if (car.getAge() < ageMin) {
                ageMin = car.getAge();
            }
            if (car.getAge() > ageMax) {
                ageMax = car.getAge();
            }
            //doorCount
            if (car.getDoorCount() < doorCountMin) {
                doorCountMin = car.getDoorCount();
            }
            if (car.getDoorCount() > doorCountMax) {
                doorCountMax = car.getDoorCount();
            }
            //trunkVolume
            if (car.getTrunkVolume() < trunkVolumeMin) {
                trunkVolumeMin = car.getTrunkVolume();
            }
            if (car.getTrunkVolume() > trunkVolumeMax) {
                trunkVolumeMax = car.getTrunkVolume();
            }
            //speedometer
            if (car.getSpeedometer() < speedometerMin) {
                speedometerMin = car.getSpeedometer();
            }
            if (car.getSpeedometer() > speedometerMax) {
                speedometerMax = car.getSpeedometer();
            }
            //tankVolume
            if (car.getTankVolume() < tankVolumeMin) {
                tankVolumeMin = car.getTankVolume();
            }
            if (car.getTankVolume() > tankVolumeMax) {
                tankVolumeMax = car.getTankVolume();
            }
            //oilVolume
            if (car.getOilVolume() < oilVolumeMin) {
                oilVolumeMin = car.getOilVolume();
            }
            if (car.getOilVolume() > oilVolumeMax) {
                oilVolumeMax = car.getOilVolume();
            }
            //numberOfSeats
            if (car.getNumberOfSeats() < numberOfSeatsMin) {
                numberOfSeatsMin = car.getNumberOfSeats();
            }
            if (car.getNumberOfSeats() > numberOfSeatsMax) {
                numberOfSeatsMax = car.getNumberOfSeats();
            }
        }
        //for each objects count its normalized value and inserts it into corresponding columnList
        for (Car car : cars) {
            //maxSpeed
            CarWithNormalizedValue maxSpeedCar = new CarWithNormalizedValue();
            maxSpeedCar.setNormalizedValue(computeNormalizedValue(maxSpeedMin, maxSpeedMax, car.getMaxSpeed()));
            maxSpeedCar.setCar(car);
            maxSpeedColumn.add(maxSpeedCar);
            //price
            CarWithNormalizedValue priceCar = new CarWithNormalizedValue();
            priceCar.setNormalizedValue(computeNormalizedValue(priceMin, priceMax, car.getPrice()));
            priceCar.setCar(car);
            priceColumn.add(priceCar);
            //horsePower
            CarWithNormalizedValue horsePowerCar = new CarWithNormalizedValue();
            horsePowerCar.setNormalizedValue(computeNormalizedValue(horsePowerMin, horsePowerMax, car.getHorsePower()));
            horsePowerCar.setCar(car);
            horsePowerColumn.add(horsePowerCar);
            //age
            CarWithNormalizedValue ageCar = new CarWithNormalizedValue();
            ageCar.setNormalizedValue(computeNormalizedValue(ageMin, ageMax, car.getAge()));
            ageCar.setCar(car);
            ageColumn.add(ageCar);
            //doorCount
            CarWithNormalizedValue doorCountCar = new CarWithNormalizedValue();
            doorCountCar.setNormalizedValue(computeNormalizedValue(doorCountMin, doorCountMax, car.getDoorCount()));
            doorCountCar.setCar(car);
            doorCountColumn.add(doorCountCar);
            //trunkVolume
            CarWithNormalizedValue trunkVolumeCar = new CarWithNormalizedValue();
            trunkVolumeCar.setNormalizedValue(computeNormalizedValue(trunkVolumeMin, trunkVolumeMax, car.getTrunkVolume()));
            trunkVolumeCar.setCar(car);
            trunkVolumeColumn.add(trunkVolumeCar);
            //speedometer
            CarWithNormalizedValue speedoMeterCar = new CarWithNormalizedValue();
            speedoMeterCar.setNormalizedValue(computeNormalizedValue(speedometerMin, speedometerMax, car.getSpeedometer()));
            speedoMeterCar.setCar(car);
            speedometerColumn.add(speedoMeterCar);
            //tankVolume
            CarWithNormalizedValue tankVolumeCar = new CarWithNormalizedValue();
            tankVolumeCar.setNormalizedValue(computeNormalizedValue(tankVolumeMin, tankVolumeMax, car.getTankVolume()));
            tankVolumeCar.setCar(car);
            tankVolumeColumn.add(tankVolumeCar);
            //oilVolume
            CarWithNormalizedValue oilVolumeCar = new CarWithNormalizedValue();
            oilVolumeCar.setNormalizedValue(computeNormalizedValue(oilVolumeMin, oilVolumeMax, car.getOilVolume()));
            oilVolumeCar.setCar(car);
            oilVolumeColumn.add(oilVolumeCar);
            //numberOfSeats
            CarWithNormalizedValue numberOfSeatsCar = new CarWithNormalizedValue();
            numberOfSeatsCar.setNormalizedValue(computeNormalizedValue(numberOfSeatsMin, numberOfSeatsMax, car.getNumberOfSeats()));
            numberOfSeatsCar.setCar(car);
            numberOfSeatsColumn.add(numberOfSeatsCar);
        }

        maxSpeedColumn.sort(new NormalizedValueComparator());
        priceColumn.sort(new NormalizedValueComparator());
        horsePowerColumn.sort(new NormalizedValueComparator());
        ageColumn.sort(new NormalizedValueComparator());
        doorCountColumn.sort(new NormalizedValueComparator());
        trunkVolumeColumn.sort(new NormalizedValueComparator());
        speedometerColumn.sort(new NormalizedValueComparator());
        tankVolumeColumn.sort(new NormalizedValueComparator());
        oilVolumeColumn.sort(new NormalizedValueComparator());
        numberOfSeatsColumn.sort(new NormalizedValueComparator());
        System.out.println("Data successfully precomputed");
    }

    float computeNormalizedValue(final int min, final int max, final int value) {
        final float formulaValue = ((float)value - min)/((float)max - min);
        if (formulaValue > 1 || formulaValue < 0) {
            throw new RuntimeException("Error while computing normalized value. From max: " + max + " and min: " + min +
                    " computed: " + formulaValue);
        }
        //rounding to 4 decimal places
        MathContext mathContext = new MathContext(4, RoundingMode.HALF_UP);
        BigDecimal bg = new BigDecimal(formulaValue,mathContext);
        return bg.floatValue();
    }

    public List<CarWithNormalizedValue> getMaxSpeedColumn() {
        return maxSpeedColumn;
    }

    public List<CarWithNormalizedValue> getPriceColumn() {
        return priceColumn;
    }

    public List<CarWithNormalizedValue> getHorsePowerColumn() {
        return horsePowerColumn;
    }

    public List<CarWithNormalizedValue> getAgeColumn() {
        return ageColumn;
    }

    public List<CarWithNormalizedValue> getDoorCountColumn() {
        return doorCountColumn;
    }

    public List<CarWithNormalizedValue> getTrunkVolumeColumn() {
        return trunkVolumeColumn;
    }

    public List<CarWithNormalizedValue> getSpeedometerColumn() {
        return speedometerColumn;
    }

    public List<CarWithNormalizedValue> getTankVolumeColumn() {
        return tankVolumeColumn;
    }

    public List<CarWithNormalizedValue> getOilVolumeColumn() {
        return oilVolumeColumn;
    }

    public List<CarWithNormalizedValue> getNumberOfSeatsColumn() {
        return numberOfSeatsColumn;
    }

    public int getMaxSpeedMin() {
        return maxSpeedMin;
    }

    public int getMaxSpeedMax() {
        return maxSpeedMax;
    }

    public int getPriceMin() {
        return priceMin;
    }

    public int getPriceMax() {
        return priceMax;
    }

    public int getHorsePowerMin() {
        return horsePowerMin;
    }

    public int getHorsePowerMax() {
        return horsePowerMax;
    }

    public int getAgeMin() {
        return ageMin;
    }

    public int getAgeMax() {
        return ageMax;
    }

    public int getDoorCountMin() {
        return doorCountMin;
    }

    public int getDoorCountMax() {
        return doorCountMax;
    }

    public int getTrunkVolumeMin() {
        return trunkVolumeMin;
    }

    public int getTrunkVolumeMax() {
        return trunkVolumeMax;
    }

    public int getSpeedometerMin() {
        return speedometerMin;
    }

    public int getSpeedometerMax() {
        return speedometerMax;
    }

    public int getTankVolumeMin() {
        return tankVolumeMin;
    }

    public int getTankVolumeMax() {
        return tankVolumeMax;
    }

    public int getOilVolumeMin() {
        return oilVolumeMin;
    }

    public int getOilVolumeMax() {
        return oilVolumeMax;
    }

    public int getNumberOfSeatsMin() {
        return numberOfSeatsMin;
    }

    public int getNumberOfSeatsMax() {
        return numberOfSeatsMax;
    }

    private class NormalizedValueComparator implements Comparator<CarWithNormalizedValue> {
        @Override
        public int compare(CarWithNormalizedValue c1, CarWithNormalizedValue c2) {
            return Float.compare(c2.getNormalizedValue(), c1.getNormalizedValue());
        }
    }

    public class CarWithNormalizedValue {
        private float normalizedValue;
        private Car car;

        public float getNormalizedValue() {
            return normalizedValue;
        }

        public void setNormalizedValue(float normalizedValue) {
            this.normalizedValue = normalizedValue;
        }

        public Car getCar() {
            return car;
        }

        public void setCar(Car car) {
            this.car = car;
        }
    }
}
